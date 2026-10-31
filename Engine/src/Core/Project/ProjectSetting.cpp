#include "spch.h"
#include "ProjectSetting.h"
#include "ProjectConf.h"
#include <algorithm>
#include <iostream>

namespace Six {
	// ★ json 相关的成员全部关在这里: 头文件看不到 Json / SProjConfig 的定义
	struct SProjSetting::Impl {
		std::string name;
		std::string entryPoint;
		SPath assetsPath;
		std::vector<std::unique_ptr<SProjConfig>> configs;

		// ★ 可编辑物化缓存(P 版本用): key -> 值
		//   第一次被 P 版本访问时从 json 取值填进来, 之后指针稳定。
		std::map<std::string, std::string> strSlots;
		std::map<std::string, int>         intSlots;
		std::map<std::string, bool>        boolSlots;

		// ★ key -> 它来自哪个 conf 文件(绝对路径)
		//   Save() 靠它把改动写回正确的文件
		std::map<std::string, std::string> keyOrigin;

		// ★ proj 文件自身的路径(Save 时写回)
		std::string projFile;
	};

	namespace {
		// 在所有 conf 文件里按 key 查找, 第一个命中即返回
		// origin(可选): 命中的那个文件的绝对路径
		template <typename T>
		bool LookupInConfigs(const std::vector<std::unique_ptr<SProjConfig>>& configs,
			const std::string& key, T& out, std::string* origin = nullptr) {
			for (const auto& conf : configs) {
				if (!conf) continue;
				const Json& js = conf->GetJson();
				if (!js.is_object()) continue;
				// ★ 新格式: 配置项在 "items" 子对象里
				//   (顶层是元信息: engine_version / config-name)
				auto items = js.find("items");
				if (items == js.end() || !items->is_object()) continue;
				auto it = items->find(key);
				if (it == items->end()) continue;
				try {
					out = it->get<T>();
				}
				catch (const Json::exception&) {
					continue;   // 类型对不上 -> 当没这个 key
				}
				if (origin) *origin = conf->GetPath().ToString();
				return true;
			}
			return false;
		}
	}

	SProjSetting::SProjSetting() : m_impl(std::make_unique<Impl>()) {
		m_impl->assetsPath = SPath("./assets");
	}
	SProjSetting::~SProjSetting() = default;

	bool SProjSetting::Load(const SPath& path) {
		// 值语义:不用 &SLoadJS(...)(那是对临时对象取地址)
		Json js;
		if (!SLoadJS(path, js)) {
			return false;
		}

		m_impl->projFile = path.ToString();
		m_impl->name = js.value("Project Name", std::string());
		m_impl->entryPoint = js.value("Entry Point", std::string());
		m_impl->assetsPath = SPath(js.value("Assets Path", std::string("./assets")));

		m_impl->configs.clear();
		// ★ 配置目录: "Config Path": "./conf"
		//   扫描该目录下所有 *.sixconf.json。
		//   ★ directory_iterator 的遍历顺序不保证 -> 必须排序,
		//     否则"同名 key 先命中哪个配置集"会随机漂。
		const std::string confDir = js.value("Config Path", std::string("./conf"));
		const auto dir = SPath(confDir).ToPath();
		std::error_code ec;
		std::vector<std::filesystem::path> confFiles;
		for (auto it = std::filesystem::directory_iterator(dir, ec);
			 !ec && it != std::filesystem::directory_iterator();
			 it.increment(ec)) {
			if (it->path().filename().string().ends_with(".sixconf.json"))
				confFiles.push_back(it->path());
		}
		std::sort(confFiles.begin(), confFiles.end());
		if (confFiles.empty()) {
			std::cerr << "[ProjectSetting] 配置目录里没有 *.sixconf.json: " << dir.string() << std::endl;
		}
		for (const auto& f : confFiles) {
			auto conf = std::make_unique<SProjConfig>();
			if (!conf->Load(SPath(f))) {
				std::cerr << "[ProjectSetting] 配置文件加载失败: " << f.string() << std::endl;
				continue;
			}
			m_impl->configs.push_back(std::move(conf));
		}

		// 重新加载 -> 之前的物化缓存作废
		m_impl->strSlots.clear();
		m_impl->intSlots.clear();
		m_impl->boolSlots.clear();
		m_impl->keyOrigin.clear();
		return true;
	}

	// ---------------- 只读 ----------------
	const std::string& SProjSetting::GetName() const {
		return m_impl->name;
	}
	const std::string& SProjSetting::GetEntryPoint() const {
		return m_impl->entryPoint;
	}
	SPath SProjSetting::GetAssetsPath() const {
		return m_impl->assetsPath;
	}

	std::string SProjSetting::GetConfigStr(const std::string& key, const std::string& fallback) const {
		// ★ 已被 P 版本物化过 -> 返回内存里的值(可能已被改过)
		auto it = m_impl->strSlots.find(key);
		if (it != m_impl->strSlots.end()) return it->second;
		std::string v;
		if (LookupInConfigs(m_impl->configs, key, v)) return v;
		return fallback;
	}
	int SProjSetting::GetConfigInt(const std::string& key, int fallback) const {
		auto it = m_impl->intSlots.find(key);
		if (it != m_impl->intSlots.end()) return it->second;
		int v = 0;
		if (LookupInConfigs(m_impl->configs, key, v)) return v;
		return fallback;
	}
	bool SProjSetting::GetConfigBool(const std::string& key, bool fallback) const {
		auto it = m_impl->boolSlots.find(key);
		if (it != m_impl->boolSlots.end()) return it->second;
		bool v = false;
		if (LookupInConfigs(m_impl->configs, key, v)) return v;
		return fallback;
	}

