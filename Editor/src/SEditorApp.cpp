#include "SEditorApp.h"
#include "Testing/YSpin.h"
#include "Layout.h"
#include <imgui_internal.h>
#include <iostream>
#include <format>

namespace SixEditor {

    bool SEditorApp::OnInit() {
		bool ok = true;
        Six::SAppContext::LoadProject("E:/Six/SixEngine/Editor/Editor.sixproj.json");
        const int w = Six::GetConfigInt("window-width", 1280);
        const int h = Six::GetConfigInt("window-height", 720);
        const std::string title = Six::GetConfigStr("window-title", "SixEngine Editor");
    const std::string entry = Six::SGetProject()->GetEntryPoint();
        m_win = new Six::SWindow(w, h, title, ok);
        Six::SRenderer::Get().SetWindow(m_win);
        // 场景名直接用 proj 的 "Entry Point"(和 Sandbox 同一个模式)
        AddSceneToRoot(Six::SEntity::Create(entry,Six::SEntityType::Scene));
        SetActiveScene(entry);
        auto layout = Six::SVertexLayout();
        layout.elements.push_back({ 0, 3, 0x1406, 0 });                     // pos @ offset 0
        layout.elements.push_back({ 1, 2, 0x1406, 3 * sizeof(float) });       // uv @ offset 12
        layout.elements.push_back({ 2, 3, 0x1406, 5 * sizeof(float) });       // normal @ offset 20
        layout.stride = 8 * sizeof(float);
        auto shader = new Six::SShader((Six::SGetEngineDllDir() / "assets" / "DefaultShader.vert").string(), (Six::SGetEngineDllDir() / "assets" / "DefaultShader.frag").string());
        auto mesh = new Six::SMesh(layout, m_vertices, m_indices);
        auto square = GetActiveScene()->CreateChild("Square");
        auto camera = GetActiveScene()->CreateChild("Main Camera");
        auto light = GetActiveScene()->CreateChild("Sun");
        {
            auto dirLight = light->CreateComponent<Six::SDirLight>();
            dirLight->dir = glm::normalize(glm::vec3(0.5f, 0.8f, 0.5f));
            dirLight->color = glm::vec3(1.f, 1.f, 1.f);
            dirLight->intensity = 3.f;
            auto transform = light->CreateComponent<Six::STransform>();
            transform->position = { 1.f,1.f,1.f };
        }
        {
            auto transform = square->CreateComponent<Six::STransform>();
            transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
            square->CreateComponent<Six::SRenderRes>(mesh, Six::SMaterial::MakeDefault(shader));
            square->CreateProgram<YSpin>();
        }
        {
            auto transform = camera->CreateComponent<Six::STransform>();
            transform->position = glm::vec3(0.0f, 0.0f, -3.0f);
            transform->rotation = glm::radians(glm::vec3(0.f, 0.f, -1.f));
            auto camcom = camera->CreateComponent<Six::SCamera>();
            camcom->is_main = true;
        }
        GetActiveScene()->CreateSystem<Six::SRenderSystem>();
        GetActiveScene()->CreateSystem<Six::SEnvironment>();
        m_panels["Game View"] = new SEGameView();
        static_cast<SEGameView*>(m_panels["Game View"])->SetViewport(Six::SViewport::Create(1280, 720));
        GetActiveScene()->GetSystem<Six::SRenderSystem>()->SetViewport(static_cast<SEGameView*>(m_panels["Game View"])->GetViewport());
        Six::SApplication::OnInit();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_win->GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
        // 注: viewport system 在 ImGui_ImplGlfw_Init 内部自动初始化
        m_imguiReady = true;
        m_panels["Logger"] = &SELogWin::Get();
        m_panels["Inspector"] = new SEInspector();
        m_panels["Hierarchy"] = new SEHierarchy();
        m_panels["Project Setting"] = new SEProjSetting();
        m_panels["Project Setting"]->SetOpen(false);
        SyncPanelVisibility();
        return ok;
    }

