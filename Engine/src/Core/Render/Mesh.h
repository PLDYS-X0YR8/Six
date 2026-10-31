#pragma once
#include "spch.h"

namespace Six {
	struct SIXAPI SVertexElement {
		int location = 0;
		int size = 0;
		int type = 0;
		int offset = 0;
		SVertexElement(int l,int s,int t,int o)
			:location(l),size(s),type(t),offset(o){ }
	};
	struct SIXAPI SVertexLayout {
		std::vector<SVertexElement> elements;
		int stride = 0;
	};
	class SIXAPI SMesh {
	public:
		SMesh(SVertexLayout& layout, std::vector<float>& vertices, std::vector<uint32_t>& indices);
		void Destroy();
		void MeshSetup();
		void Bind() const;
		void Unbind() const;
		void Draw() const;
	private:
		std::vector<float> m_vertices;
		std::vector<uint32_t> m_indices;
		uint32_t m_VAO = 0, m_VBO = 0,m_EBO = 0;
		SVertexLayout m_layout;
	};
}