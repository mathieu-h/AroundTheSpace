/*#include "stdafx.h"
#include "OculusManagerApp.h"
#include "ovr.h"


class OculusApp : public OculusManagerApp
{

	struct FramebufferWraper {
		glm::uvec2 size;
		GLuint fbo{ 0 };
		GLuint color{ 0 };
		GLuint depth{ 0 };

		virtual ~FramebufferWraper() {
			release();
		}

		void allocate() {
			release();
			glGenRenderbuffers(1, &depth);
			assert(depth);
			glGenTextures(1, &color);
			assert(color);
			glGenFramebuffers(1, &fbo);
			assert(fbo);
		}

		void release() {
			if (fbo) {
				glDeleteFramebuffers(1, &fbo);
				fbo = 0;
			}
			if (color) {
				glDeleteTextures(1, &color);
				color = 0;
			}
			if (depth) {
				glDeleteRenderbuffers(1, &depth);
				depth = 0;
			}
		}

		static bool checkStatus(GLenum target = GL_FRAMEBUFFER) {
			GLuint status = glCheckFramebufferStatus(target);
			switch (status) {
			case GL_FRAMEBUFFER_COMPLETE:
				return true;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cerr << "framebuffer incomplete attachment" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cerr << "framebuffer missing attachment" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				std::cerr << "framebuffer incomplete draw buffer" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				std::cerr << "framebuffer incomplete read buffer" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				std::cerr << "framebuffer incomplete multisample" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				std::cerr << "framebuffer incomplete layer targets" << std::endl;
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cerr << "framebuffer unsupported internal format or image" << std::endl;
				break;

			default:
				std::cerr << "other framebuffer error" << std::endl;
				break;
			}

			return false;
		}

		void init(const glm::ivec2 & size) {
			this->size = size;
			allocate();

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glBindTexture(GL_TEXTURE_2D, color);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, size.x, size.y);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

			glBindRenderbuffer(GL_RENDERBUFFER, depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size.x, size.y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

			if (!checkStatus()) {
				throw std::runtime_error("Could not create a valid framebuffer");
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void activate() {
			assert(fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glViewport(0, 0, size.x, size.y);
		}
	};

	class GlfwApp {

	protected:
		uvec2 windowSize;
		ivec2 windowPosition;
		GLFWwindow * window{ nullptr };
		unsigned int frame{ 0 };

	public:
		GlfwApp() {
			// Initialize the GLFW system for creating and positioning windows
			if (!glfwInit()) {
				throw std::runtime_error("Failed to initialize GLFW");
			}
			glfwSetErrorCallback(ErrorCallback);
		}

		virtual ~GlfwApp() {
			if (nullptr != window) {
				glfwDestroyWindow(window);
			}
			glfwTerminate();
		}

		virtual int run() {
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


	protected:
		virtual GLFWwindow * createRenderingTarget(uvec2 & size, ivec2 & pos) = 0;

		virtual void draw() = 0;

		void preCreate() {
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

		void postCreate()  {
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

		virtual void initGl() {
		}

		virtual void shutdownGl() {

		}

		virtual void finishFrame() {
			glfwSwapBuffers(window);
		}

		virtual void destroyWindow() {
			glfwSetKeyCallback(window, nullptr);
			glfwSetMouseButtonCallback(window, nullptr);
			glfwDestroyWindow(window);
		}

		virtual void onKey(int key, int scancode, int action, int mods) {
			if (GLFW_PRESS != action) {
				return;
			}

			switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, 1);
				return;
			}
		}

		virtual void update() {  }

		virtual void onMouseButton(int button, int action, int mods) { }

	protected:
		virtual void viewport(const ivec2 & pos, const uvec2 & size) {
			glViewport(pos.x, pos.y, size.x, size.y);
		}

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

	
};*/