#include "stdafx.h"
#include "OculusManager.h"
#include "GLFW\glfw3.h"
#include <GLFW/glfw3native.h>
#include "Constants.h"

static unsigned int fbo, fb_tex, fb_depth;
static int fb_tex_width, fb_tex_height;
static ovrTexture fb_ovr_tex[2];
static ovrHmd hmd;
static ovrSizei eyeres[2];
static int fb_width, fb_height;
static ovrEyeRenderDesc eyeRenderDescs[2];
static OVR::Matrix4f g_ProjectionMatrici[2];
static ovrVector3f g_EyeOffsets[2];
static ovrPosef g_EyePoses[2];

OculusManager& OculusManager::getOculusManager()
{

	static OculusManager* oculusManager = NULL;

	if (oculusManager == NULL)
	{
		oculusManager = new OculusManager();
		if (!ovr_Initialize()) {
			fprintf(stderr, "Failed to initialize the Oculus SDK");
		}

		//= *OculusManager::getHmd();

		if (!(hmd = ovrHmd_Create(0))) {
			fprintf(stderr, "failed to open Oculus HMD, falling back to virtual debug HMD\n");
			if (!(hmd = ovrHmd_CreateDebug(ovrHmd_DK2))) {
				fprintf(stderr, "failed to create virtual debug HMD\n");
			}
		}
		printf("initialized HMD: %s - %s\n", hmd->Manufacturer, hmd->ProductName);

		if (!ovrHmd_ConfigureTracking(hmd,
			ovrTrackingCap_Orientation | ovrTrackingCap_Position | ovrTrackingCap_MagYawCorrection, 0)) {
			fprintf(stderr, "Could not attach to sensor device");
		}

		glfwInit();
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_SAMPLES, 16);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		uvec2 windowSize;
		ivec2 windowPosition;
		GLFWwindow * window = OculusManager::createRiftRenderingWindow(hmd, windowSize, windowPosition);

		glfwMakeContextCurrent(window);
		GLenum error = glewInit();

		eyeres[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0);
		eyeres[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0);

		/* and create a single render target texture to encompass both eyes */
		fb_width = eyeres[0].w + eyeres[1].w;
		fb_height = eyeres[0].h > eyeres[1].h ? eyeres[0].h : eyeres[1].h;
		update_rtarg(fb_width, fb_height);

		/* fill in the ovrGLTexture structures that describe our render target texture */
		for (int i = 0; i < 2; i++) {
			fb_ovr_tex[i].Header.API = ovrRenderAPI_OpenGL;
			fb_ovr_tex[i].Header.TextureSize.w = fb_tex_width;
			fb_ovr_tex[i].Header.TextureSize.h = fb_tex_height;
			/* this next field is the only one that differs between the two eyes */
			fb_ovr_tex[i].Header.RenderViewport.Pos.x = i == 0 ? 0 : fb_width / 2.0;
			fb_ovr_tex[i].Header.RenderViewport.Pos.y = 0;
			fb_ovr_tex[i].Header.RenderViewport.Size.w = fb_width / 2.0;
			fb_ovr_tex[i].Header.RenderViewport.Size.h = fb_height;
			((ovrGLTexture&)(fb_ovr_tex[i])).OGL.TexId = fb_tex;
		}


		ovrGLConfig cfg;
		memset(&cfg, 0, sizeof(cfg));
		cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
		ovrSizei result;
		result.w = windowSize.x;
		result.h = windowSize.y;
		cfg.OGL.Header.RTSize = result;
		cfg.OGL.Header.Multisample = 0;
		cfg.OGL.Window = glfwGetWin32Window(window);

		int distortionCaps = 0
			| ovrDistortionCap_Vignette
			| ovrDistortionCap_Chromatic
			| ovrDistortionCap_Overdrive
			| ovrDistortionCap_TimeWarp
			;

		printf("SDK Version : %s \n", ovr_GetVersionString());

		int configResult = ovrHmd_ConfigureRendering(hmd, &cfg.Config,
			distortionCaps, hmd->MaxEyeFov, eyeRenderDescs);

		// Projection matrici for each eye will not change at runtime, we can set them here...
		g_ProjectionMatrici[ovrEye_Left] = ovrMatrix4f_Projection(eyeRenderDescs[ovrEye_Left].Fov, 0.3f, 100.0f, true);
		g_ProjectionMatrici[ovrEye_Right] = ovrMatrix4f_Projection(eyeRenderDescs[ovrEye_Right].Fov, 0.3f, 100.0f, true);

		// IPD offset values will not change at runtime, we can set them here...
		g_EyeOffsets[ovrEye_Left] = eyeRenderDescs[ovrEye_Left].HmdToEyeViewOffset;
		g_EyeOffsets[ovrEye_Right] = eyeRenderDescs[ovrEye_Right].HmdToEyeViewOffset;

		ovrHmd_RecenterPose(hmd);

	}

	return *oculusManager;
}

