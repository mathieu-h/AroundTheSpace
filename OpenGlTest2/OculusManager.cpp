#include "stdafx.h"
#include "OculusManager.h"
#include "GLFW\glfw3.h"
#include <GLFW/glfw3native.h>
#include "Constants.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLuint l_FBOId;
static GLuint l_DepthBufferId;
static GLuint l_TextureId;

static GLFWwindow* l_Window;

static ovrHmd g_Hmd;
static ovrGLConfig g_Cfg;
static ovrEyeRenderDesc g_EyeRenderDesc[2];
static ovrVector3f g_EyeOffsets[2];
static ovrPosef g_EyePoses[2];
static ovrTexture g_EyeTextures[2];
static OVR::Matrix4f g_ProjectionMatrici[2];
static OVR::Sizei g_RenderTargetSize;
static ovrVector3f g_CameraPosition;

const bool l_MultiSampling = false;
const bool l_Spin = false;

static int g_DistortionCaps = 0
| ovrDistortionCap_Vignette
| ovrDistortionCap_Chromatic
| ovrDistortionCap_Overdrive
// | ovrDistortionCap_TimeWarp // Turning this on gives ghosting???
;

static std::vector<GLfloat> vertices2 = std::vector<GLfloat>();
static std::vector<GLuint> indices2 = std::vector<GLuint>();

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

		g_Hmd = ovrHmd_Create(0);
		if (!g_Hmd)
		{
			printf("No Oculus Rift device attached, using virtual version...\n");
			g_Hmd = ovrHmd_CreateDebug(ovrHmd_DK2);
		}
		printf("initialized HMD: %s - %s\n", g_Hmd->Manufacturer, g_Hmd->ProductName);

		if (!glfwInit()) exit(EXIT_FAILURE);

		if (l_MultiSampling) glfwWindowHint(GLFW_SAMPLES, 4); else glfwWindowHint(GLFW_SAMPLES, 0);

		bool l_DirectMode = ((g_Hmd->HmdCaps & ovrHmdCap_ExtendDesktop) == 0);

		GLFWmonitor* l_Monitor;
		ovrSizei l_ClientSize;
		if (l_DirectMode)
		{
			printf("Running in \"Direct\" mode...\n");
			l_Monitor = NULL;

			l_ClientSize.w = g_Hmd->Resolution.w / 2; // Something reasonable, smaller, but maintain aspect ratio...
			l_ClientSize.h = g_Hmd->Resolution.h / 2; // Something reasonable, smaller, but maintain aspect ratio...
		}
		else // Extended Desktop mode...
		{
			printf("Running in \"Extended Desktop\" mode...\n");
			int l_Count;
			GLFWmonitor** l_Monitors = glfwGetMonitors(&l_Count);
			switch (l_Count)
			{
			case 0:
				printf("No monitors found, exiting...\n");
				exit(EXIT_FAILURE);
				break;
			case 1:
				printf("Two monitors expected, found only one, using primary...\n");
				l_Monitor = glfwGetPrimaryMonitor();
				break;
			case 2:
				printf("Two monitors found, using second monitor...\n");
				l_Monitor = l_Monitors[1];
				break;
			default:
				printf("More than two monitors found, using second monitor...\n");
				l_Monitor = l_Monitors[1];
			}

			l_ClientSize.w = g_Hmd->Resolution.w; // 1920 for DK2...
			l_ClientSize.h = g_Hmd->Resolution.h; // 1080 for DK2...
		}

		l_Window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", l_Monitor, NULL);

		if (!l_Window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

#if defined(_WIN32)
		if (l_DirectMode)
		{
			ovrBool l_AttachResult = ovrHmd_AttachToWindow(g_Hmd, glfwGetWin32Window(l_Window), NULL, NULL);
			if (!l_AttachResult)
			{
				printf("Could not attach to window...");
				exit(EXIT_FAILURE);
			}
		}
#endif

		glfwMakeContextCurrent(l_Window);

		glewExperimental = GL_TRUE;
		GLenum l_GlewResult = glewInit();
		if (l_GlewResult != GLEW_OK)
		{
			printf("glewInit() error.\n");
			exit(EXIT_FAILURE);
		}

		int l_Major = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MAJOR);
		int l_Minor = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MINOR);
		int l_Profile = glfwGetWindowAttrib(l_Window, GLFW_OPENGL_PROFILE);
		printf("OpenGL: %d.%d ", l_Major, l_Minor);
		if (l_Major >= 3) // Profiles introduced in OpenGL 3.0...
		{
			if (l_Profile == GLFW_OPENGL_COMPAT_PROFILE) printf("GLFW_OPENGL_COMPAT_PROFILE\n"); else printf("GLFW_OPENGL_CORE_PROFILE\n");
		}
		printf("Vendor: %s\n", (char*)glGetString(GL_VENDOR));
		printf("Renderer: %s\n", (char*)glGetString(GL_RENDERER));

		ovrSizei l_EyeTextureSizes[2];

		l_EyeTextureSizes[ovrEye_Left] = ovrHmd_GetFovTextureSize(g_Hmd, ovrEye_Left, g_Hmd->MaxEyeFov[ovrEye_Left], 1.0f);
		l_EyeTextureSizes[ovrEye_Right] = ovrHmd_GetFovTextureSize(g_Hmd, ovrEye_Right, g_Hmd->MaxEyeFov[ovrEye_Right], 1.0f);

		// Combine for one texture for both eyes...
		g_RenderTargetSize.w = l_EyeTextureSizes[ovrEye_Left].w + l_EyeTextureSizes[ovrEye_Right].w;
		g_RenderTargetSize.h = (l_EyeTextureSizes[ovrEye_Left].h > l_EyeTextureSizes[ovrEye_Right].h ? l_EyeTextureSizes[ovrEye_Left].h : l_EyeTextureSizes[ovrEye_Right].h);

		// Create the FBO being a single one for both eyes (this is open for debate)...
		glGenFramebuffers(1, &l_FBOId);
		glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

		// The texture we're going to render to...
		glGenTextures(1, &l_TextureId);
		// "Bind" the newly created texture : all future texture functions will modify this texture...
		glBindTexture(GL_TEXTURE_2D, l_TextureId);
		// Give an empty image to OpenGL (the last "0")
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_RenderTargetSize.w, g_RenderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		// Linear filtering...
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Create Depth Buffer...
		glGenRenderbuffers(1, &l_DepthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, l_DepthBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, g_RenderTargetSize.w, g_RenderTargetSize.h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, l_DepthBufferId);

		// Set the texture as our colour attachment #0...
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, l_TextureId, 0);

		// Set the list of draw buffers...
		GLenum l_GLDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, l_GLDrawBuffers); // "1" is the size of DrawBuffers

		// Check if everything is OK...
		GLenum l_Check = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (l_Check != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("There is a problem with the FBO.\n");
			exit(EXIT_FAILURE);
		}

		// Unbind...
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Setup textures for each eye...

		// Left eye...
		g_EyeTextures[ovrEye_Left].Header.API = ovrRenderAPI_OpenGL;
		g_EyeTextures[ovrEye_Left].Header.TextureSize = g_RenderTargetSize;
		g_EyeTextures[ovrEye_Left].Header.RenderViewport.Pos.x = 0;
		g_EyeTextures[ovrEye_Left].Header.RenderViewport.Pos.y = 0;
		g_EyeTextures[ovrEye_Left].Header.RenderViewport.Size = l_EyeTextureSizes[ovrEye_Left];
		((ovrGLTexture&)(g_EyeTextures[ovrEye_Left])).OGL.TexId = l_TextureId;

		// Right eye (mostly the same as left but with the viewport on the right side of the texture)...
		g_EyeTextures[ovrEye_Right] = g_EyeTextures[ovrEye_Left];
		g_EyeTextures[ovrEye_Right].Header.RenderViewport.Pos.x = (g_RenderTargetSize.w + 1) / 2;
		g_EyeTextures[ovrEye_Right].Header.RenderViewport.Pos.y = 0;

		// Oculus Rift eye configurations...
		g_Cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
		g_Cfg.OGL.Header.RTSize.w = l_ClientSize.w;
		g_Cfg.OGL.Header.RTSize.h = l_ClientSize.h;
		g_Cfg.OGL.Header.Multisample = (l_MultiSampling ? 1 : 0);
