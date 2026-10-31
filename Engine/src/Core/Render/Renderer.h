#pragma once
#include "spch.h"
#include "Window.h"
#include "graphicObj/Material.h"
#include "graphicObj/Viewport.h"
#include "Mesh.h"
#include "RenderAPI.h"
#include "Core/Scene/ECS/Light.h"
#include <glm/glm.hpp>

namespace Six {
	struct SIXAPI SRenderCommand {
		SMesh* mesh = nullptr;
		SMaterial* material = nullptr;
		glm::mat4 transform = glm::mat4(1.f);
	};
	struct SIXAPI SLightInfo {
		std::vector<SDirLight*> dirLights;
	};
	struct SIXAPI SCamInfo {
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec3 pos;
		// 用于按 render target 尺寸重算 projection(aspect 修正)
		float fov = 60.f;
		float nearPlane = 0.1f;
		float farPlane = 1000.f;
	};
	class SIXAPI SRenderer {
	public:
		static SRenderer& Get() {
			static SRenderer r;
			return r;
		}
		void SetWindow(int width,int height,std::string title);
		void SetWindow(SWindow* w);
		inline SWindow* GetWindow() {
			return m_window.get();
		}
		bool Init();
		void Draw(SCamInfo& cam,SLightInfo& light);
		void DrawTo(SViewport* vp, SCamInfo& cam, SLightInfo& light);
		// 只清主窗口 default framebuffer,不画任何场景。
		// 场景只输出到 Viewport(FBO)时,主窗口需要清干净避免残留上一帧像素
		void ClearDefaultFrameBuffer();
		void Submit(SRenderCommand& command);
		void Shutdown();
	private:
		std::unique_ptr<SWindow> m_window;
		std::unique_ptr<SRenderAPI> m_renderApi;
		std::vector<SRenderCommand> m_commands;
	};
}