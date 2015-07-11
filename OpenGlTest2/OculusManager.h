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
};