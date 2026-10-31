#pragma once
#include "PanelBase.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <SixEngine.h>
#include "Log.h"

namespace SixEditor {
	class SEProjSetting :public SEPanel {
	public:
		void Render() override;
		const char* GetTitle() override {
			return "Project Setting";
		}
	};
}