#pragma once
#include <SixEngine.h>

namespace SixEditor {
	class SEContext {
	public:
		static SEContext& Get() {
			static SEContext con;
			return con;
		}
		Six::SEntity* selected = nullptr;
	private:
		SEContext() = default;
		~SEContext() = default;
		// 禁用拷贝
		SEContext(const SEContext&) = delete;
		SEContext& operator=(const SEContext&) = delete;
	};
}