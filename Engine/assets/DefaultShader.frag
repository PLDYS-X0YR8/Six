#version 330 core

in vec2 texCoord;
in vec3 worldPos;
in vec3 fNormal;
out vec4 FragColor;

uniform sampler2D uAlbedo;
uniform sampler2D uNormal;
uniform sampler2D uMetallic;
uniform sampler2D uRoughness;
uniform sampler2D uAO;
uniform sampler2D uHeight;
uniform sampler2D uMask;
struct DirLight{
    vec3 direction;
    vec3 color;
    float intensity;
};
struct Ambient{
    vec3 sky;      // (0.35, 0.40, 0.50)
    vec3 ground;   // (0.15, 0.13, 0.12)
    float intensity;
};

uniform int uDLightCount;
uniform DirLight[32] uDLights;
uniform Ambient uAmbient;
uniform vec3 uViewPos;

float D_GGX(vec3 N, vec3 H, float roughness) {
    float a  = roughness * roughness;       // α = r² (UE convention)
    float a2 = a * a;
    float NoH = max(dot(N, H), 0.0);
    float NoH2 = NoH * NoH;
    float denom = (NoH2 * (a2 - 1.0) + 1.0);
    return a2 / (3.14159265 * denom * denom);
}
// === F — Schlick Fresnel ===
vec3 F_Schlick(float cosTheta, vec3 F0) {
    return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}
// === G — Smith + Schlick-GGX ===
float G_SchlickGGX(float NoV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NoV / (NoV * (1.0 - k) + k);
}
float G_Smith(float NoL, float NoV, float roughness) {
    return G_SchlickGGX(NoV, roughness) * G_SchlickGGX(NoL, roughness);
}
// === Cook-Torrance specular BRDF ===
vec3 SpecularBRDF(vec3 N, vec3 V, vec3 L, vec3 F0, float roughness) {
    vec3 H = normalize(V + L);
    float NoL = max(dot(N, L), 0.0);
    float NoV = max(dot(N, V), 0.0);
    float NoH = max(dot(N, H), 0.0);
    float VoH = max(dot(V, H), 0.0);
    float D = D_GGX(N, H, roughness);
    vec3  F = F_Schlick(VoH, F0);
    float G = G_Smith(NoL, NoV, roughness);
    float denom = 4.0 * NoV * NoL + 0.001;  // 防止除 0
    return (D * F * G) / denom;
}
// === 最终 direct lighting (REDIRECT) ===
vec3 BRDF_DirLight(vec3 albedo, float metallic, float roughness,
                   vec3 N, vec3 V, vec3 L, vec3 lightColor) {
    // F0:金属用 albedo 作基础反射率;非金属用 0.04 (玻璃/塑料)
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F  = F_Schlick(max(dot(N, V), 0.0), F0);
    vec3 Fs = SpecularBRDF(N, V, L, F0, roughness);
    // 漫反射 + 镜面反射
    vec3 kD   = (vec3(1.0) - F) * (1.0 - metallic);
    vec3 diff = kD * albedo / 3.14159265;
    vec3 spec = Fs;
    return (diff + spec) * lightColor * max(dot(N, L), 0.0);
}

void main() {
    vec3 albedo  = texture(uAlbedo, texCoord).rgb * texture(uMask, texCoord).r;
    float metal  = texture(uMetallic, texCoord).r;
    float rough  = texture(uRoughness, texCoord).r;
    float ao     = texture(uAO, texCoord).r;
    vec3 N = normalize(fNormal);  // vertex normal (mat3(uTransform) * vNormal)
    // 用 normal map 扰动(flat PNG (128,128,255) 几乎不扰动,但 pipeline 打开)
    vec3 nT = texture(uNormal, texCoord).rgb * 2.0 - 1.0;
    vec3 up = abs(N.y) < 0.99 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
    vec3 T = normalize(cross(up, N));
    vec3 B = cross(N, T);
    N = normalize(T * nT.x + B * nT.y + N * nT.z);

    vec3 V = normalize(uViewPos - worldPos);
    vec3 color = vec3(0.0);
    float hemi = N.y * 0.5 + 0.5;
    vec3 ambient = mix(uAmbient.ground, uAmbient.sky, hemi) * uAmbient.intensity;
    color += ambient * albedo * (1.0 - metal);
    for (int i=0;i<uDLightCount;i++){
        // convention: uDLights[i].direction = scene→light (PardCode 风格)
        // L = light→fragment,所以必须取反;不取反则 dot(N,L)<0,
        // max(dot(N,L),0)=0 → BRDF_DirLight 返回 0 → cube 全黑
        vec3 L = normalize(-uDLights[i].direction);
        color += BRDF_DirLight(albedo,metal,rough,N,V,L,uDLights[i].color) * uDLights[i].intensity;
    }

    FragColor = vec4(color, 1.0);
}