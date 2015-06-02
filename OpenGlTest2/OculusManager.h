#include "stdafx.h"
#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define OVR_OS_WIN32
#endif

#include "Entity.h"
#include <OVR_CAPI_GL.h>

using glm::ivec2;
using glm::uvec2;

class OculusManager
{
private:

public:
	static GLFWwindow * createRiftRenderingWindow(ovrHmd hmd, glm::uvec2 & outSize, glm::ivec2 & outPosition);
	static OculusManager& getOculusManager();
	static void destroyOculusManager();

	static GLFWwindow * createWindow(const uvec2 & size, const ivec2 & position = ivec2(INT_MIN));
	static GLFWwindow * createWindow(int w, int h, int x = INT_MIN, int y = INT_MIN);
	static GLFWwindow * createFullscreenWindow(const uvec2 & size, GLFWmonitor * targetMonitor);
	static GLFWwindow * createSecondaryScreenWindow(const uvec2 & size);
	static ivec2 getSecondaryScreenPosition(const uvec2 & size);
	static uvec2 getSize(GLFWmonitor * monitor);
	static ivec2 getPosition(GLFWmonitor * monitor);
};