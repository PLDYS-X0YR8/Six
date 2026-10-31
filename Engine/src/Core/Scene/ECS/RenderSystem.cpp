#include "spch.h"
#include "Core/Scene/ECS/RenderSystem.h"

namespace Six {
	bool SRenderSystem::Init() {
		return SRenderer::Get().Init();
	}
	void SRenderSystem::Update() {
		auto renderEntities = m_owner->Filter([](const SEntity& e) {return e.HasComponent<SRenderRes>();});
		auto lightEntities = m_owner->Filter([](const SEntity& e) {return e.HasComponent<SDirLight>();});
		for (const auto& entity : renderEntities) {
			const auto& renderRes = entity->GetComponent<SRenderRes>();
			SRenderCommand command;
			command.mesh = const_cast<SMesh*>(renderRes->GetMesh());
			command.material = const_cast<SMaterial*>(renderRes->GetMaterial());
			command.transform = entity->GetComponent<STransform>()->GetWorldMatrix();
			SRenderer::Get().Submit(command);
		}
		SCamInfo cinfo;
		const auto& cam = findMainCam();
		if (cam)
		{
			cinfo.projection = cam->GetProjectMat();
			cinfo.view = cam->GetViewMat();
			cinfo.pos = cam->GetOwner()->GetComponent<STransform>()->position;
			cinfo.fov = cam->foSize;
			cinfo.nearPlane = cam->nearPlane;
			cinfo.farPlane = cam->farPlane;
		}
		SLightInfo linfo;
		for (const auto& ent:lightEntities)
		{
			if(ent->HasComponent<SDirLight>())
			{
				linfo.dirLights.push_back(ent->GetComponent<SDirLight>());
			}
		}
		// 场景只输出到 Viewport(FBO):主窗口不画场景,只清屏避免残留上一帧像素。
		// Sandbox 没设 viewport,DrawTo(nullptr) 会回退到 default framebuffer,行为不变。
		SRenderer::Get().ClearDefaultFrameBuffer();
		SRenderer::Get().DrawTo(m_viewport, cinfo, linfo);
	}
	void SRenderSystem::End() {
		SRenderer::Get().Shutdown();
	}

	SCamera* SRenderSystem::findMainCam() {
		auto camEntities = m_owner->Filter([](const SEntity& e) {return e.HasComponent<SCamera>();});
		for (const auto& entity : camEntities) {
			const auto& cam = entity->GetComponent<SCamera>();
			if (cam && cam->is_main) {
				return cam;
			}
		}
		return nullptr;
	}
}
