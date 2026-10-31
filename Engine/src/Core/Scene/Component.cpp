#include "spch.h"
#include "Component.h"
#include <unordered_map>
#include <atomic>

namespace Six {
	// typeid registry 单例在 Engine.dll 里,跨 DLL 调用拿到一致 id
	// key 是 FNV-1a hash of typeid(T).name() — 跨编译单元/DLL 确定性
	static std::unordered_map<size_t, size_t>& TypeRegistry() {
		static std::unordered_map<size_t, size_t> reg;
		return reg;
	}
	static std::atomic<size_t>& NextTypeId() {
		static std::atomic<size_t> n{ 0 };
		return n;
	}

	size_t SRegisterType(size_t typeNameHash) {
		auto& reg = TypeRegistry();
		auto it = reg.find(typeNameHash);
		if (it == reg.end()) {
			it = reg.emplace(typeNameHash, NextTypeId().fetch_add(1)).first;
		}
		return it->second;
	}
}
