#include "stdafx.h"
#include "OculusManager.h"
#include "GLFW\glfw3.h"
#include <GLFW/glfw3native.h>
#include "Constants.h"



OculusManager& OculusManager::getOculusManager()
{

	static OculusManager* oculusManager = NULL;

	if (oculusManager == NULL)
	{
		if (!ovr_Initialize()) {
			fprintf(stderr, "Failed to initialize the Oculus SDK");
		}

		ovrHmd hmd; //= *OculusManager::getHmd();

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

		ovrGLConfig cfg;
		memset(&cfg, 0, sizeof(cfg));
		cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
		ovrSizei result;
		result.w = windowSize.x;
		result.h = windowSize.y;
		cfg.OGL.Header.RTSize = result;
		cfg.OGL.Header.Multisample = 0;

		int distortionCaps = 0
			| ovrDistortionCap_Vignette
			| ovrDistortionCap_Chromatic
			| ovrDistortionCap_Overdrive
			| ovrDistortionCap_TimeWarp
			;
		ovrEyeRenderDesc eyeRenderDescs[2];

		int configResult = ovrHmd_ConfigureRendering(hmd, &cfg.Config,
			distortionCaps, hmd->MaxEyeFov, eyeRenderDescs);

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

GLFWwindow * OculusManager::createWindow(const uvec2 & size, const ivec2 & position ) {
	GLFWwindow * window = glfwCreateWindow(size.x, size.y, Constants::game_name(), nullptr, nullptr);
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