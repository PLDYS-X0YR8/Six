#pragma once
#include "sixcore.h"
#include "Texture.h"

namespace Six {
	class SIXAPI SViewport {
	public:
		static SViewport* Create(int width, int height);
		virtual bool InitViewport(int width, int height) = 0;
		virtual void ResizeViewport(int width, int height) = 0;
		virtual void BindViewport() = 0;
		virtual void UnbindViewport() = 0;
		virtual const STexture* GetViewportColorAttachment() = 0;
		// 直接拿 FBO 真实渲染的 GL color texture id(ImGui::Image 用这个)
		virtual unsigned int GetColorTextureID() const = 0;
		// viewport 尺寸(算 projection aspect 用)
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	protected:
		unsigned int m_tFBO = 0;
		unsigned int m_viewportColorTex = 0;
		unsigned int m_viewportDepthRBO = 0;
		int m_width = 0;
		int m_height = 0;
		const STexture* m_tex = nullptr;
	private:
		virtual void ReleaseViewport() = 0;
	};
}