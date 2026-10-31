#include "spch.h"
#include "GLRenderAPI.h"
#include <glad/glad.h>

namespace Six {
	bool S_GLRenderAPI::Init() {
		if (!gladLoadGL()) {
			return false;
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		return true;
	}

	void S_GLRenderAPI::SetViewport(int x, int y, int width, int height) {
		glViewport(x, y, width, height);
	}

	void S_GLRenderAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void S_GLRenderAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
