#include "spch.h"
#include "Window.h"
#include "GraphicsContext.h"
#include <glad/glad.h>
#include <glfw3.h>
#include "../Event/Event.h"
#include "../Event/EventQueue.h"

namespace Six {
	SWindow::~SWindow() {
		OnShutdown();
	}
	SWindow::SWindow(int w, int h, std::string t,bool& ok) {
		m_data.width = w;
		m_data.height = h;
		m_data.title = t;
		ok = OnInit();
	}
	bool SWindow::OnInit(){
		if (!glfwInit()) {
			std::cerr << "Failed to initialize GLFW" << std::endl;
			return false;
		}
		if (!m_window)
		{
			m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		}
		if (!m_window) {
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		m_context = SGraphicsContext::Create(m_window);
		if(!m_context->Init()) return false;
		glViewport(0, 0, m_data.width, m_data.height);
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			SWinClosedEvent* event = new SWinClosedEvent();
			SEventQueue::Get().Push(event);
			});
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			SWinResizedEvent* event = new SWinResizedEvent(width, height);
			SEventQueue::Get().Push(event);
			});
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS || action == GLFW_REPEAT) {
				SKeyPressedEvent* event = new SKeyPressedEvent(static_cast<SKeyCodes>(key), action == GLFW_REPEAT ? 1 : 0);
				SEventQueue::Get().Push(event);
			}
			else {
				SKeyReleasedEvent* event = new SKeyReleasedEvent(static_cast<SKeyCodes>(key));
				SEventQueue::Get().Push(event);
			}
			});
		glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t codepoint) {
			SKeyTypedEvent* event = new SKeyTypedEvent(codepoint);
			SEventQueue::Get().Push(event);
			});
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mod) {
			if (action == GLFW_PRESS || action == GLFW_REPEAT) {
				SMouseButtonPressedEvent* event = new SMouseButtonPressedEvent(static_cast<SMouseButton>(button), action == GLFW_REPEAT ? 1 : 0);
				SEventQueue::Get().Push(event);
			}
			else {
				SMouseButtonReleasedEvent* event = new SMouseButtonReleasedEvent(static_cast<SMouseButton>(button));
				SEventQueue::Get().Push(event);
			}
			});
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			SMouseMovedEvent* event = new SMouseMovedEvent(xpos, ypos);
			SEventQueue::Get().Push(event);
			});
		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			SMouseScrolledEvent* event = new SMouseScrolledEvent(xoffset, yoffset);
			SEventQueue::Get().Push(event);
			});
		return true;
	}
	void SWindow::OnUpdate(){
		m_context->SwapBuffers();
		glfwPollEvents();
	}
	void SWindow::OnShutdown(){
		if (m_window)
		{
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
		glfwTerminate();
	}
	bool SWindow::ShouldClose(){
		return glfwWindowShouldClose(m_window);
	}

	bool SWindow::IsVsync(){
		return m_data.isvsync;
	}
	void SWindow::SetVsync(bool enable){
		m_data.isvsync = enable;
		glfwSwapInterval((bool)m_data.isvsync);
	}
	void SWindow::SetWidth(int w){
		m_data.width = w;
		glViewport(0, 0, m_data.width, m_data.height);
	}
	void SWindow::SetHeight(int h){
		m_data.height = h;
		glViewport(0, 0, m_data.width, m_data.height);
	}
	void SWindow::SetTitle(std::string t){
		m_data.title = t;
	}
	const SWindow::SWindowData& SWindow::GetData() const{
		return m_data;
	}
	const SWindow::SWindowData& SWindow::GetData() {
		return m_data;
	}
}