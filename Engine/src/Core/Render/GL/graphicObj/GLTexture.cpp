#include "spch.h"
#include "GLTexture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstring>
#include <vector>

namespace Six {
	S_GLTexture::S_GLTexture(int width, int height, int channels, unsigned char* data) {
		m_width = width;
		m_height = height;
		m_channels = channels;
		m_data = data;
		GLenum format = (m_channels == 4) ? GL_RGBA :
			(m_channels == 3) ? GL_RGB :
			(m_channels == 1) ? GL_RED : GL_RGB;
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format,
			m_width, m_height, 0,
			format, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	S_GLTexture* S_GLTexture::Create(const std::filesystem::path& filepath) {
		int w, h, c;
		unsigned char* data = stbi_load(filepath.string().c_str(), &w, &h, &c, 0);
		if (!data) return nullptr;

		// Flip Y in-place: PNG row 0 is top, GL texture row 0 is bottom.
		// stbi_set_flip_vertically_on_load doesn't reliably kick in across all TU/link
		// paths, so we flip explicitly here.
		const int stride = w * c;
		std::vector<unsigned char> tmp(stride);
		for (int y = 0; y < h / 2; ++y) {
			unsigned char* top = data + y * stride;
			unsigned char* bot = data + (h - 1 - y) * stride;
			std::memcpy(tmp.data(), top, stride);
			std::memcpy(top, bot, stride);
			std::memcpy(bot, tmp.data(), stride);
		}

		return new S_GLTexture(w, h, c, data);
	}
	S_GLTexture::~S_GLTexture() {
		if (m_id) glDeleteTextures(1, &m_id);
	}
	void S_GLTexture::Bind(unsigned int slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
	void S_GLTexture::Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}