#pragma once
#include "spch.h"

namespace Six {
	class SEntity;  // forward declare 避免循环
	/**
	 * @brief ECS 系统基类
	 *
	 * 由 SScene 统一管理，在 Init() / Update() / End() 中被调用。
	 * 每个系统代表一种逻辑（渲染、物理、输入等），不持有实体数据。
	 *
	 * @note 派生类必须实现 Init / Update / End / toStr / GetTypeId
	 *
	 * @warning 类型 ID 基于静态计数器，跨 DLL 可能不一致
	 */
	class SIXAPI SSystem {
	public:
		virtual size_t GetTypeId() const = 0;
		template <typename T>
		static size_t StaticTypeId() {
			static size_t id = m_nextid++;
			return id;
		}
		template <typename T>
		bool is() const { return GetTypeId() == T::GetStaticTypeId(); }
		SSystem();
		virtual ~SSystem();
		SSystem(const SSystem&) = delete;
		SSystem& operator=(const SSystem&) = delete;
		SSystem(SSystem&&) = default;
		SSystem& operator=(SSystem&&) = default;
		virtual bool Init() = 0;
		virtual void Update() = 0;
		virtual void End() = 0;
		virtual bool IsEnable() const;
		virtual void SetEnable(bool enable);
		virtual std::string toStr() = 0;
	protected:
		SEntity* m_owner = nullptr;
		bool m_enable = true;
		friend class SEntity;
	private:
		static size_t m_nextid;
	};
}
#define SIX_SYSTEM(cc) \
	public: \
		static size_t GetStaticTypeId() { \
			return StaticTypeId<cc>(); \
		} \
		virtual size_t GetTypeId() const override { return GetStaticTypeId(); }