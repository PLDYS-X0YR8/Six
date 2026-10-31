#include "spch.h"
#include "AppContext.h"
#include "Application.h"
#include "Timer.h"

#include "Project/Project.h"      // ★ 只有这一个 TU 会编 json.hpp
#include "Assets/FileSystem.h"    // SGetProjectFile()

namespace Six {
	// 匿名 namespace:存储只存在于 Engine.dll 内部,外部只能走 SAppContext 的导出函数
	namespace {
		SApplication* s_app = nullptr;
		STimer* s_timer = nullptr;
		std::unique_ptr<SProject> s_project;   // 项目配置: 引擎内单副本
		bool s_projectTried = false;            // 只尝试发现一次, 免得每次访问都刷屏
	}

	SApplication* SAppContext::App() {
		return s_app;
	}
	void SAppContext::SetApp(SApplication* app) {
		s_app = app;
	}
	STimer* SAppContext::Timer() {
		return s_timer;
	}
	void SAppContext::SetTimer(STimer* timer) {
		s_timer = timer;
	}
	SProject* SAppContext::Project() {
		if (!s_projectTried) {
			s_projectTried = true;
			const auto file = SGetProjectFile();
			if (file.ToString().empty()) {
				std::cerr << "[Project] *.sixproj.json not found" << std::endl;
			} else {
					// ★ 找不到文件也给一个空项目: 调用方不必到处判空, 用 IsLoaded() 区分
					s_project = std::make_unique<SProject>(file.ToString().empty() ? SPath() : file);
			}
		}
		return s_project.get();
	}
	bool SAppContext::LoadProject(const std::string& file) {
		auto proj = std::make_unique<SProject>(SPath(file));
		if (!proj->IsLoaded()) return false;
		s_project = std::move(proj);
		s_projectTried = true;
		return true;
	}
	void SAppContext::Reset() {
		s_app = nullptr;
		s_timer = nullptr;
		s_project.reset();
		s_projectTried = false;
	}
}
