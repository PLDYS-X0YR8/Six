#pragma once
#include "sixcore.h"
#include "../System.h"
#include "Core/Render/Renderer.h"
#include "RenderRes.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

namespace Six {
	class SIXAPI SRenderSystem : public SSystem {
		SIX_SYSTEM(SRenderSystem)
	public:
		SRenderSystem() = default;
		virtual ~SRenderSystem() = default;
		SRenderSystem(const SRenderSystem&) = delete;
		SRenderSystem& operator=(const SRenderSystem&) = delete;
		SRenderSystem(SRenderSystem&&) = default;
		SRenderSystem& operator=(SRenderSystem&&) = default;
		virtual bool Init() override;
		virtual void Update() override;
		virtual void End() override;
		virtual std::string toStr() override {
			return "SRenderSystem";
		}
		SViewport* GetViewport() { return m_viewport; }
		SViewport* GetViewport() const { return m_viewport; }
		void SetViewport(SViewport* vp) { m_viewport = vp; }
	private:
		SCamera* findMainCam();
		SViewport* m_viewport = nullptr;
	};
}