#if defined(_WIN32)
		g_Cfg.OGL.Window = glfwGetWin32Window(l_Window);
		g_Cfg.OGL.DC = GetDC(g_Cfg.OGL.Window);
#elif defined(__linux__)
		l_Cfg.OGL.Win = glfwGetX11Window(l_Window);
		l_Cfg.OGL.Disp = glfwGetX11Display();
#endif

		// Enable capabilities...
		// ovrHmd_SetEnabledCaps(g_Hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

		ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(g_Hmd, &g_Cfg.Config, g_DistortionCaps, g_Hmd->MaxEyeFov, g_EyeRenderDesc);
		glUseProgram(0); // Avoid OpenGL state leak in ovrHmd_ConfigureRendering...
		if (!l_ConfigureResult)
		{
			printf("Configure failed.\n");
			exit(EXIT_FAILURE);
		}

		// Start the sensor which provides the Rift’s pose and motion...
		uint32_t l_SupportedSensorCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position;
		uint32_t l_RequiredTrackingCaps = 0;
		ovrBool l_TrackingResult = ovrHmd_ConfigureTracking(g_Hmd, l_SupportedSensorCaps, l_RequiredTrackingCaps);
		if (!l_TrackingResult)
		{
			printf("Could not start tracking...");
			exit(EXIT_FAILURE);
		}

		// Projection matrici for each eye will not change at runtime, we can set them here...
		g_ProjectionMatrici[ovrEye_Left] = ovrMatrix4f_Projection(g_EyeRenderDesc[ovrEye_Left].Fov, 0.3f, 100.0f, true);
		g_ProjectionMatrici[ovrEye_Right] = ovrMatrix4f_Projection(g_EyeRenderDesc[ovrEye_Right].Fov, 0.3f, 100.0f, true);

		// IPD offset values will not change at runtime, we can set them here...
		g_EyeOffsets[ovrEye_Left] = g_EyeRenderDesc[ovrEye_Left].HmdToEyeViewOffset;
		g_EyeOffsets[ovrEye_Right] = g_EyeRenderDesc[ovrEye_Right].HmdToEyeViewOffset;

		ovrHmd_RecenterPose(g_Hmd);


		return *oculusManager;
	}
}

