#pragma once
#include "../Assets/Json.h"

namespace Six {
	class SIXAPI SProjConfig {
	public:
		// 读一个 *.sixconf.json。成功返回 true
		bool Load(const SPath& path);
		const Json& GetJson() const { return m_js; }

		// ★ 自己来自哪个文件(Save 时要按来源文件分组写回)
		const SPath& GetPath() const { return m_path; }

		// ★ 配置集名("config-name")
		//   只是元信息/分组名: UI 上用来分组显示,
		//   引擎查配置项时【不】用它 —— 直接在所有配置集的 items 里找。
		const std::string& GetConfigName() const { return m_configName; }
	private:
		// ★ 必须是值,不能用指针:
		//   SLoadJS 按值返回 Json(临时对象),对它取地址得到的是函数返回后就失效的悬垂指针
		Json m_js;
		SPath m_path;
		std::string m_configName;
	};
}
