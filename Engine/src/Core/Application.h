#pragma once
#include "spch.h"
#include "Timer.h"
#include "Scene/Entity.h"

namespace Six {
	class SIXAPI SApplication {
	public:
		virtual ~SApplication();
		virtual void OnUpdate();
		virtual bool OnInit();
		virtual void OnShutdown();
		virtual bool ShouldClose();
		virtual void Shutdown();
		// 注:原先的 static SApplication* Get() / SetInstance() / 成员 STimer m_timer 已移除。
		//     app 与 timer 现在由 entrypoint 创建,并通过 Six::SGetApp() / Six::SGetTimer() 取用
		//     (见 Core/AppContext.h)。
		virtual void SetActiveScene(const std::string& name);
		virtual SEntity* GetActiveScene();
		virtual void AddSceneToRoot(SEntity* s);
		virtual SEntity* GetScene(std::function<bool(SEntity*)> predicate);
		virtual SEntity* GetScene(const std::string& name);
		virtual std::vector<SEntity*> GetScenes();
		virtual void DeleteScene(SEntity* s);
	protected:
		bool m_running = true;
		bool m_pause = false;
		std::vector<SEntity*> m_roots;
		SEntity* m_activeScene = nullptr;
	};

	SApplication* CreateApplication();
}
