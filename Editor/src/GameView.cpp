#include "GameView.h"

namespace SixEditor {
	void SEGameView::Render() {
		if (m_open) {
			if (!m_open || !m_viewport) return;
			if(ImGui::Begin("Game View", &m_open)) {
				const ImVec2 avail = ImGui::GetContentRegionAvail();
				if (avail.x >= 1.0f && avail.y >= 1.0f) {
					m_gameViewSize = ImVec2((float)(int)avail.x, (float)(int)avail.y);
				}
				const unsigned int texId = m_viewport->GetColorTextureID();
				if (texId != 0) {
					ImGui::Image((ImTextureID)texId, avail, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				}
			}
			ImGui::End();
		}
	}
	Six::SViewport* SEGameView::GetViewport() {
		return m_viewport;
	}
	void SEGameView::SetViewport(Six::SViewport* v) {
		m_viewport = v;
		if (!m_viewport || m_gameViewSize.x <= 0.f || m_gameViewSize.y <= 0.f) return;
		m_viewport->ResizeViewport((int)m_gameViewSize.x, (int)m_gameViewSize.y);
	}

	ImVec2 SEGameView::GetSize() {
		return m_gameViewSize;
	}
	void SEGameView::SetSize(ImVec2 size) {
		m_gameViewSize = size;
	}
}