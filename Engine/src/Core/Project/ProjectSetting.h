#pragma once
#include "spch.h"
#include "../Assets/FileSystem.h"

namespace Six {
	// ★ 前向声明即可: 想直接读某个 conf 的原始 json 才 include ProjectConf.h
	//   (那个头会拖进 json.hpp)
	class SIXAPI SProjConfig;

	// 项目设置 = 一个 *.sixproj.json 的内容
	//
	// ★ 这个头刻意 **不** include Json.h / ProjectConf.h:
	//   那两个会拖进 953KB 的 json.hpp, 每个 TU 多编 2~4 秒。
	//   json 相关的东西全部关在 Impl 里(定义在 ProjectSetting.cpp),
	//   所以 Sandbox / Editor 可以直接 include 本头用 SProject / SProjSetting。
	class SIXAPI SProjSetting {
	public:
		// ★ 必须显式删掉拷贝(同 SEntity / SEventQueue 的写法):
		//   成员里有 unique_ptr,
		//   而类是 SIXAPI(dllexport) 时 MSVC 会强制实例化隐式拷贝赋值
		//   -> 触发已删除的 unique_ptr::operator= -> error C2280
		SProjSetting();
		// ★ 析构必须定义在 .cpp: Impl 是不完整类型,
		//   unique_ptr<Impl> 的析构需要完整类型才能实例化
		~SProjSetting();
		SProjSetting(const SProjSetting&) = delete;
		SProjSetting& operator=(const SProjSetting&) = delete;

		// 读一个 *.sixproj.json。成功返回 true
		bool Load(const SPath& path);

		// ---- 结构字段(来自 *.sixproj.json 本身) ----
		const std::string& GetName() const;
		// "Entry Point": 入口场景名
		const std::string& GetEntryPoint() const;
		// "Assets Path": 项目资源目录
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
		// 返回是否全部写成功
		bool Save() const;
		// ---- 配置集 ----
		// 所有配置集的 "config-name"(只是分组用的元信息, 查配置项时不用它)
		std::vector<std::string> GetConfigSetNames() const;
		// 顶层元信息(engine_version / config-name 等), 它们不在 items 里
		std::string GetMeta(const std::string& key, const std::string& fallback = "") const;
		size_t GetConfigCount() const;
		const SProjConfig* GetConfigJson(size_t index) const;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}
