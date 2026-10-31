#pragma once
#include "spch.h"
namespace Six{
	class SEntity;
	class SIXAPI SProgram {
	public:
		virtual size_t GetTypeId() const = 0;
		template <typename T>
		static size_t StaticTypeId() {
			static size_t id = m_nextid++;
			return id;
		}
		template <typename T>
		bool is() const { return GetTypeId() == T::GetStaticTypeId(); }
		SProgram();
		virtual ~SProgram();
		SProgram(const SProgram&) = delete;
		SProgram& operator=(const SProgram&) = delete;
		SProgram(SProgram&&) = default;
		SProgram& operator=(SProgram&&) = default;
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

#define SIX_PROGRAM(cc) \
	public: \
		static size_t GetStaticTypeId() { \
			return StaticTypeId<cc>(); \
		} \
		virtual size_t GetTypeId() const override { return GetStaticTypeId(); }