	// ---------------- 可写(P 版本) ----------------
	std::string* SProjSetting::GetNameP() const {
		return &m_impl->name;
	}
	std::string* SProjSetting::GetEntryPointP() const {
		return &m_impl->entryPoint;
	}
	SPath* SProjSetting::GetAssetsPathP() const {
		return &m_impl->assetsPath;
	}

	std::string* SProjSetting::GetConfigStrP(const std::string& key, const std::string& fallback) const {
		auto it = m_impl->strSlots.find(key);
		if (it != m_impl->strSlots.end()) return &it->second;

		std::string v = fallback;
		std::string origin;
		if (LookupInConfigs(m_impl->configs, key, v, &origin)) {
			m_impl->keyOrigin[key] = origin;
		}
		auto res = m_impl->strSlots.emplace(key, std::move(v));
		return &res.first->second;
	}
	int* SProjSetting::GetConfigIntP(const std::string& key, int fallback) const {
		auto it = m_impl->intSlots.find(key);
		if (it != m_impl->intSlots.end()) return &it->second;

		int v = fallback;
		std::string origin;
		if (LookupInConfigs(m_impl->configs, key, v, &origin)) {
			m_impl->keyOrigin[key] = origin;
		}
		auto res = m_impl->intSlots.emplace(key, v);
		return &res.first->second;
	}
	bool* SProjSetting::GetConfigBoolP(const std::string& key, bool fallback) const {
		auto it = m_impl->boolSlots.find(key);
		if (it != m_impl->boolSlots.end()) return &it->second;

		bool v = fallback;
		std::string origin;
		if (LookupInConfigs(m_impl->configs, key, v, &origin)) {
			m_impl->keyOrigin[key] = origin;
		}
		auto res = m_impl->boolSlots.emplace(key, v);
		return &res.first->second;
	}

	// ---------------- 写回磁盘 ----------------
	bool SProjSetting::Save() const {
		bool ok = true;

		// ---- 1. proj 文件自身: 结构字段 ----
		if (!m_impl->projFile.empty()) {
			Json js;
			if (!SLoadJS(SPath(m_impl->projFile), js) || !js.is_object()) {
				std::cerr << "[ProjectSetting] 保存失败(读): " << m_impl->projFile << std::endl;
				ok = false;
			} else {
				js["Project Name"] = m_impl->name;
				js["Entry Point"] = m_impl->entryPoint;
				js["Assets Path"] = m_impl->assetsPath.path_str;
				if (!SDumpJS(SPath(m_impl->projFile), js)) {
					std::cerr << "[ProjectSetting] 保存失败(写): " << m_impl->projFile << std::endl;
					ok = false;
				}
			}
		}

		// ---- 2. conf 文件: 按 key 的来源文件分组 ----
		std::map<std::string, std::map<std::string, Json>> changes;
		auto collect = [&](const std::string& key, const Json& v) {
			auto it = m_impl->keyOrigin.find(key);
			if (it != m_impl->keyOrigin.end()) {
				changes[it->second][key] = v;
				return;
			}
			// 新增的 key(原来不存在) -> 归到第一个 conf 文件
			if (!m_impl->configs.empty() && m_impl->configs.front()) {
				changes[m_impl->configs.front()->GetPath().ToString()][key] = v;
			}
		};
		for (const auto& kv : m_impl->strSlots) collect(kv.first, Json(kv.second));
		for (const auto& kv : m_impl->intSlots)  collect(kv.first, Json(kv.second));
		for (const auto& kv : m_impl->boolSlots) collect(kv.first, Json(kv.second));

		for (const auto& fc : changes) {
			const std::string& file = fc.first;
			Json js;
			if (!SLoadJS(SPath(file), js) || !js.is_object()) {
				std::cerr << "[ProjectSetting] 保存失败(读): " << file << std::endl;
				ok = false;
				continue;
			}
			// ★ 新格式: 值写进 items 子对象
			//   只覆盖被访问过的 key, 文件里其他键原样保留
			if (!js.contains("items") || !js["items"].is_object()) js["items"] = Json::object();
			for (const auto& kv : fc.second) js["items"][kv.first] = kv.second;
			if (!SDumpJS(SPath(file), js)) {
				std::cerr << "[ProjectSetting] 保存失败(写): " << file << std::endl;
				ok = false;
			}
		}
		return ok;
	}

	// ---------------- 配置集 ----------------
	std::vector<std::string> SProjSetting::GetConfigSetNames() const {
		std::vector<std::string> names;
		for (const auto& conf : m_impl->configs) {
			if (conf) names.push_back(conf->GetConfigName());
		}
		return names;
	}
	std::string SProjSetting::GetMeta(const std::string& key, const std::string& fallback) const {
		// 顶层元信息(engine_version / config-name 等), 不在 items 里
		for (const auto& conf : m_impl->configs) {
			if (!conf) continue;
			const Json& js = conf->GetJson();
			if (!js.is_object()) continue;
			auto it = js.find(key);
			if (it == js.end()) continue;
			try {
				return it->get<std::string>();
			}
			catch (const Json::exception&) {
				continue;
			}
		}
		return fallback;
	}

	// ---------------- 原始 json 出口 ----------------
	size_t SProjSetting::GetConfigCount() const {
		return m_impl->configs.size();
	}
	const SProjConfig* SProjSetting::GetConfigJson(size_t index) const {
		if (index >= m_impl->configs.size()) return nullptr;
		return m_impl->configs[index].get();
	}
}
