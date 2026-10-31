#pragma once
#include "sixcore.h"

struct GLFWwindow;
namespace Six {
	class SIXAPI SGraphicsContext {
	public:
		~SGraphicsContext() = default;
		virtual bool Init() = 0;
		virtual void SwapBuffers() = 0;
		static SGraphicsContext* Create(GLFWwindow* window);
	protected:
		GLFWwindow* m_window = nullptr;
	};

}