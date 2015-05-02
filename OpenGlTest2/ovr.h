#include "stdafx.h"

#include <OVR_CAPI_GL.h>
#include "glfw.h"

namespace ovr {

	// Convenience method for looping over each eye with a lambda
	template <typename Function>
	inline void for_each_eye(Function function) {
		for (ovrEyeType eye = ovrEyeType::ovrEye_Left;
			eye < ovrEyeType::ovrEye_Count;
			eye = static_cast<ovrEyeType>(eye + 1)) {
			function(eye);
		}
	}

	inline mat4 toGlm(const ovrMatrix4f & om) {
		return glm::transpose(glm::make_mat4(&om.M[0][0]));
	}

	inline mat4 toGlm(const ovrFovPort & fovport, float nearPlane = 0.01f, float farPlane = 10000.0f) {
		return toGlm(ovrMatrix4f_Projection(fovport, nearPlane, farPlane, true));
	}

	inline vec3 toGlm(const ovrVector3f & ov) {
		return glm::make_vec3(&ov.x);
	}

	inline vec2 toGlm(const ovrVector2f & ov) {
		return glm::make_vec2(&ov.x);
	}

	inline uvec2 toGlm(const ovrSizei & ov) {
		return uvec2(ov.w, ov.h);
	}

	inline quat toGlm(const ovrQuatf & oq) {
		return glm::make_quat(&oq.x);
	}

	inline mat4 toGlm(const ovrPosef & op) {
		mat4 orientation = glm::mat4_cast(toGlm(op.Orientation));
		mat4 translation = glm::translate(mat4(), ovr::toGlm(op.Position));
		return translation * orientation;
	}

	inline ovrMatrix4f fromGlm(const mat4 & m) {
		ovrMatrix4f result;
		mat4 transposed(glm::transpose(m));
		memcpy(result.M, &(transposed[0][0]), sizeof(float) * 16);
		return result;
	}

	inline ovrVector3f fromGlm(const vec3 & v) {
		ovrVector3f result;
		result.x = v.x;
		result.y = v.y;
		result.z = v.z;
		return result;
	}

	inline ovrVector2f fromGlm(const vec2 & v) {
		ovrVector2f result;
		result.x = v.x;
		result.y = v.y;
		return result;
	}

	inline ovrSizei fromGlm(const uvec2 & v) {
		ovrSizei result;
		result.w = v.x;
		result.h = v.y;
		return result;
	}

	inline ovrQuatf fromGlm(const quat & q) {
		ovrQuatf result;
		result.x = q.x;
		result.y = q.y;
		result.z = q.z;
		result.w = q.w;
		return result;
	}


	inline GLFWwindow * createRiftRenderingWindow(ovrHmd hmd, glm::uvec2 & outSize, glm::ivec2 & outPosition) {
		GLFWwindow * window = nullptr;
		bool directHmdMode = false;

		outPosition = glm::ivec2(hmd->WindowsPos.x, hmd->WindowsPos.y);
		outSize = glm::uvec2(hmd->Resolution.w, hmd->Resolution.h);

		// The ovrHmdCap_ExtendDesktop only reliably reports on Windows currently
		ON_WINDOWS([&]{
			directHmdMode = (0 == (ovrHmdCap_ExtendDesktop & hmd->HmdCaps));
		});

		// In direct HMD mode, we always use the native resolution, because the
		// user has no control over it.
		// In legacy mode, we should be using the current resolution of the Rift
		// (unless we're creating a 'fullscreen' window)
		if (!directHmdMode) {
			GLFWmonitor * monitor = glfw::getMonitorAtPosition(outPosition);
			if (nullptr != monitor) {
				auto mode = glfwGetVideoMode(monitor);
				outSize = glm::uvec2(mode->width, mode->height);
			}
		}

		// On linux it's recommended to leave the screen in it's default portrait orientation.
		// The SDK currently allows no mechanism to test if this is the case.  I could query
		// GLFW for the current resolution of the Rift, but that sounds too much like actual
		// work.
		ON_LINUX([&]{
			std::swap(outSize.x, outSize.y);
		});

		if (directHmdMode) {
			// In direct mode, try to put the output window on a secondary screen
			// (for easier debugging, assuming your dev environment is on the primary)
			window = glfw::createSecondaryScreenWindow(outSize);
		}
		else {
			// If we're creating a desktop window, we should strip off any window decorations
			// which might change the location of the rendered contents relative to the lenses.
			//
			// Another alternative would be to create the window in fullscreen mode, on
			// platforms that support such a thing.
			glfwWindowHint(GLFW_DECORATED, 0);
			window = glfw::createWindow(outSize, outPosition);
		}

		// If we're in direct mode, attach to the window
		if (directHmdMode) {
			void * nativeWindowHandle = glfw::getNativeWindowHandle(window);
			if (nullptr != nativeWindowHandle) {
				ovrHmd_AttachToWindow(hmd, nativeWindowHandle, nullptr, nullptr);
			}
		}

		return window;
	}
}