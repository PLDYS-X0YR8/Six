#pragma once
#include "PanelBase.h"
#include <imgui.h>
#include <SixEngine.h>
#include "EditorContext.h"

namespace SixEditor {
	class SEInspector :public SEPanel {
	public:
		void Render() override;
		const char* GetTitle() override { return "Inspector"; }
	};
}