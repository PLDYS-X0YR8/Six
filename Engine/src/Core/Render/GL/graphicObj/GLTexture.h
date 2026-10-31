#pragma once
#include "sixcore.h"
#include "../../graphicObj/Texture.h"
#include <filesystem>

namespace Six {
	class SIXAPI S_GLTexture:public STexture {
	public:
		S_GLTexture(int width, int height, int channels,unsigned char* data);
		static S_GLTexture* Create(const std::filesystem::path& filepath);
		virtual ~S_GLTexture() override;
		virtual void Bind(unsigned int slot = 0) const override;
		virtual void Unbind() const override;
		virtual unsigned int GetID() const { return m_id; }
		virtual unsigned int GetWidth() const { return m_width; }
		virtual unsigned int GetHeight() const { return m_height; }
		virtual unsigned int GetChannels() const { return m_channels; }
		virtual unsigned char* GetData() const { return m_data; }
	};
}