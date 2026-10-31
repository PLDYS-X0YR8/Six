#include "spch.h"
#include "RenderAPI.h"
#include "GraphicsContext.h"
#include "GL/GLContext.h"
//#include "VK/VKContext.h"

namespace Six {
	SGraphicsContext* SGraphicsContext::Create(GLFWwindow* window) {
		switch (SRenderAPI::GetBackendAPI()) {
		case SRenderAPI::SBackendAPI::OpenGL:return new S_GLContext(window);
		//case SRenderAPI::SBackendAPI::Vulkan:return new S_VKContext(window);
		}
		return nullptr;
	}
}