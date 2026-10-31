#pragma once
#include "PanelBase.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <SixEngine.h>
#include "EditorContext.h"

namespace SixEditor {
	class SEHierarchy :public SEPanel {
	public:
		void Render() override;
		const char* GetTitle() override { return "Hierarchy"; }
	private:
		void DrawEntityTree(Six::SEntity* scn,Six::SEntity* ent);
	};
}