#include "spch.h"
#include "Material.h"
#include "Core/Assets/FileSystem.h"

namespace Six {
	void SMaterial::Bind() {
		if (!shader)
		{
			return;
		}
		auto bindTex = [&](const char* name, STexture* tex, int slot) {
			if (!tex) return;
			shader->SetUniform(name,tex, slot);
			};
		shader->Bind();
		bindTex("uAlbedo", albedo, 0);
		bindTex("uNormal", normal, 1);
		bindTex("uMetallic", metallic, 2);
		bindTex("uRoughness", roughness, 3);
		bindTex("uAO", AO, 4);
		bindTex("uHeight", height, 5);
		bindTex("uMask", detailMask, 6);
	}
	void SMaterial::Unbind() {
		shader->Unbind();
	}

	SMaterial::~SMaterial() {
		Unbind();
		delete shader;
		delete albedo;
		delete normal;
		delete metallic;
		delete roughness;
		delete AO;
		delete height;
		delete detailMask;
	}

	SMaterial* SMaterial::MakeDefault(SShader* shader)
	{
		auto albedo = Six::STexture::Create((Six::SGetEngineDllDir() / "assets" / "DefaultTexture.png").string());
		auto normal = Six::STexture::Create((Six::SGetEngineDllDir() / "assets" / "DefaultNormal.png").string());
		auto metallic = Six::STexture::Create((Six::SGetEngineDllDir() / "assets" / "DefaultMetallic.png").string());
		auto roughness = Six::STexture::Create((Six::SGetEngineDllDir() / "assets" / "DefaultRoughness.png").string());
		auto ao = Six::STexture::Create((Six::SGetEngineDllDir() / "assets" / "DefaultAO.png").string());
		auto height = Six::STexture::Create((Six::SGetEngineDllDir() / "assets" / "DefaultHeight.png").string());
		auto mask = Six::STexture::Create((Six::SGetEngineDllDir() / "assets" / "DefaultMask.png").string());
		auto material = new Six::SMaterial{
			.shader = shader,
			.albedo = albedo,
			.normal = normal,
			.metallic = metallic,
			.roughness = roughness,
			.AO = ao,
			.height = height,
			.detailMask = mask
		};
		return material;
	}
}

