#pragma once
#include "SixEngine.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glfw3.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "Log.h"
#include "GameView.h"
#include "Inspector.h"
#include "PanelBase.h"
#include "Hierarchy.h"
#include "ProjectSetting.h"

namespace SixEditor {
    class SEditorApp : public Six::SApplication {
    public:
        SEditorApp() = default;
        ~SEditorApp() override = default;

        bool OnInit() override;
        void OnUpdate() override;
        void OnShutdown() override;
        bool ShouldClose() override;
    private:
        // 搭 DockSpace + 主菜单;具体面板布局由 SELayout 的 preset 决定
        void BuildDockLayout();
        void CloseAll();
        void SyncPanelVisibility();
        Six::SWindow* m_win = nullptr;
        std::unordered_map<std::string, SEPanel*> m_panels;
        bool m_imguiReady = false;
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
}
