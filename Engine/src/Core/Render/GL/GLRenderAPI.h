#pragma once
#include "../RenderAPI.h"

namespace Six {
	class S_GLRenderAPI : public SRenderAPI {
	public:
		virtual bool Init() override;
		virtual void SetViewport(int x, int y, int width, int height) override;
		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void Clear() override;
	};
}
