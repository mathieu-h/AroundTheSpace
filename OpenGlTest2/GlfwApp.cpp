#include "stdafx.h"
#include "GlfwApp.h"

GlfwApp::GlfwApp() {
	// Initialize the GLFW system for creating and positioning windows
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
	glfwSetErrorCallback(ErrorCallback);
}

GlfwApp::~GlfwApp() {
	if (nullptr != window) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}

int GlfwApp::run() {
	preCreate();

	window = createRenderingTarget(windowSize, windowPosition);

	if (!window) {
		std::cout << "Unable to create OpenGL window" << std::endl;
		return -1;
	}

	postCreate();

	initGl();

	while (!glfwWindowShouldClose(window)) {
		++frame;
		glfwPollEvents();
		update();
		draw();
		finishFrame();
	}

	shutdownGl();

	return 0;
}

void GlfwApp::preCreate() {
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Without this line we get
	// FATAL (86): NSGL: The targeted version of OS X only supports OpenGL 3.2 and later versions if they are forward-compatible
	ON_MAC([]{
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	});
#ifdef DEBUG_BUILD
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
}

void GlfwApp::postCreate()  {
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize the OpenGL bindings
	// For some reason we have to set this experminetal flag to properly
	// init GLEW if we use a core context.
	glewExperimental = GL_TRUE;
	if (0 != glewInit()) {
		throw std::runtime_error("Failed to initialize GLEW");
	}
}

void GlfwApp::initGl() {
}

void GlfwApp::shutdownGl() {

}

void GlfwApp::finishFrame() {
	glfwSwapBuffers(window);
}

void GlfwApp::destroyWindow() {
	glfwSetKeyCallback(window, nullptr);
	glfwSetMouseButtonCallback(window, nullptr);
	glfwDestroyWindow(window);
}

void GlfwApp::onKey(int key, int scancode, int action, int mods) {
	if (GLFW_PRESS != action) {
		return;
	}

	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, 1);
		return;
	}
}

void GlfwApp::update() {  }

void GlfwApp::onMouseButton(int button, int action, int mods) { }

void GlfwApp::viewport(const ivec2 & pos, const uvec2 & size) {
	glViewport(pos.x, pos.y, size.x, size.y);
}