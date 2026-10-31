#include "spch.h"
#include "GLViewport.h"
#include "GLTexture.h"
#include <glad/glad.h>

namespace Six {
	S_GLViewport::S_GLViewport(int width, int height) {
		InitViewport(width, height);
	}

	S_GLViewport::~S_GLViewport() {
		ReleaseViewport();
	}

	SViewport* S_GLViewport::Create(int width, int height) {
		return new S_GLViewport(width, height);
	}

	void S_GLViewport::ReleaseViewport() {
		if (m_tFBO) { glDeleteFramebuffers(1, &m_tFBO); m_tFBO = 0; }
		if (m_viewportDepthRBO) { glDeleteRenderbuffers(1, &m_viewportDepthRBO); m_viewportDepthRBO = 0; }
		if (m_viewportColorTex) { glDeleteTextures(1, &m_viewportColorTex); m_viewportColorTex = 0; }
		if (m_tex) { delete m_tex; m_tex = nullptr; }
		m_width = m_height = 0;
	}

	bool S_GLViewport::InitViewport(int width, int height) {
		ReleaseViewport();
		m_width = width;
		m_height = height;

		glGenTextures(1, &m_viewportColorTex);
		glBindTexture(GL_TEXTURE_2D, m_viewportColorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &m_viewportDepthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_viewportDepthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glGenFramebuffers(1, &m_tFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_tFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_viewportColorTex, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_viewportDepthRBO);

		bool ok = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (!ok) {
			ReleaseViewport();
			return false;
		}

		m_tex = new S_GLTexture(width, height, 4, nullptr);
		return true;
	}

	void S_GLViewport::ResizeViewport(int width, int height) {
		if (width == m_width && height == m_height) return;
		InitViewport(width, height);
	}

	void S_GLViewport::BindViewport() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_tFBO);
		glViewport(0, 0, m_width, m_height);
	}

	void S_GLViewport::UnbindViewport() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const STexture* S_GLViewport::GetViewportColorAttachment() {
		return m_tex;
	}

	unsigned int S_GLViewport::GetColorTextureID() const {
		// FBO 真正渲染到的 color texture(不是 m_tex 那个独立空 texture)
		return m_viewportColorTex;
	}
}
