#pragma once
#include "../GraphicsContext.h"

namespace Six {
	class SIXAPI S_GLContext : public SGraphicsContext {
	public:
		S_GLContext(GLFWwindow* window);
		virtual bool Init() override;
		virtual void SwapBuffers() override;
	};

}