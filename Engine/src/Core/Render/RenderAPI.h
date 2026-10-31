#pragma once
#include "sixcore.h"
#include "graphicObj/Texture.h"

namespace Six {
	class SRenderAPI {
	public:
		enum class SBackendAPI {
			OpenGL,
			Vulkan
		};
	public:
		virtual bool Init() = 0;
		virtual void SetViewport(int x, int y, int width, int height) = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;
		static SBackendAPI GetBackendAPI() { return s_backend; }
		static void SetBackendAPI(SBackendAPI api) { s_backend = api; }
		static SRenderAPI* Create();
	private:
		static SBackendAPI s_backend;
	};
}