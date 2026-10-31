#include "spch.h"
#include "System.h"

namespace Six {
	SSystem::SSystem() = default;
	SSystem::~SSystem() = default;
	bool SSystem::IsEnable() const { return m_enable; }
	void SSystem::SetEnable(bool enable) { m_enable = enable; }
	size_t SSystem::m_nextid = 0;
}