GLFWwindow * OculusManager::createRiftRenderingWindow(ovrHmd hmd, glm::uvec2 & outSize, glm::ivec2 & outPosition) {
	GLFWwindow * window = nullptr;
	bool directHmdMode = false;

	outPosition = glm::ivec2(hmd->WindowsPos.x, hmd->WindowsPos.y);
	outSize = glm::uvec2(hmd->Resolution.w, hmd->Resolution.h);

	directHmdMode = (0 == (ovrHmdCap_ExtendDesktop & hmd->HmdCaps));

	window = createSecondaryScreenWindow(outSize);

	void * nativeWindowHandle = glfwGetWin32Window(window);
	if (nullptr != nativeWindowHandle) {
		ovrHmd_AttachToWindow(hmd, nativeWindowHandle, nullptr, nullptr);
	}

	return window;
}

GLFWwindow * OculusManager::createWindow(const uvec2 & size, const ivec2 & position) {
	GLFWwindow * window = glfwCreateWindow(size.x, size.y, Constants::getConstants()->game_name(), nullptr, nullptr);
	if (!window) {
		fprintf(stderr, "Unable to create rendering window");
	}
	if ((position.x > INT_MIN) && (position.y > INT_MIN)) {
		glfwSetWindowPos(window, position.x, position.y);
	}
	return window;
}

GLFWwindow * OculusManager::createWindow(int w, int h, int x, int y) {
	return createWindow(uvec2(w, h), ivec2(x, y));
}

GLFWwindow * OculusManager::createFullscreenWindow(const uvec2 & size, GLFWmonitor * targetMonitor) {
	return glfwCreateWindow(size.x, size.y, "glfw", targetMonitor, nullptr);
}

GLFWwindow * OculusManager::createSecondaryScreenWindow(const uvec2 & size) {
	return createWindow(size, getSecondaryScreenPosition(size));
}

ivec2 OculusManager::getSecondaryScreenPosition(const uvec2 & size) {
	GLFWmonitor * primary = glfwGetPrimaryMonitor();
	int monitorCount;
	GLFWmonitor ** monitors = glfwGetMonitors(&monitorCount);
	GLFWmonitor * best = nullptr;
	uvec2 bestSize;
	for (int i = 0; i < monitorCount; ++i) {
		GLFWmonitor * cur = monitors[i];
		if (cur == primary) {
			continue;
		}
		uvec2 curSize = getSize(cur);
		if (best == nullptr || (bestSize.x < curSize.x && bestSize.y < curSize.y)) {
			best = cur;
			bestSize = curSize;
		}
	}
	if (nullptr == best) {
		best = primary;
		bestSize = getSize(best);
	}
	ivec2 pos = getPosition(best);
	if (bestSize.x > size.x) {
		pos.x += (bestSize.x - size.x) / 2;
	}

	if (bestSize.y > size.y) {
		pos.y += (bestSize.y - size.y) / 2;
	}

	return pos;
}

uvec2 OculusManager::getSize(GLFWmonitor * monitor) {
	const GLFWvidmode * mode = glfwGetVideoMode(monitor);
	return uvec2(mode->width, mode->height);
}

ivec2 OculusManager::getPosition(GLFWmonitor * monitor) {
	ivec2 result;
	glfwGetMonitorPos(monitor, &result.x, &result.y);
	return result;
}

