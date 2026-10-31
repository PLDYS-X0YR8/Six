#pragma once
#include "spch.h"

namespace Six {
	class SApplication;
	class STimer;
	// ★ 前向声明即可: 这里 include Project.h 会把 953KB 的 json.hpp 拖进每个 TU
	class SProject;

	// 全局上下文:取代原先 SApplication 内部的静态单例(s_instance / Get / SetInstance)。
	//
	// 设计要点:
	//  - 存储只有一份:静态成员变量定义在 Engine.dll 的 AppContext.cpp(匿名 namespace),
	//    外部只能通过导出的静态函数访问 -> 不存在 Engine.dll 和 exe 各存一份的问题。
	//  - 生命周期由 entrypoint 负责:创建 app/timer -> SetApp/SetTimer -> 主循环 -> Reset。
	//  - Engine 内部(ProcessClose 等)通过 Six::SGetApp() 拿到宿主,不再直接依赖 SApplication 静态成员。
	class SIXAPI SAppContext {
	public:
		static SApplication* App();
		static void SetApp(SApplication* app);

		static STimer* Timer();
		static void SetTimer(STimer* timer);

		// ★ 项目配置(懒加载): 第一次访问时自动从 <exe名>.sixproj.json 建立
		//   存储同样只在 Engine.dll 内有一份(见 AppContext.cpp 的匿名 namespace)
		static SProject* Project();
		// 显式加载指定项目文件(覆盖懒加载结果); 成功返回 true
		static bool LoadProject(const std::string& file);

		static void Reset();
	};

	// 便捷 getter(header 内 inline,无重复符号问题)
	inline SApplication* SGetApp() { return SAppContext::App(); }
	inline STimer* SGetTimer() { return SAppContext::Timer(); }
	inline SProject* SGetProject() { return SAppContext::Project(); }
}
