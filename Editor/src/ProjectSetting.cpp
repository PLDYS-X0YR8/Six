#include "ProjectSetting.h"

namespace SixEditor {
	void SEProjSetting::Render() {
        if (m_open) {
            if (ImGui::Begin("Project Setting", &m_open))
            {
                float footerHeight = 45.0f;
                ImVec2 fullAvail = ImGui::GetContentRegionAvail();
                float contentHeight = fullAvail.y - footerHeight;
                ImGui::BeginChild("MainContentArea", ImVec2(0, contentHeight), false);

                // --- 左侧分类 ---
                float leftPanelWidth = 180.0f;
                ImGui::BeginChild("SettingCategories", ImVec2(leftPanelWidth, 0), false);
                static int selectedCategory = 0;
                const char* categories[] = { "General" };

                for (int i = 0; i < IM_ARRAYSIZE(categories); i++) {
                    if (ImGui::Selectable(categories[i], selectedCategory == i)) {
                        selectedCategory = i;
                    }
                }
                ImGui::EndChild();

                // --- 竖分割线（保留你原来的画线逻辑）---
                ImGui::SameLine();
                ImVec2 linePos = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddLine(
                    ImVec2(linePos.x, linePos.y),
                    ImVec2(linePos.x, linePos.y + contentHeight),
                    ImGui::GetColorU32(ImGuiCol_Border),
                    1.0f
                );
                ImGui::Dummy(ImVec2(10.0f, 0.0f));
                ImGui::SameLine();

                // --- 右侧内容区 ---
                ImGui::BeginChild("SettingContent", ImVec2(0, 0), false); // 高度填满
                switch (selectedCategory) {
                case 0:
                    ImGui::PushID(categories[0]);
                    ImGui::Text("General Settings");
                    ImGui::Separator();

                    ImGui::Text("Project Name");
                    ImGui::SameLine();
                    ImGui::InputText("##Name", Six::SAppContext::Project()->GetSetting()->GetNameP());

                    ImGui::Text("Project Entry Point");
                    ImGui::SameLine();
                    static int idx = 0;
                    auto scenes = Six::SGetApp()->GetScenes();
                    if (!scenes.empty()) {
                        if (idx >= scenes.size()) idx = 0;
                        auto getSceneName = [](void* data, int index) -> const char* {
                            auto& sceneList = *(static_cast<std::vector<Six::SEntity*>*>(data));
                            return sceneList[index]->GetName().c_str();
                            };
                        if (ImGui::Combo("##scenes", &idx, getSceneName, &scenes, static_cast<int>(scenes.size()))) {
                            *Six::SGetProject()->GetSetting()->GetEntryPointP() = scenes[idx]->GetName();
                        }
                    }
                    ImGui::PopID();
                    break;
                }
                ImGui::EndChild(); // 结束右侧内容区

                ImGui::EndChild(); // 结束上方主内容区

                // ==========================================
                // 3. 底部操作栏（你要的水平分割线）
                // ==========================================
                ImGui::Separator(); // <--- 这里就是贯穿左右的水平分割线

                // 为了美观，稍微往下挪一点按钮
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                // 靠右对齐
                float buttonWidth = 100.0f;
                float buttonHeight = 30.0f;
                float padding = 10.0f;

                // 将按钮推到最右侧
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - padding);

                // 绘制 Save 按钮
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.8f, 1.0f)); // 蓝色
                if (ImGui::Button("Save", ImVec2(buttonWidth, buttonHeight))) {
                    if (Six::SaveProjectConfig()) {
                        SELogWin::Get().SE_LOG(SELogLevel::Info, "Saved!", "save");
                    }
                }
                ImGui::PopStyleColor();

            }
            ImGui::End();
        }
	}
}