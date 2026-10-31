#pragma once
#include "../System.h"

namespace Six {
	class SIXAPI SProcessClose : public SSystem {
		SIX_SYSTEM(SProcessClose)
	public:
		bool Init() override;
		void Update() override;
		void End() override;
		std::string toStr() {
			return "Process Close";
		}
	};
}