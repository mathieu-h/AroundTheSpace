#include "stdafx.h"
#include "glfw.h"


class GlfwApp
{
public:
	GlfwApp();
	~GlfwApp();
	virtual int run();

	uvec2 windowSize;
	ivec2 windowPosition;
	GLFWwindow * window{ nullptr };
	unsigned int frame{ 0 };

	virtual GLFWwindow * createRenderingTarget(uvec2 & size, ivec2 & pos) = 0;
	virtual void draw() = 0;

	void preCreate();
	void postCreate();
	virtual void initGl();
	virtual void shutdownGl();
	virtual void finishFrame();
	virtual void destroyWindow();
	virtual void onKey(int key, int scancode, int action, int mods);
	virtual void update();
	virtual void onMouseButton(int button, int action, int mods);
	virtual void viewport(const ivec2 & pos, const uvec2 & size);

private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		GlfwApp * instance = (GlfwApp *)glfwGetWindowUserPointer(window);
		instance->onKey(key, scancode, action, mods);
	}

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		GlfwApp * instance = (GlfwApp *)glfwGetWindowUserPointer(window);
		instance->onMouseButton(button, action, mods);
	}

	static void ErrorCallback(int error, const char* description) {
		throw std::runtime_error(description);
	}

};

