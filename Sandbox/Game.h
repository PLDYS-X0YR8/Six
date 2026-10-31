#pragma once
#include "SixEngine.h" // 包含 SApplication 的头文件

class Game : public Six::SApplication { // 1. 加上 public
public: // 2. 加上 public:
	Game() = default;

	// 重写帧更新逻辑
	void OnUpdate() override;
	bool OnInit() override;
	void OnShutdown() override;
	bool ShouldClose() override;
private:
	Six::SWindow* m_win = nullptr;
	Six::SEntity* m_scene = nullptr;
    // cube vertex: pos(3) + uv(2) + normal(3) = 8 floats/vertex, stride = 8*sizeof(float)
    // normal = normalize(position) — symmetric cube 的每个 vertex 自然 smooth normal
    // 这对应:vertex 0 (-0.5,-0.5, 0.5) → normal = normalize((-0.5,-0.5, 0.5)) ≈ (-0.577, -0.577, 0.577)
    // 36-vertex flat-shaded cube: 每个 face 6 个 vertex (per-tri 重复)
// pos(3) + uv(2) + normal(3) = 8 floats/vertex, stride = 8*sizeof(float)
// 每个 face 内 4 顶点共享 face normal(face-aligned,不是平均 normal)
// face 顶点顺序按右手定则 CCW(从外面看)
std::vector<float> m_vertices = {
    // ===== 前面 +Z (n = (0, 0, 1)) =====   顶点 0..5  (4 顶点 ×2 tri 复制)
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  0.0f,  1.0f,    // 0 bot-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    // 1 bot-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f,  0.0f,  1.0f,    // 2 top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f,  0.0f,  1.0f,    // 3 = 2 (tri 2)
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,    0.0f,  0.0f,  1.0f,    // 4 top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  0.0f,  1.0f,    // 5 = 0 (tri 2)

    // ===== 右面 +X (n = (1, 0, 0)) =====   顶点 6..11
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    1.0f,  0.0f,  0.0f,    // 6
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    1.0f,  0.0f,  0.0f,    // 7
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f,  0.0f,  0.0f,    // 8
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f,  0.0f,  0.0f,    // 9 = 8
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,    1.0f,  0.0f,  0.0f,    // 10
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    1.0f,  0.0f,  0.0f,    // 11 = 6

    // ===== 后面 -Z (n = (0, 0, -1)) =====   顶点 12..17
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,    // 12
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,    // 13
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,    // 14
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,    // 15 = 14
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,    0.0f,  0.0f, -1.0f,    // 16
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,    // 17 = 12

    // ===== 左面 -X (n = (-1, 0, 0)) =====   顶点 18..23
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,    // 18
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,    // 19
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,    // 20
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,    // 21 = 20
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,    // 22
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,    // 23 = 18

    // ===== 顶面 +Y (n = (0, 1, 0)) =====   顶点 24..29
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  1.0f,  0.0f,    // 24
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f,  1.0f,  0.0f,    // 25
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  1.0f,  0.0f,    // 26
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  1.0f,  0.0f,    // 27 = 26
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,    0.0f,  1.0f,  0.0f,    // 28
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  1.0f,  0.0f,    // 29 = 24

    // ===== 底面 -Y (n = (0, -1, 0)) =====   顶点 30..35
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    // 30
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    0.0f, -1.0f,  0.0f,    // 31
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,    0.0f, -1.0f,  0.0f,    // 32
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,    0.0f, -1.0f,  0.0f,    // 33 = 32
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,    0.0f, -1.0f,  0.0f,    // 34
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    // 35 = 30
};

std::vector<unsigned int> m_indices = {
    // 前面 +Z:  0,1,2 / 2,4,5 (because 3 = 2, 4 duplicate, 5 = 0 in tri-duplicated scheme)
    // Let me verify: tri 1 = (0,1,2), tri 2 = (3,4,5) where 3=2 pos, 4=top-left, 5=0 pos
    0, 1, 2,   3, 4, 5,        // 前面 (+Z)
    6, 7, 8,   9, 10, 11,     // 右面 (+X)
    12, 13, 14, 15, 16, 17,   // 后面 (-Z)
    18, 19, 20, 21, 22, 23,  // 左面 (-X)
    24, 25, 26, 27, 28, 29,  // 顶面 (+Y)
    30, 31, 32, 33, 34, 35,  // 底面 (-Y)
};
};