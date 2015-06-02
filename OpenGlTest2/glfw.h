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


