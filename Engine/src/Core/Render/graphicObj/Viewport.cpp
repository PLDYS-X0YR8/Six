#include "spch.h"
#include "Viewport.h"
#include "../RenderAPI.h"
#include "../GL/graphicObj/GLViewport.h"

namespace Six {
	SViewport* SViewport::Create(int width, int height) {
		switch (SRenderAPI::GetBackendAPI()) {
		case SRenderAPI::SBackendAPI::OpenGL:return S_GLViewport::Create(width, height);
		}
	}
}