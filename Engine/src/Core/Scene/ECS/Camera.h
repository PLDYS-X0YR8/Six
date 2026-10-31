#pragma once
#include "../Component.h"
#include "../Entity.h"
#include "Transform.h"
#include "Core/Render/Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Six {
	class SIXAPI SCamera :public SComponent {
		SIX_COMPONENT(SCamera)
	public:
		bool is_main = false;
		bool is_orthographic = false;
		float foSize = 60.f;
		float nearPlane = 0.1f;
		float farPlane = 1000.f;
		glm::mat4 GetProjectMat() {
			auto data = SRenderer::Get().GetWindow()->GetData();
			// 注意: width/height 都是 int,必须先转 float 再除,
			// 否则 1280/720 整数除法 = 1 → aspect 变 1.0 → 画面横向拉伸 1.778 倍
			float aspect = (float)data.width / (float)(data.height ? data.height : 1);
			if (is_orthographic) {
				float h = foSize * 0.5f, w = h * aspect;
				return glm::ortho(w, -w, h, -h,nearPlane,farPlane);
			}
			return glm::perspective(glm::radians(foSize), aspect, nearPlane, farPlane);
		}
		glm::mat4 GetProjectMat(float aspect) {
			auto data = SRenderer::Get().GetWindow()->GetData();
			// 注意: width/height 都是 int,必须先转 float 再除,
			// 否则 1280/720 整数除法 = 1 → aspect 变 1.0 → 画面横向拉伸 1.778 倍
			if (is_orthographic) {
				float h = foSize * 0.5f, w = h * aspect;
				return glm::ortho(w, -w, h, -h, nearPlane, farPlane);
			}
			return glm::perspective(glm::radians(foSize), aspect, nearPlane, farPlane);
		}
		glm::mat4 GetViewMat() {
			SIX_REQUARED_COMPONENT(STransform)
			auto trans = m_owner->GetComponent<STransform>();
			return glm::lookAt(trans->position, trans->position+trans->GetForward(), { 0.f,1.f,0.f });
		}
		std::string toStr() override {
			return "Camera foSize:" + std::to_string(foSize);
		}
	};
}