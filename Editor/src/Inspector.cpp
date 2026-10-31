#include "Inspector.h"
#include <glm/glm.hpp>
#include "Testing/YSpin.h"

namespace SixEditor {
	void SEInspector::Render() {
		if (m_open) {
			if (ImGui::Begin("Inspector", &m_open))
			{
				ImGui::SeparatorText("Components");
				if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
					auto selected = SEContext::Get().selected;
					Six::STransform* trans = nullptr;
					if(selected) 
					{
						trans = selected->GetComponent<Six::STransform>();
						ImGui::PushID(selected);
					}
					if(trans){
						ImGui::Text("position");
						ImGui::SameLine();
						ImGui::PushID("Transform");
						float pos[3] = { trans->position.x, trans->position.y, trans->position.z };
						if (ImGui::DragFloat3("##Position", pos, 0.1f))
							trans->position = glm::vec3(pos[0], pos[1], pos[2]);
						ImGui::Text("rotation");
						ImGui::SameLine();
						float rot[3] = { trans->rotation.x, trans->rotation.y, trans->rotation.z };
						if (ImGui::DragFloat3("##Rotation", rot, 0.1f)) {
							// 拖完必须重新归一化,否则 mat4_cast 出来的矩阵不是纯旋转 → 物体变形
							// 注意: GLM 的四元数构造顺序是 (w, x, y, z) —— w 在前!
							// 写成 quat(rot[0], rot[1], rot[2], w) 会把 rot[0] 当成 w,转轴全错。
							trans->rotation = glm::normalize(glm::quat(trans->rotation.w, rot[0], rot[1], rot[2]));
						}
						ImGui::Text("scale   ");
						ImGui::SameLine();
						float scl[3] = { trans->scale.x, trans->scale.y, trans->scale.z };
						if (ImGui::DragFloat3("##Scale", scl, 0.1f))
							trans->scale = glm::vec3(scl[0], scl[1], scl[2]);
						ImGui::PopID();
					}
					// 配对 line17 的 PushID(selected): selected 为空时那里根本没 push,
					// 这里就不能 pop —— 否则 ID 栈多弹一层,TreePop 会撞 7419 的 assert
					if (selected) ImGui::PopID();
					ImGui::TreePop();
				}
				ImGui::SeparatorText("Programs");
				//if (ImGui::TreeNodeEx("YSpin", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
				//	auto yspin = Six::SGetApp()->GetActiveScene()->GetChild("Square")->GetProgram<YSpin>();
				//	if (yspin)
				//	{
				//		ImGui::Text("speed");
				//		ImGui::SameLine();
				//		ImGui::DragFloat("##Speed", &yspin->speed, 0.1f);
				//	}
				//	ImGui::TreePop();
				//}
				//ImGui::SeparatorText("Systems");
				//if (ImGui::TreeNodeEx("Environment-Scene", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) { 
				//	auto env = Six::SGetApp()->GetActiveScene()->GetSystem<Six::SEnvironment>();
				//	if (env)
				//	{
				//		ImGui::Text("intensity");
				//		ImGui::SameLine();
				//		ImGui::DragFloat("##Intensity", env->GetIntensityPtr(), 0.1f);
				//	}
				//	ImGui::TreePop();
				//} 
			}
			ImGui::End();
		}
	}
}