    void SEditorApp::OnUpdate() {
        // 用上一帧 Game View 的面板尺寸同步 FBO。
        // 必须在 m_scene->Update()(内部会 DrawTo)之前,否则这一帧的 projection
        // 和 glViewport 跟 texture 尺寸对不上。
        if (static_cast<SEGameView*>(m_panels["Game View"])->GetViewport() && static_cast<SEGameView*>(m_panels["Game View"])->GetSize().x > 0.f && static_cast<SEGameView*>(m_panels["Game View"])->GetSize().y > 0.f) {
            static_cast<SEGameView*>(m_panels["Game View"])->GetViewport()->ResizeViewport((int)static_cast<SEGameView*>(m_panels["Game View"])->GetSize().x, (int)static_cast<SEGameView*>(m_panels["Game View"])->GetSize().y);
        }

        //render
        Six::SApplication::OnUpdate();
        if (!m_imguiReady) return;

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        BuildDockLayout();

        for (auto&[n,p]:m_panels)
        {
            if (p) p->Render();   // 要在 ImGui::Render() 之前 — Render() ends frame
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // viewport / platform windows: Update + render  detached OS windows
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(m_win->GetNativeWindow());
        m_win->OnUpdate();
    }

    void SEditorApp::BuildDockLayout() {
        const ImGuiViewport* vp = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(vp->WorkPos);
        ImGui::SetNextWindowSize(vp->WorkSize);
        ImGui::SetNextWindowViewport(vp->ID);

        const ImGuiWindowFlags hostFlags =
            ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("##EditorDockHost", nullptr, hostFlags);
        ImGui::PopStyleVar(3);

        const ImGuiID dockspaceId = ImGui::GetID("##EditorDockSpace");

        // ===== 主菜单栏 =====
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) m_running = false;
                ImGui::Separator();
                if (ImGui::MenuItem("Project Settings")) {
                    m_panels["Project Setting"]->SetOpen(true);
                };
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Game View", nullptr, nullptr, true)) {
                    m_panels["Game View"]->SetOpen(true);
                };
                if(ImGui::MenuItem("Log", nullptr, nullptr, true)) m_panels["Logger"]->SetOpen(true);
                if(ImGui::MenuItem("Inspector", nullptr, nullptr, true)) m_panels["Inspector"]->SetOpen(true);
                if(ImGui::MenuItem("Hierarchy", nullptr, nullptr, true)) m_panels["Hierarchy"]->SetOpen(true);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Layout")) {
                for (const auto& kv : SELayout::Get().Presets()) {
                    const bool cur = (kv.first == SELayout::Get().CurrentName());
                    if (ImGui::MenuItem(kv.first.c_str(), nullptr, cur) && !cur) {
                        SELayout::Get().SetCurrent(kv.first);
                        SyncPanelVisibility();   // preset 里没列出的面板保持关闭
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // 布局交给 SELayout 管:首次启动 / 用户切换 preset 时重建,其余帧不动
        SELayout& layout = SELayout::Get();
        if (layout.NeedsRebuild()) {
            layout.Apply(dockspaceId, layout.Current());
            layout.MarkBuilt();
        }

        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
    }

    void SEditorApp::CloseAll()
    {
        for (auto& [n, p] : m_panels)
        {
            p->SetOpen(false);
        }
    }

    // 让面板的开关状态跟随当前 preset:
    //   preset 里列出的 -> 打开;没列出的 -> 保持关闭(不占 dock 位置)
    // 按 GetTitle() 匹配窗口名,所以 m_panels 的 key 叫什么都无所谓
    // ("Logger" 对应窗口名 "Log" 这个坑就自动消失了)
    void SEditorApp::SyncPanelVisibility()
    {
        CloseAll();
        const SEGUILayout& cur = SELayout::Get().Current();
        for (const auto& e : cur.elements)
        {
            for (auto& kv : m_panels)
            {
                SEPanel* p = kv.second;
                if (p && p->GetTitle() == e.title)
                {
                    p->SetOpen(true);
                    break;
                }
            }
        }
    }

    void SEditorApp::OnShutdown() {
        if (static_cast<SELogWin*>(m_panels["Logger"])) {
            static_cast<SELogWin*>(m_panels["Logger"])->Send(SixEditor::SELogLevel::Info, "Editor shutting down.", "Shutdown", __FILE__, __TIME__, __LINE__);
            m_panels["Logger"] = nullptr;
        }
        if (m_imguiReady) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();  // viewposrt system SHUTDOWN 也在内部
            ImGui::DestroyContext();
        }
        Six::SApplication::OnShutdown();
        if (static_cast<SEGameView*>(m_panels["Game View"])->GetViewport()) { delete static_cast<SEGameView*>(m_panels["Game View"])->GetViewport(); m_panels["Game View"] = nullptr; }
        for (auto& [n, p] : m_panels)
        {
            p = nullptr;
        }
        m_win->OnShutdown();
    }

    bool SEditorApp::ShouldClose() {
        return m_win->ShouldClose() || !m_running;
    }

}
