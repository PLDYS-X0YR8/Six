#include "Layout.h"

namespace SixEditor {

	const SEGUILayout* SELayout::Find(const std::string& name) const {
		auto it = m_presets.find(name);
		return it == m_presets.end() ? nullptr : &it->second;
	}

	void SELayout::AddPreset(const std::string& name, const SEGUILayout& lay) {
		if (name.empty()) return;
		SEGUILayout copy = lay;
		copy.name = name;          // 以 map key 为准,保证 Current().name 始终等于 preset 名
		m_presets[name] = copy;
		if (m_current.name == name) {
			m_current = copy;
			m_needRebuild = true;
		}
	}

	void SELayout::DelPreset(const std::string& name) {
		m_presets.erase(name);
		if (m_current.name == name) {
			m_current = m_presets.empty() ? SEGUILayout{} : m_presets.begin()->second;
			m_needRebuild = true;
		}
	}

	ImGuiDir SELayout::DM2IGD(SELayoutDockMode m) {
		switch (m) {
		case SELayoutDockMode::Up:    return ImGuiDir_Up;
		case SELayoutDockMode::Down:  return ImGuiDir_Down;
		case SELayoutDockMode::Left:  return ImGuiDir_Left;
		case SELayoutDockMode::Right: return ImGuiDir_Right;
		default:                      return ImGuiDir_None;   // Center / Float 不走切分
		}
	}

	// 把同一方向的一组元素塞进已经切好的 band。
	// items[i].ratio 是"占整个 dockspace"的绝对比例;bandTotal = 该方向所有 ratio 之和。
	// cur 每轮代表"还没分配的部分",所以 frac = r / (r + remain) 能还原成绝对比例。
	static void FillBand(ImGuiID band, ImGuiDir dir, const std::vector<const SEGUILayoutElement*>& items, float bandTotal) {
		ImGuiID cur = band;
		float remain = bandTotal;
		for (size_t i = 0; i < items.size(); ++i) {
			const float r = items[i]->ratio;
			remain -= r;
			ImGuiID node = cur;
			if (i + 1 < items.size()) {
				const float frac = (r + remain) > 0.0f ? r / (r + remain) : 1.0f;
				node = ImGui::DockBuilderSplitNode(cur, dir, frac, nullptr, &cur);
			}
			if (node != 0)
				ImGui::DockBuilderDockWindow(items[i]->title.c_str(), node);
		}
	}

	void SELayout::Apply(ImGuiID dockspaceId, const SEGUILayout& lay) {
		// 1) 拆掉旧节点树
		ImGui::DockBuilderRemoveNode(dockspaceId);
		// 2) 建根节点。第二个参数是 ImGuiDockNodeFlags(1.93 起不再是 parent_id),
		//    必须带 DockSpace flag,否则建出来的是浮动节点(见 imgui_internal.h:3939-3940)
		ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
		// 3) 必须先给尺寸,否则 DockBuilderSplitNode 里 IM_ASSERT(size_avail > 0)
		const ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::DockBuilderSetNodeSize(dockspaceId, vp->WorkSize);

		// 按方向分组 + 各方向总占比
		std::vector<const SEGUILayoutElement*> left, right, up, down, center;
		float totalLeft = 0.f, totalRight = 0.f, totalUp = 0.f, totalDown = 0.f;
		for (const auto& e : lay.elements) {
			switch (e.mode) {
			case SELayoutDockMode::Left:   left.push_back(&e);   totalLeft  += e.ratio; break;
			case SELayoutDockMode::Right:  right.push_back(&e);  totalRight += e.ratio; break;
			case SELayoutDockMode::Up:     up.push_back(&e);     totalUp    += e.ratio; break;
			case SELayoutDockMode::Down:   down.push_back(&e);   totalDown  += e.ratio; break;
			case SELayoutDockMode::Center: center.push_back(&e); break;
			case SELayoutDockMode::Float:
				// 浮动窗口不是 DockBuilder 的事(要 ImGuiWindowFlags_NoDock),这里跳过
				break;
			}
		}

		ImGuiID centerId = dockspaceId;

		// --- X 轴:先左后右 ---
		if (!left.empty() && totalLeft > 0.f) {
			ImGuiID band = 0;
			ImGui::DockBuilderSplitNode(centerId, ImGuiDir_Left, totalLeft, &band, &centerId);
			if (band) FillBand(band, ImGuiDir_Left, left, totalLeft);
		}
		if (!right.empty() && totalRight > 0.f) {
			// centerId 现在只剩 (1 - totalLeft) 宽,要还原成"占整个 dockspace 的绝对比例"
			const float avail = 1.0f - totalLeft;
			const float frac = avail > 0.0f ? totalRight / avail : 1.0f;
			ImGuiID band = 0;
			ImGui::DockBuilderSplitNode(centerId, ImGuiDir_Right, frac, &band, &centerId);
			if (band) FillBand(band, ImGuiDir_Right, right, totalRight);
		}

		// --- Y 轴:先上后下(此时 centerId 高度仍是 100%) ---
		if (!up.empty() && totalUp > 0.f) {
			ImGuiID band = 0;
			ImGui::DockBuilderSplitNode(centerId, ImGuiDir_Up, totalUp, &band, &centerId);
			if (band) FillBand(band, ImGuiDir_Up, up, totalUp);
		}
		if (!down.empty() && totalDown > 0.f) {
			const float avail = 1.0f - totalUp;
			const float frac = avail > 0.0f ? totalDown / avail : 1.0f;
			ImGuiID band = 0;
			ImGui::DockBuilderSplitNode(centerId, ImGuiDir_Down, frac, &band, &centerId);
			if (band) FillBand(band, ImGuiDir_Down, down, totalDown);
		}

		// 中心窗口最后 dock 到最终的中心节点(切分全部结束后 centerId 才是最终的中心)
		for (const auto* e : center)
			ImGui::DockBuilderDockWindow(e->title.c_str(), centerId);

		ImGui::DockBuilderFinish(dockspaceId);
	}
}
