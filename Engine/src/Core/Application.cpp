#include "spch.h"
#include "Application.h"

namespace Six {
	SApplication::~SApplication() {
		Shutdown();
	}
	bool SApplication::OnInit() {
		bool ok = true;
		// 注:timer 不再是 SApplication 的成员,它的 Init/Update 由 entrypoint 负责
		// (见 Core/EntryPoint.h 的 RunApplication)。
		// m_activeScene 可能还没设置(派生类忘了调 SetActiveScene),
		// 直接 m_activeScene->Init() 会空指针解引用
		if (m_activeScene) {
			ok &= m_activeScene->Init();
		}
		return ok;
	}
	void SApplication::OnShutdown() {
		if (m_activeScene) {
			m_activeScene->End();
		}
	}
	void SApplication::OnUpdate() {
		if (m_activeScene) {
			m_activeScene->Update();
		}
	}
	bool SApplication::ShouldClose() {
		return !m_running;
	}
	void SApplication::Shutdown() {
		m_running = false;
	}
	void SApplication::SetActiveScene(const std::string& name) {
		auto it = std::find_if(
			m_roots.begin(),
			m_roots.end(),
			[name](SEntity* e) {
				return e->GetType() == SEntityType::Scene && e->GetName() == name;
			}
		);
		if (it == m_roots.end())
		{
			auto sc = SEntity::Create(name, SEntityType::Scene);
			m_roots.push_back(sc);
			m_activeScene = sc;
			return;
		}

		// 只负责"选场景",不负责 Init。
		// Init 统一由 SApplication::OnInit() 触发,否则场景会被 Init 两次
		// (这里一次 + OnInit 一次)。
		m_activeScene = *it;
	}
	SEntity* SApplication::GetActiveScene() {
		return m_activeScene;
	}
	void SApplication::AddSceneToRoot(SEntity* s) {
		m_roots.push_back(s);
	}
	SEntity* SApplication::GetScene(std::function<bool(SEntity*)> predicate) {
		auto it = std::find_if(
			m_roots.begin(),
			m_roots.end(),
			[predicate](SEntity* e) {
				return e->GetType() == SEntityType::Scene && predicate(e);
			}
		);
		if (it != m_roots.end())
		{
			return (*it);
		}
	}
	SEntity* SApplication::GetScene(const std::string& name) {
		auto it = std::find_if(
			m_roots.begin(),
			m_roots.end(),
			[name](SEntity* e) {
				return e->GetType() == SEntityType::Scene && e->GetName() == name;
			}
		);
		if (it != m_roots.end())
		{
			return (*it);
		}
	}
	std::vector<SEntity*> SApplication::GetScenes()
	{
		return m_roots;
	}
	void SApplication::DeleteScene(SEntity* s) {
		auto it = std::find(
			m_roots.begin(),
			m_roots.end(),
			s
		);
		if (it == m_roots.end())
		{
			m_roots.erase(it);
		}
	}
}
