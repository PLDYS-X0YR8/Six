#include "spch.h"
#include "Environment.h"
#include "../Entity.h"
#include "RenderRes.h"

namespace Six {
	bool SEnvironment::Init() {
		return true;
	}

	void SEnvironment::Update() {
		auto renderEntities = m_owner->Filter([](const SEntity& e) {return e.HasComponent<SRenderRes>();});
		for (const auto& entity : renderEntities) {
			const auto& renderRes = entity->GetComponent<SRenderRes>();
			renderRes->GetMaterial()->shader->SetUniform("uAmbient.sky",m_sky);
			renderRes->GetMaterial()->shader->SetUniform("uAmbient.ground",m_ground);
			renderRes->GetMaterial()->shader->SetUniform("uAmbient.intensity",m_intensity);
		}
	}

	void SEnvironment::End() {
		
	}

	void SEnvironment::SetSkyColor(glm::vec3 s) { m_sky = s; }
	void SEnvironment::SetGroundColor(glm::vec3 g) { m_ground = g; }
	void SEnvironment::SetIntensity(float i) { m_intensity = i; }
	glm::vec3 SEnvironment::GetSkyColor() { return m_sky; }
	glm::vec3 SEnvironment::GetGroundColor() { return m_ground; }
	float SEnvironment::GetIntensity() { return m_intensity; }
	glm::vec3* SEnvironment::GetSkyColorPtr() { return &m_sky; }
	glm::vec3* SEnvironment::GetGroundColorPtr() { return &m_ground; }
	float* SEnvironment::GetIntensityPtr() { return &m_intensity; }
}