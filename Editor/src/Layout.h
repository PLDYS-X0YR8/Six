#pragma once
#include <map>
#include <vector>
#include <string>
#include <imgui.h>
#include <imgui_internal.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace SixEditor {
	// Center = 占剩余中心区域(不参与切分)
	// Float  = 浮动窗口。注意 ImGuiDir_None 的语义是"不切分",不是"浮动";
	//          浮动靠窗口级 ImGuiWindowFlags_NoDock,需要 SEPanel 支持 per-panel flag。
	//          当前 DockBuilder 流程不处理它(见 Layout.cpp),该模式下元素会被跳过。
	enum class SELayoutDockMode {
		Up,
		Down,
		Left,
		Right,
		Center,
		Float
	};
	struct SEGUILayoutElement {
		std::string title;   // 必须 == ImGui::Begin() 的窗口名
		SELayoutDockMode mode;
		float ratio;         // 该元素占整个 dockspace 的比例(绝对值,0~1);Center/Float 忽略
	};
	struct SEGUILayout {
		std::string name;                    // preset 名;等于它在 m_presets 里的 key
		std::vector<SEGUILayoutElement> elements;
	};
	class SELayout {
	public:
		static SELayout& Get() {
			static SELayout instance;
			return instance;
		}

		const std::map<std::string, SEGUILayout>& Presets() const { return m_presets; }
		const SEGUILayout* Find(const std::string& name) const;
		void AddPreset(const std::string& name, const SEGUILayout& lay);
		void DelPreset(const std::string& name);


		// 当前选中的布局(整个对象,方便直接读 elements)
		const SEGUILayout& Current() const { return m_current; }
		const std::string& CurrentName() const { return m_current.name; }
		void SetCurrent(const std::string& name) {
			// 用 find 而不是 m_presets[name]:operator[] 在名字不存在时会插入新元素,
			// 导致调用方正在遍历 m_presets 的迭代器失效
			const SEGUILayout* lay = Find(name);
			if (!lay) return;
			m_current = *lay;
			m_needRebuild = true;
		}
		bool NeedsRebuild() const { return m_needRebuild; }
		void MarkBuilt() { m_needRebuild = false; }

		void Apply(ImGuiID dockspaceId, const SEGUILayout& lay);

	private:
		static std::map<std::string, SEGUILayout> MakePresets() {
			std::map<std::string, SEGUILayout> presets;

			SEGUILayout onlyGame;
			onlyGame.name = "Only Game";
			onlyGame.elements = {
				{ "Game View", SELayoutDockMode::Center, 0.0f },
				{ "Log",       SELayoutDockMode::Down,  0.25f },
			};
			presets["Only Game"] = onlyGame;

			SEGUILayout editor;
			editor.name = "Editor";
			editor.elements = {
				{ "Hierarchy", SELayoutDockMode::Left,  0.20f },
				{ "Inspector", SELayoutDockMode::Right, 0.25f },
				{ "Log",       SELayoutDockMode::Down,  0.25f },
				{ "Game View", SELayoutDockMode::Center, 0.0f },
			};
			presets["Editor"] = editor;

			return presets;
		}

		std::map<std::string, SEGUILayout> m_presets = MakePresets();
		SEGUILayout m_current = m_presets.at("Editor");
		bool m_needRebuild = true;
		ImGuiDir DM2IGD(SELayoutDockMode m);
	};
}
