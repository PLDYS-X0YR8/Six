#include "spch.h"
#include "Program.h"

namespace Six {
	SProgram::SProgram() = default;
	SProgram::~SProgram() = default;
	bool SProgram::IsEnable() const { return m_enable; }
	void SProgram::SetEnable(bool enable) { m_enable = enable; }
	size_t SProgram::m_nextid = 0;
}
