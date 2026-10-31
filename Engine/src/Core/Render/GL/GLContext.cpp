#include "spch.h"
#include "GLContext.h"
#include "../GraphicsContext.h"
#include <glad/glad.h>
#include <glfw3.h>

namespace Six {
	S_GLContext::S_GLContext(GLFWwindow* window) {
		m_window = window;
	}
	bool S_GLContext::Init() {
		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		return (bool)status;
	}

	void S_GLContext::SwapBuffers() {
		glfwSwapBuffers(m_window);
	}
}