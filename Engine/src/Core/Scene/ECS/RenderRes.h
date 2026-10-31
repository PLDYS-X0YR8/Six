#pragma once
#include "spch.h"
#include "../Component.h"
#include "Core/Render/Mesh.h"
#include "Core/Render/graphicObj/Material.h"

namespace Six {
	class SIXAPI SRenderRes : public SComponent {
		SIX_COMPONENT(SRenderRes)
	public:
		SRenderRes(SMesh* mesh, SMaterial* mat)
			:m_mesh(mesh), m_mat(mat) {

		}
		SMesh* GetMesh() const { return m_mesh.get(); }
		SShader* GetShader() const { return m_mat->shader; }
		SMaterial* GetMaterial() const { return m_mat.get(); }
		std::string toStr() override {
			return "SRenderRes";
		}
	private:
		std::unique_ptr<SMesh> m_mesh;
		std::unique_ptr<SMaterial> m_mat;
	};
}