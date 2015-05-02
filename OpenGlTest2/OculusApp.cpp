#include "stdafx.h"
#include "OculusApp.h"
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
