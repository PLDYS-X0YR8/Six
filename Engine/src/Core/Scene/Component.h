#pragma once
#include "spch.h"

namespace Six {
    // 单例 typeid registry 在 Engine.dll 里 — 跨 DLL 调用拿到一致 id
    // key 用 typeid(T).name() 字符串的 FNV-1a hash (跨编译单元 100% 一致)
    SIXAPI size_t SRegisterType(size_t typeNameHash);

    // FNV-1a hash (64-bit) — 纯算法,跨编译单元/平台/DLL 都确定性
    inline size_t HashTypeName(const char* s) {
        constexpr size_t FNV_OFFSET = 14695981039346656037ULL;
        constexpr size_t FNV_PRIME  = 1099511628211ULL;
        size_t h = FNV_OFFSET;
        while (*s) {
            h ^= static_cast<size_t>(*s++);
            h *= FNV_PRIME;
        }
        return h;
    }

    class SEntity;  // 前置声明,打破 Entity.h ↔ Component.h 循环

	class SIXAPI SComponent {
	public:
		virtual ~SComponent() = default;
		virtual size_t GetTypeId() const = 0;
		template <typename T>
		static size_t StaticTypeId() {
			// FNV-1a(typeid(T).name()) 跨 translation unit / DLL 一致
			static const size_t id = SRegisterType(HashTypeName(typeid(T).name()));
			return id;
		}
		template <typename T>
		bool is() const { return GetTypeId() == T::GetStaticTypeId(); }
		virtual std::string toStr() = 0;
		virtual SEntity* GetOwner() const { return m_owner; }
		virtual SEntity* GetOwner() { return m_owner; }
	protected:
		SEntity* m_owner = nullptr;
		friend class SEntity;
	};
}
#define SIX_REQUARED_COMPONENT(T) \
	do{\
	if(!m_owner->GetComponent<T>()){\
		m_owner->CreateComponent<T>();\
	}\
}while(0);

#define SIX_COMPONENT(cc) \
	public: \
		static size_t GetStaticTypeId() { \
			return StaticTypeId<cc>(); \
		} \
		virtual size_t GetTypeId() const override { return GetStaticTypeId(); }