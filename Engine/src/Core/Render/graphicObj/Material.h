#pragma once
#include "sixcore.h"
#include "Texture.h"
#include "../Shader.h"

namespace Six {
	class SIXAPI SMaterial
	{
	public:
        SShader *shader;
        STexture *albedo, *normal, *metallic, *roughness, *AO, *height, *detailMask;
        float metallicFactor = 1.0f;
        float roughnessFactor = 1.0f;
        float normalScale = 1.0f;
        float aoStrength = 1.0f;
        glm::vec2 uvScale = { 1,1 }, uvOffset = { 0,0 };
        void Bind();
        void Unbind();
        ~SMaterial();
        static SMaterial* MakeDefault(SShader* shader);
	};
}