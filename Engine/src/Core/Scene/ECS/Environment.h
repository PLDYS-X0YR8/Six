#pragma once
#include "sixcore.h"
#include "../System.h"
#include <glm/glm.hpp>

namespace Six {
	class SIXAPI SEnvironment :public SSystem {
		SIX_SYSTEM(SEnvironment)
	public:
		void SetSkyColor(glm::vec3 s);
		void SetGroundColor(glm::vec3 g);
		void SetIntensity(float i);
		glm::vec3 GetSkyColor();
		glm::vec3 GetGroundColor();
		float GetIntensity();
		glm::vec3* GetSkyColorPtr();
		glm::vec3* GetGroundColorPtr();
		float* GetIntensityPtr();
		virtual bool Init() override;
		virtual void Update() override;
		virtual void End() override;
		virtual std::string toStr() override {
			return "SEnvironment";
		}
	private:
		glm::vec3 m_sky{ 0.20f,0.22f,0.28f };
		glm::vec3 m_ground{ 0.10f,0.09f,0.08f };
		float m_intensity = 1;
	};
}