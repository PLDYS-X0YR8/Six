#include "spch.h"
#include "ProjectConf.h"

namespace Six {
	bool SProjConfig::Load(const SPath& path) {
		// ★ 记住自己来自哪个文件: Save() 靠它把改动写回正确的文件
		m_path = path;
		// 直接写进成员,不走临时对象(见 ProjectConf.h 的说明)
		if (!SLoadJS(path, m_js)) {
			return false;
		}
		// 配置集名(只是分组用的元信息, 查 key 时不用它)
		m_configName = m_js.value("config-name", std::string());
		return true;
	}
}
