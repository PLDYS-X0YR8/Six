#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "SixEngine.h"
#include "PanelBase.h"

namespace SixEditor {
	class SEGameView :public SEPanel{
	public:
		Six::SViewport* GetViewport();
		void SetViewport(Six::SViewport* v);
		ImVec2 GetSize();
		void SetSize(ImVec2 size);
		void Render() override;
		const char* GetTitle() override { return "Game View"; }
	private:
		Six::SViewport* m_viewport = nullptr;
		ImVec2 m_gameViewSize = ImVec2(0.0f, 0.0f);
	};
}