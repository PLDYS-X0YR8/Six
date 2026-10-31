#pragma once
#include "sixcore.h"
#include <filesystem>

namespace Six {
	class SIXAPI STexture {
	public:
		//STexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
		static STexture* Create(const std::filesystem::path& filepath);
		virtual ~STexture() = 0;
		virtual void Bind(unsigned int slot) const = 0;
		virtual void Unbind() const = 0;
		virtual unsigned int GetID() const { return m_id; }
		virtual unsigned int GetWidth() const { return m_width; }
		virtual unsigned int GetHeight() const { return m_height; }
		virtual unsigned int GetChannels() const { return m_channels; }
		virtual unsigned char* GetData() const { return m_data; }
	protected:
		unsigned int m_id;
		int m_width;
		int m_height;
		int m_channels;
		unsigned char* m_data;
	};
}