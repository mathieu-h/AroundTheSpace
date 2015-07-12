#include "stdafx.h"
#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define OVR_OS_WIN32
#endif

#include "Entity.h"
#include <OVR_CAPI_GL.h>
#include "RenderSystem.h"
#include "Scene.h"
#include "OVR.h"

using glm::ivec2;
using glm::uvec2;

class OculusManager
{
private:

public:

	static OculusManager& getOculusManager();
	static void destroyOculusManager();

	void render(RenderSystem* render, Scene* scene);

	inline glm::vec3 toGlm(const ovrVector3f & ov) {
		return glm::make_vec3(&ov.x);
	}

	inline glm::quat toGlm(const ovrQuatf & oq) {
		return glm::make_quat(&oq.x);
	}

	inline glm::mat4 toGlm(const ovrPosef & op) {
		glm::mat4 orientation = glm::mat4_cast(toGlm(op.Orientation));
		glm::mat4 translation = glm::translate(glm::mat4(), toGlm(op.Position));
		return translation * orientation;
	}
};