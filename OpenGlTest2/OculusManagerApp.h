#include "stdafx.h"
#include <iostream>

#define GLM_FORCE_RADIANS

#include <OVR_CAPI_GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Import the most commonly used types into the default namespace
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;

class OculusManagerApp
{

protected:
	ovrHmd hmd;

	uvec2 hmdNativeResolution;
	ivec2 hmdDesktopPosition;

public:
	OculusManagerApp(ovrHmdType defaultHmdType = ovrHmd_DK2);
	virtual ~OculusManagerApp();

	int getEnabledCaps();
	void enableCaps(int caps);
	void toggleCap(ovrHmdCaps cap);
	bool isEnabled(ovrHmdCaps cap);
	void disableCaps(int caps);

};
