#pragma once
#include "spch.h"
#include "graphicObj/Texture.h"
#include <glm/glm.hpp>
namespace Six{
	class SIXAPI SShader
	{
	public:
		SShader(std::string vert, std::string frag);
		void Bind() const;
		void Unbind() const;
		unsigned int GetId() const { return m_id; }
		bool IsValid() const { return m_id != 0; }
		void SetUniform(const std::string& name, int v);
		void SetUniform(const std::string& name, float v);
		void SetUniform(const std::string& name, bool v);
		void SetUniform(const std::string& name, float v1,float v2);
		void SetUniform(const std::string& name, float v1, float v2, float v3);
		void SetUniform(const std::string& name, float v1, float v2, float v3,float v4);
		void SetUniform(const std::string& name, glm::vec2 v);
		void SetUniform(const std::string& name, glm::vec3 v);
		void SetUniform(const std::string& name, glm::vec4 v);
		void SetUniform(const std::string& name, glm::mat4 m);
		void SetUniform(const std::string& name, STexture* texture, int slot = 0);
	private:
		unsigned int m_id = 0;
	};
}

