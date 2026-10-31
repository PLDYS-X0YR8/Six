#include "spch.h"
#include "Texture.h"
#include "Core/Render/RenderAPI.h"
#include "Core/Render/GL/graphicObj/GLTexture.h"

namespace Six {
	STexture::~STexture() = default;

	STexture* STexture::Create(const std::filesystem::path& filepath) {
		switch (SRenderAPI::GetBackendAPI()) {
		case SRenderAPI::SBackendAPI::OpenGL:return S_GLTexture::Create(filepath);
		}
		return nullptr;
	}
}
