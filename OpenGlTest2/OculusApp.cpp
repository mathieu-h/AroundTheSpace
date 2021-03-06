#include "stdafx.h"
/*#include "OculusApp.h"
#include "ovr.h"



OculusApp::OculusApp()
{
	if (!ovrHmd_ConfigureTracking(hmd,
		ovrTrackingCap_Orientation | ovrTrackingCap_Position | ovrTrackingCap_MagYawCorrection, 0)) {
		throw std::runtime_error("Could not attach to sensor device");
	}

	memset(eyeTextures, 0, 2 * sizeof(ovrGLTexture));

	ovr::for_each_eye([&](ovrEyeType eye){
		ovrSizei eyeTextureSize = ovrHmd_GetFovTextureSize(hmd, eye, hmd->MaxEyeFov[eye], 1.0f);
		ovrTextureHeader & eyeTextureHeader = eyeTextures[eye].Header;
		eyeTextureHeader.TextureSize = eyeTextureSize;
		eyeTextureHeader.RenderViewport.Size = eyeTextureSize;
		eyeTextureHeader.API = ovrRenderAPI_OpenGL;
	});
}


OculusApp::~OculusApp()
{
}


GLFWwindow * OculusApp::createRenderingTarget(uvec2 & outSize, ivec2 & outPosition) {
	return ovr::createRiftRenderingWindow(hmd, outSize, outPosition);
}

void OculusApp::initGl() {
	GlfwApp::initGl();

	ovrGLConfig cfg;
	memset(&cfg, 0, sizeof(cfg));
	cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	cfg.OGL.Header.RTSize = ovr::fromGlm(windowSize);
	cfg.OGL.Header.Multisample = 0;

	int distortionCaps = 0
		| ovrDistortionCap_Vignette
		| ovrDistortionCap_Chromatic
		| ovrDistortionCap_Overdrive
		| ovrDistortionCap_TimeWarp
		;

	ON_LINUX([&]{
		// This cap bit causes the SDK to properly handle the
		// Rift in portrait mode.
		distortionCaps |= ovrDistortionCap_LinuxDevFullscreen;

		// On windows, the SDK does a good job of automatically
		// finding the correct window.  On Linux, not so much.
		cfg.OGL.Disp = glfwGetX11Display();
		cfg.OGL.Win = glfwGetX11Window(window);
	});

	int configResult = ovrHmd_ConfigureRendering(hmd, &cfg.Config,
		distortionCaps, hmd->MaxEyeFov, eyeRenderDescs);


	ovr::for_each_eye([&](ovrEyeType eye){
		const ovrEyeRenderDesc & erd = eyeRenderDescs[eye];
		ovrMatrix4f ovrPerspectiveProjection = ovrMatrix4f_Projection(erd.Fov, OVR_DEFAULT_IPD * 4, 100000.0f, true);
		projections[eye] = ovr::toGlm(ovrPerspectiveProjection);
		eyeOffsets[eye] = erd.HmdToEyeViewOffset;

		// Allocate the frameBuffer that will hold the scene, and then be
		// re-rendered to the screen with distortion
		auto & eyeTextureHeader = eyeTextures[eye];
		eyeFbos[eye].init(ovr::toGlm(eyeTextureHeader.Header.TextureSize));
		// Get the actual OpenGL texture ID
		((ovrGLTexture&)eyeTextureHeader).OGL.TexId = eyeFbos[eye].color;
	});

	class OculusApp : public GlfwApp, public OculusManagerApp
	{

	protected:
		ovrTexture eyeTextures[2];
		ovrVector3f eyeOffsets[2];

		GLFWwindow* createRenderingTarget(uvec2 & outSize, ivec2 & outPosition);
		virtual void initGl();

	private:
		ovrEyeType currentEye{ ovrEye_Count };
		ovrEyeRenderDesc eyeRenderDescs[2];
		mat4 projections[2];
		ovrPosef eyePoses[2];
		//FramebufferWraper eyeFbos[2];
		FramebufferWraper eyeFbos[2];

	public:
		OculusApp();
		~OculusApp();
	};
}*/