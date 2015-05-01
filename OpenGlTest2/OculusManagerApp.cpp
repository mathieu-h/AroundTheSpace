#include "stdafx.h"
#include "OculusManagerApp.h"


OculusManagerApp::OculusManagerApp(ovrHmdType defaultHmdType)
{
	//hmd = ovrHmd_Create(0);
	//if (nullptr == hmd) {
	//	hmd = ovrHmd_CreateDebug(defaultHmdType);
	//	hmdDesktopPosition = ivec2(100, 100);
	//	hmdNativeResolution = uvec2(1200, 800);
	//}
	//else {
	//	hmdDesktopPosition = ivec2(hmd->WindowsPos.x, hmd->WindowsPos.y);
	//	hmdNativeResolution = ivec2(hmd->Resolution.w, hmd->Resolution.h);
	//}
}


OculusManagerApp::~OculusManagerApp()
{
	//ovrHmd_Destroy(hmd);
	//hmd = nullptr;
}

int OculusManagerApp::getEnabledCaps() {
	//return ovrHmd_GetEnabledCaps(hmd);
	return 0;
}

void OculusManagerApp::enableCaps(int caps) {
	//ovrHmd_SetEnabledCaps(hmd, getEnabledCaps() | caps);
}

void OculusManagerApp::toggleCap(ovrHmdCaps cap) {
	if (cap & getEnabledCaps()) {
		disableCaps(cap);
	}
	else {
		enableCaps(cap);
	}
}

bool OculusManagerApp::isEnabled(ovrHmdCaps cap) {
	//return (cap == (cap & getEnabledCaps()));
	return false;
}

void OculusManagerApp::disableCaps(int caps) {
	//ovrHmd_SetEnabledCaps(hmd, getEnabledCaps() & ~caps);
}