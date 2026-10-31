#include "Hierarchy.h"

namespace SixEditor {
	void SEHierarchy::Render() {
		if (m_open) {
			if (ImGui::Begin("Hierarchy", &m_open)) {
				if (ImGui::TreeNode("Scene")) {
					auto* selected = SEContext::Get().selected;
					auto asc = Six::SGetApp()->GetActiveScene();
					if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
						if (asc != nullptr) {
							if (ImGui::MenuItem("Create Positioned Entity")) {
								auto* newEntity = asc->CreateChild("New Entity");
								newEntity->CreateComponent<Six::STransform>();
							}
							if (ImGui::MenuItem("Create Logical Entity")) {
								auto* newEntity = asc->CreateChild("New Entity");
							}
						}
						ImGui::EndPopup();
					}
					if (asc != nullptr) {
						for (auto& e : asc->GetChildren()) {
							DrawEntityTree(asc, e);
						}
					}
					ImGui::TreePop(); // 极其重要！配对 line7 的 TreeNode("Scene")
				}
			}
			ImGui::End();
		}
	}

    void SEHierarchy::DrawEntityTree(Six::SEntity* scn, Six::SEntity* ent) {
        if (!ent) return;
        ImGui::PushID(ent);
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (ent->GetChildren().empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }
        if (SEContext::Get().selected == ent) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        bool opened = ImGui::TreeNodeEx((void*)(uintptr_t)ent, flags, "%s", ent->GetName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            SEContext::Get().selected = ent;
        }
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Create Child Entity")) {
                auto* newChild = ent->CreateChild("New Child");
                if (newChild) {
                    newChild->CreateComponent<Six::STransform>();
                }
            }
            if (ImGui::MenuItem("Create Logical Child Entity")) {
                auto* newChild = ent->CreateChild("New Child");
                if (newChild) {
                    
                }
            }
            ImGui::Separator();

            if (ImGui::MenuItem("Create Parent Entity")) {
                auto* newParent = ent->CreateParent("New Parent");
                if (newParent) {
                    newParent->CreateComponent<Six::STransform>();
                }
            }
            if (ImGui::MenuItem("Create Logical Parent Entity")) {
                auto* newParent = ent->CreateParent("New Parent");
                if (newParent) {

                }
            }
            ImGui::Separator();

            if (ImGui::MenuItem("Delete Entity")) {
                if (SEContext::Get().selected == ent) {
                    SEContext::Get().selected = nullptr;
                }

                ent->Delete();

                ImGui::EndPopup();
                ImGui::PopID();
                return;
            }
            ImGui::EndPopup();
        }
        if (opened) {
            for (auto* child : ent->GetChildren()) {
                DrawEntityTree(scn, child);
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}
