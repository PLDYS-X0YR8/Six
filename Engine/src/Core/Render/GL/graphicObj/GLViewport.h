#pragma once
#include "../../graphicObj/Viewport.h"

namespace Six {
	class SIXAPI S_GLViewport : public SViewport {
	public:
		S_GLViewport(int width, int height);
		~S_GLViewport();
		static SViewport* Create(int width, int height);
		virtual bool InitViewport(int width, int height) override;
		virtual void ResizeViewport(int width, int height) override;
		virtual void BindViewport() override;
		virtual void UnbindViewport() override;
		virtual const STexture* GetViewportColorAttachment() override;
		virtual unsigned int GetColorTextureID() const override;
		virtual int GetWidth() const override { return m_width; }
		virtual int GetHeight() const override { return m_height; }
	private:
		virtual void ReleaseViewport() override;
	};
}