void OculusManager::render(RenderSystem* render, Scene* scene)
{
	unsigned int l_FrameIndex = 0;
	int i;

	/* the drawing starts with a call to ovrHmd_BeginFrame */
	ovrHmd_BeginFrame(g_Hmd, l_FrameIndex);

	ovrHmd_GetEyePoses(g_Hmd, l_FrameIndex, g_EyeOffsets, g_EyePoses, NULL);

	/* start drawing onto our texture render target */
	glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* for each eye ... */
	for (int l_EyeIndex = 0; l_EyeIndex < ovrEye_Count; l_EyeIndex++)
	{
		ovrEyeType l_Eye = g_Hmd->EyeRenderOrder[l_EyeIndex];

		glViewport(
			g_EyeTextures[l_Eye].Header.RenderViewport.Pos.x,
			g_EyeTextures[l_Eye].Header.RenderViewport.Pos.y,
			g_EyeTextures[l_Eye].Header.RenderViewport.Size.w,
			g_EyeTextures[l_Eye].Header.RenderViewport.Size.h
			);

		// Pass projection matrix on to OpenGL...
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(&(g_ProjectionMatrici[l_Eye].Transposed().M[0][0]));

		// Create the model-view matrix and pass on to OpenGL...
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Multiply with orientation retrieved from sensor...
		OVR::Quatf l_Orientation = OVR::Quatf(g_EyePoses[l_Eye].Orientation);
		OVR::Matrix4f l_ModelViewMatrix = OVR::Matrix4f(l_Orientation.Inverted());
		glMultMatrixf(&(l_ModelViewMatrix.Transposed().M[0][0]));

		// Translation due to positional tracking (DK2) and IPD...
		glTranslatef(-g_EyePoses[l_Eye].Position.x, -g_EyePoses[l_Eye].Position.y, -g_EyePoses[l_Eye].Position.z);

		// Move the world forward a bit to show the scene in front of us...
		glTranslatef(g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z);


		//Render
		render->renderOcculus(scene->getChildren(), scene->getLights(), (GLfloat)g_EyeTextures[l_Eye].Header.RenderViewport.Size.w, (GLfloat)g_EyeTextures[l_Eye].Header.RenderViewport.Size.h);
	}

	/* after drawing both eyes into the texture render target, revert to drawing directly to the
	* display, and we call ovrHmd_EndFrame, to let the Oculus SDK draw both images properly
	* compensated for lens distortion and chromatic abberation onto the HMD screen.
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ovrHmd_EndFrame(g_Hmd, g_EyePoses, g_EyeTextures);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Avoid OpenGL state leak in ovrHmd_EndFrame...
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Avoid OpenGL state leak in ovrHmd_EndFrame...

	glUseProgram(0);

	++l_FrameIndex;
}


void OculusManager::destroyOculusManager()
{
	glDeleteRenderbuffers(1, &l_DepthBufferId);
	glDeleteTextures(1, &l_TextureId);
	glDeleteFramebuffers(1, &l_FBOId);

	// Clean up Oculus...
	ovrHmd_Destroy(g_Hmd);
	ovr_Shutdown();

	// Clean up window...
	glfwDestroyWindow(l_Window);
	glfwTerminate();
}