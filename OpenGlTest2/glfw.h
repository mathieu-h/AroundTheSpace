#include "stdafx.h"
#include <iostream>

#include <GLFW/glfw3.h>

#if (defined(WIN64) || defined(WIN32))
#define OS_WIN
#elif (defined(__APPLE__))
#define OS_OSX
#endif

#if defined(OS_WIN)
#define ON_WINDOWS(runnable) runnable()
#define ON_MAC(runnable)
#define ON_LINUX(runnable)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#undef NOMINMAX
#elif defined(OS_OSX)
#define ON_WINDOWS(runnable) 
#define ON_MAC(runnable) runnable()
#define ON_LINUX(runnable)
#endif

#if defined(OS_WIN)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif defined(OS_OSX)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#endif

#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;


namespace glfw {
	inline uvec2 getSize(GLFWmonitor * monitor) {
		const GLFWvidmode * mode = glfwGetVideoMode(monitor);
		return uvec2(mode->width, mode->height);
	}

	inline ivec2 getPosition(GLFWmonitor * monitor) {
		ivec2 result;
		glfwGetMonitorPos(monitor, &result.x, &result.y);
		return result;
	}

	inline ivec2 getSecondaryScreenPosition(const uvec2 & size) {
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

	inline GLFWmonitor * getMonitorAtPosition(const ivec2 & position) {
		int count;
		GLFWmonitor ** monitors = glfwGetMonitors(&count);
		for (int i = 0; i < count; ++i) {
			ivec2 candidatePosition;
			glfwGetMonitorPos(monitors[i], &candidatePosition.x, &candidatePosition.y);
			if (candidatePosition == position) {
				return monitors[i];
			}
		}
		return nullptr;
	}

	inline GLFWwindow * createWindow(const uvec2 & size, const ivec2 & position = ivec2(INT_MIN)) {
		GLFWwindow * window = glfwCreateWindow(size.x, size.y, "glfw", nullptr, nullptr);
		if (!window) {
			throw std::runtime_error("Unable to create rendering window");
		}
		if ((position.x > INT_MIN) && (position.y > INT_MIN)) {
			glfwSetWindowPos(window, position.x, position.y);
		}
		return window;
	}

	inline GLFWwindow * createWindow(int w, int h, int x = INT_MIN, int y = INT_MIN) {
		return createWindow(uvec2(w, h), ivec2(x, y));
	}

	inline GLFWwindow * createFullscreenWindow(const uvec2 & size, GLFWmonitor * targetMonitor) {
		return glfwCreateWindow(size.x, size.y, "glfw", targetMonitor, nullptr);
	}

	inline GLFWwindow * createSecondaryScreenWindow(const uvec2 & size) {
		return createWindow(size, glfw::getSecondaryScreenPosition(size));
	}

	inline void * getNativeWindowHandle(GLFWwindow * window) {
		void * nativeWindowHandle = nullptr;
		ON_WINDOWS([&]{
			nativeWindowHandle = (void*)glfwGetWin32Window(window);
		});
		ON_LINUX([&]{
			nativeWindowHandle = (void*)glfwGetX11Window(window);
		});
		ON_MAC([&]{
			nativeWindowHandle = (void*)glfwGetCocoaWindow(window);
		});
		return nativeWindowHandle;
	}

}