#pragma once
#include "spch.h"
#include "../Component.h"
#include "../Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

namespace Six {
	class STransform:public SComponent {
	SIX_COMPONENT(STransform)
	public:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		STransform() : position(0.0f), rotation(glm::quat()), scale(1.0f) {}
		void Translate(const glm::vec3& delta) {
			position += delta;
		}

		// 局部空间平移：沿物体自己的轴移动
		void TranslateLocal(const glm::vec3& delta) {
			position += rotation * delta;
		}
		void Rotate(const glm::vec3& axis, float angleRad) {
			glm::quat delta = glm::angleAxis(angleRad, glm::normalize(axis));
			rotation = glm::normalize(delta * rotation);
		}

		// 绕物体自己的轴旋转（局部空间）
		void RotateLocal(const glm::vec3& axis, float angleRad) {
			glm::quat delta = glm::angleAxis(angleRad, glm::normalize(axis));
			rotation = glm::normalize(rotation * delta);
		}

		// 用欧拉角追加旋转（编辑器友好）
		void RotateEuler(const glm::vec3& eulerDeg) {
			glm::quat delta = glm::quat(glm::radians(eulerDeg));
			rotation = glm::normalize(delta * rotation);
		}
		// 均匀缩放
		void ScaleBy(float factor) {
			scale *= factor;
		}

		// 非均匀缩放
		void ScaleBy(const glm::vec3& factor) {
			scale *= factor;
		}
		glm::vec3 GetRight() {
			return glm::normalize(glm::vec3(glm::mat4_cast(rotation)[0]));
		}
		glm::vec3 GetUp() {
			return glm::normalize(glm::vec3(glm::mat4_cast(rotation)[1]));
		}
		glm::vec3 GetForward() {
			return glm::normalize(glm::vec3(glm::mat4_cast(rotation)[2]));
		}
		glm::mat4 GetMatrix() const {
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
			// 用 mat4_cast 而不是 glm::rotate(angle, axis) ——
			// glm::axis(identity_quat) = (0,0,0), normalize = NaN,会污染整个 matrix
			glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
			return translationMatrix * rotationMatrix * scaleMatrix;
		}
		glm::mat4 GetWorldMatrix() const {
			glm::mat4 localMatrix = GetMatrix();
			if (GetOwner()->GetParent()) {
				STransform* parentTransform = GetOwner()->GetParent()->GetComponent<STransform>();
				if (parentTransform) {
					return parentTransform->GetWorldMatrix() * localMatrix;
				}
			}
			return localMatrix;
		}
		std::string toStr() override {
			return "STransform";
		}
	};
}
