#pragma once
#include "spch.h"
#include "Application.h"
#include "AppContext.h"
#include "Timer.h"
#include "Event/Input.h"

namespace Six {
	// 由宿主(Editor / Sandbox)实现:返回该 exe 的 app 实例。
	// 注意这个符号定义在 exe 里(不是 Engine.dll 里),所以 Engine 的头文件只做声明。
	SApplication* CreateApplication();

	// entrypoint 公共流程:
	//   创建 app + timer -> 注入全局上下文 -> 主循环 -> 清理
	// timer 用栈局部变量,不用堆、不用 new,main 返回前清掉上下文指针。
	//
	// 这段是 inline 函数放头文件:每个 exe 各自实例化一份,不会产生重复符号。
	inline int RunApplication() {
#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
#endif
		SApplication* app = CreateApplication();
		STimer timer;
		timer.Init();

		SAppContext::SetApp(app);
		SAppContext::SetTimer(&timer);

		if (app->OnInit()) {
			while (!app->ShouldClose()) {
				SInput::Get().Update();
				timer.Update();
				app->OnUpdate();
				SInput::Get().EndFrame();
			}
		}

		app->OnShutdown();
		SAppContext::Reset();
		delete app;
		return 0;
	}
}
