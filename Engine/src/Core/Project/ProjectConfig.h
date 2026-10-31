#pragma once
#include "spch.h"

namespace Six {
	// ★ 便捷全局入口(纯薄转发, 真正的实现在 ProjectSetting 里)
	//
	//   不想每次都写 Six::SGetProject()->GetConfigInt(...) 的时候用这些。
	//   要更细的控制(拿 SProjSetting* / 原始 json)就直接:
	//     #include "Core/Project/Project.h"
	//     Six::SGetProject()->GetSetting()->...
	//
	// ★ 这个头不拉 json.hpp ✅
	//   真正的项目对象入口是 Six::SGetProject()(在 AppContext.h),
	//   它**永不返回 null**: 找不到项目文件时给一个空项目,
	//   用 IsLoaded() 区分"没配置"和"配置是空的"。
	SIXAPI bool HasProjectConfig();

	// 从 conf 文件里按 key 取值; 取不到 / 类型不符就用 fallback
	SIXAPI std::string GetConfigStr(const std::string& key, const std::string& fallback = "");
	SIXAPI int         GetConfigInt(const std::string& key, int fallback = 0);
	SIXAPI bool        GetConfigBool(const std::string& key, bool fallback = false);

	// 把内存里的改动写回磁盘(proj 文件 + 被访问过的 conf 键)
	SIXAPI bool SaveProjectConfig();
}