/* update_rtarg creates (and/or resizes) the render target used to draw the two stero views */
void OculusManager::update_rtarg(int width, int height)
{
	if (!fbo) {
		/* if fbo does not exist, then nothing does... create every opengl object */
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &fb_tex);
		glGenRenderbuffers(1, &fb_depth);

		glBindTexture(GL_TEXTURE_2D, fb_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	/* calculate the next power of two in both dimensions and use that as a texture size */
	fb_tex_width = next_pow2(width);
	fb_tex_height = next_pow2(height);

	/* create and attach the texture that will be used as a color buffer */
	glBindTexture(GL_TEXTURE_2D, fb_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb_tex_width, fb_tex_height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);

	/* create and attach the renderbuffer that will serve as our z-buffer */
	glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fb_tex_width, fb_tex_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "incomplete framebuffer!\n");

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	printf("created render target: %dx%d (texture size: %dx%d)\n", width, height, fb_tex_width, fb_tex_height);
}

unsigned int OculusManager::next_pow2(unsigned int x)
{
	x -= 1;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
}

void OculusManager::render(RenderSystem* render, Scene* scene)
{
	unsigned int l_FrameIndex = 0;
	int i;
	//ovrMatrix4f proj;
	//ovrPosef pose[2];
	//float rot_mat[16];

	/* the drawing starts with a call to ovrHmd_BeginFrame */
	ovrHmd_BeginFrame(hmd, 0);

	ovrHmd_GetEyePoses(hmd, 0, g_EyeOffsets, g_EyePoses, NULL);

	/* start drawing onto our texture render target */
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* for each eye ... */
	for (i = 0; i < 2; i++) {
		ovrEyeType eye = hmd->EyeRenderOrder[i];

		glViewport(
			fb_ovr_tex[eye].Header.RenderViewport.Pos.x,
			fb_ovr_tex[eye].Header.RenderViewport.Pos.y,
			fb_ovr_tex[eye].Header.RenderViewport.Size.w,
			fb_ovr_tex[eye].Header.RenderViewport.Size.h
			);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(&(g_ProjectionMatrici[eye].Transposed().M[0][0]));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		OVR::Quatf l_Orientation = OVR::Quatf(g_EyePoses[eye].Orientation);
		OVR::Matrix4f l_ModelViewMatrix = OVR::Matrix4f(l_Orientation.Inverted());
		glMultMatrixf(&(l_ModelViewMatrix.Transposed().M[0][0]));

		glTranslatef(-g_EyePoses[eye].Position.x, -g_EyePoses[eye].Position.y, -g_EyePoses[eye].Position.z);

		//glTranslatef(g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z);


		render->render(scene->getChildren(), scene->getLights());
	}

	/* after drawing both eyes into the texture render target, revert to drawing directly to the
	* display, and we call ovrHmd_EndFrame, to let the Oculus SDK draw both images properly
	* compensated for lens distortion and chromatic abberation onto the HMD screen.
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ovrHmd_EndFrame(hmd, g_EyePoses, fb_ovr_tex);

	glUseProgram(0);

	++ l_FrameIndex;
}

/* convert a quaternion to a rotation matrix */
void OculusManager::quat_to_matrix(const float *quat, float *mat)
{
	mat[0] = 1.0 - 2.0 * quat[1] * quat[1] - 2.0 * quat[2] * quat[2];
	mat[4] = 2.0 * quat[0] * quat[1] + 2.0 * quat[3] * quat[2];
	mat[8] = 2.0 * quat[2] * quat[0] - 2.0 * quat[3] * quat[1];
	mat[12] = 0.0f;

	mat[1] = 2.0 * quat[0] * quat[1] - 2.0 * quat[3] * quat[2];
	mat[5] = 1.0 - 2.0 * quat[0] * quat[0] - 2.0 * quat[2] * quat[2];
	mat[9] = 2.0 * quat[1] * quat[2] + 2.0 * quat[3] * quat[0];
	mat[13] = 0.0f;

	mat[2] = 2.0 * quat[2] * quat[0] + 2.0 * quat[3] * quat[1];
	mat[6] = 2.0 * quat[1] * quat[2] - 2.0 * quat[3] * quat[0];
	mat[10] = 1.0 - 2.0 * quat[0] * quat[0] - 2.0 * quat[1] * quat[1];
	mat[14] = 0.0f;

	mat[3] = mat[7] = mat[11] = 0.0f;
	mat[15] = 1.0f;
}