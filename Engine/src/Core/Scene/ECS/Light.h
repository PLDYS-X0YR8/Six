#pragma once
#include "../Component.h"
#include <glm/glm.hpp>

namespace Six {
	class SIXAPI SLight : public SComponent {
	public:
		float intensity = 10.f;
		glm::vec3 color = { 1.f,1.f,1.f };
	};
	class SIXAPI SDirLight :public SLight {
		SIX_COMPONENT(SDirLight)
	public:
		glm::vec3 dir{ 0.f,0.f,0.f };
		std::string toStr() override {
			return "Direction Light";
		}
	};
}