#include "spch.h"
#include "RenderAPI.h"
#include "GL/GLRenderAPI.h"
//#include "VK/VKRenderAPI.h"
//#include "DX/DXRenderAPI.h"

namespace Six {
	SRenderAPI::SBackendAPI SRenderAPI::s_backend = SRenderAPI::SBackendAPI::OpenGL;
	SRenderAPI* SRenderAPI::Create() {
		switch (s_backend) {
		case SBackendAPI::OpenGL:return new S_GLRenderAPI();
		//case SBackendAPI::Vulkan:return new S_VKRenderAPI();
		//case SBackendAPI::DirectX:return new S_DXRenderAPI();
		}
		return nullptr;
	}
}