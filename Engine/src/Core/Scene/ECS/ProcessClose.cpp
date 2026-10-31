#include "spch.h"
#include "ProcessClose.h"
#include "Core/Application.h"
#include "Core/AppContext.h"
#include "../Entity.h"

namespace Six {
	bool SProcessClose::Init(){
		return true;
	}

	void SProcessClose::Update() {
		SApplication* app = Six::SGetApp();
		if (app && app->ShouldClose()) {
			app->Shutdown();
		}
	}
	void SProcessClose::End() {
		// cleanup if needed; nothing right now
	}
}