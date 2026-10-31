#pragma once
#include "ProjectSetting.h"

namespace Six {
	// 项目 = 一个 *.sixproj.json + 它引用的配置
	//
	// ★ 这个头不拉 json.hpp (ProjectSetting.h 已经把 json 关进 .cpp 了),
	//   所以 Sandbox / Editor 可以直接 include 它。
	class SIXAPI SProject {
	public:
		// ★ 构造函数必须 public(class 默认 private,之前写成 private 导致这个类无法实例化)
		//   路径可以是 *.sixproj.json;相对路径按项目根解析
		explicit SProject(const SPath& path);
		~SProject() = default;

		// ★ 同 ProjectSetting.h: 成员是 unique_ptr, 必须显式删拷贝,
		//   否则 dllexport 类会被 MSVC 强制实例化隐式拷贝赋值 -> C2280
		SProject(const SProject&) = delete;
		SProject& operator=(const SProject&) = delete;

		// ★ 配置是否加载成功。构造函数里 Load() 的返回值以前被丢弃,
		//   配置文件写错会静默失败 -> 后面所有参数悄悄走默认值。
		bool IsLoaded() const { return m_loaded; }

		SProjSetting* GetSetting() { return m_setting.get(); }
		const SProjSetting* GetSetting() const { return m_setting.get(); }

		// ---- 便捷转发: 不想每次都写 GetSetting()-> 时用这些 ----
		const std::string& GetName() const;
		const std::string& GetEntryPoint() const;
		SPath GetAssetsPath() const;
		std::string GetConfigStr(const std::string& key, const std::string& fallback = "") const;
		int         GetConfigInt(const std::string& key, int fallback = 0) const;
		bool        GetConfigBool(const std::string& key, bool fallback = false) const;
		std::string* GetNameP() const;
		std::string* GetEntryPointP() const;
		SPath*        GetAssetsPathP() const;
		std::string* GetConfigStrP(const std::string& key, const std::string& fallback = "") const;
		int*         GetConfigIntP(const std::string& key, int fallback = 0) const;
		bool*        GetConfigBoolP(const std::string& key, bool fallback = false) const;

		std::vector<std::string> GetConfigSetNames() const;
		std::string GetMeta(const std::string& key, const std::string& fallback = "") const;

		bool Save() const;

	private:
		// ★ unique_ptr: 之前的裸指针没初始化 -> GetSetting() 可能返回垃圾地址
		std::unique_ptr<SProjSetting> m_setting;

		bool m_loaded = false;
	};
}
