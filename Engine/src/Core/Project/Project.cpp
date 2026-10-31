#include "spch.h"
#include "Project.h"

namespace Six {
	SProject::SProject(const SPath& path) {
		m_setting = std::make_unique<SProjSetting>();
		// ★ 不能丢掉返回值: 加载失败必须留下状态,否则调用方无从察觉
		m_loaded = m_setting->Load(path);
		if (!m_loaded && !path.path_str.empty()) {
			std::cerr << "[Project] 项目配置加载失败: " << path.ToString() << std::endl;
		}
	}

	const std::string& SProject::GetName() const {
		return m_setting->GetName();
	}
	const std::string& SProject::GetEntryPoint() const {
		return m_setting->GetEntryPoint();
	}
	SPath SProject::GetAssetsPath() const {
		return m_setting->GetAssetsPath();
	}
	std::string SProject::GetConfigStr(const std::string& key, const std::string& fallback) const {
		return m_setting->GetConfigStr(key, fallback);
	}
	int SProject::GetConfigInt(const std::string& key, int fallback) const {
		return m_setting->GetConfigInt(key, fallback);
	}
	bool SProject::GetConfigBool(const std::string& key, bool fallback) const {
		return m_setting->GetConfigBool(key, fallback);
	}

	std::string* SProject::GetNameP() const {
		return m_setting->GetNameP();
	}
	std::string* SProject::GetEntryPointP() const {
		return m_setting->GetEntryPointP();
	}
	SPath* SProject::GetAssetsPathP() const {
		return m_setting->GetAssetsPathP();
	}
	std::string* SProject::GetConfigStrP(const std::string& key, const std::string& fallback) const {
		return m_setting->GetConfigStrP(key, fallback);
	}
	int* SProject::GetConfigIntP(const std::string& key, int fallback) const {
		return m_setting->GetConfigIntP(key, fallback);
	}
	bool* SProject::GetConfigBoolP(const std::string& key, bool fallback) const {
		return m_setting->GetConfigBoolP(key, fallback);
	}

	std::vector<std::string> SProject::GetConfigSetNames() const {
		return m_setting->GetConfigSetNames();
	}
	std::string SProject::GetMeta(const std::string& key, const std::string& fallback) const {
		return m_setting->GetMeta(key, fallback);
	}
	bool SProject::Save() const {
		return m_setting->Save();
	}
}
