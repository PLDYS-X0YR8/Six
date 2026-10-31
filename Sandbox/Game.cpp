#include "Game.h"
#include "MyProgram.h"   // 读 *.sixconf.json 的窗口参数

void Game::OnUpdate() {
    m_scene->Update();
    m_win->OnUpdate();
}

bool Game::OnInit() {
    bool ok = true;
    // 窗口参数改走项目配置(sandboxconf.sixconf.json);
    // 没有项目配置 / 键不存在时退回默认值
    int w = Six::GetConfigInt("window-width", 800);
    int h = Six::GetConfigInt("window-height", 800);
    std::string title = Six::GetConfigStr("window-title", "SixEngine Sandbox");
    const std::string entry = Six::SGetProject()->GetEntryPoint();
    m_win = new Six::SWindow(w, h, title, ok);
    m_win->SetVsync(Six::GetConfigBool("window-vsync", true));
    Six::SRenderer::Get().SetWindow(m_win);
    auto layout = Six::SVertexLayout();
    layout.elements.push_back({ 0, 3, 0x1406, 0 });                     // pos @ offset 0
    layout.elements.push_back({ 1, 2, 0x1406, 3*sizeof(float) });       // uv @ offset 12
    layout.elements.push_back({ 2, 3, 0x1406, 5*sizeof(float) });       // normal @ offset 20
    layout.stride = 8 * sizeof(float);
    auto shader = new Six::SShader((Six::SGetEngineDllDir()/"assets"/"DefaultShader.vert").string(), (Six::SGetEngineDllDir()/"assets"/"DefaultShader.frag").string());
    auto mesh = new Six::SMesh(layout, m_vertices, m_indices);
    // 场景名直接用 proj 的 "Entry Point"(proj 里写什么就是什么, 不做回退兜底)
    m_scene = Six::SEntity::Create(entry,Six::SEntityType::Scene);
    // 场景要先挂到 root 并设为 active,基类 SApplication::OnInit() 会去 Init 它
    AddSceneToRoot(m_scene);
    SetActiveScene(entry);
    m_scene->CreateSystem<Six::SProcessClose>();
    auto square = m_scene->CreateChild("Square");
    auto camera = m_scene->CreateChild("Main Camera");
    auto light  = m_scene->CreateChild("Sun");
    {
        auto dirLight = light->CreateComponent<Six::SDirLight>();
        dirLight->dir = glm::normalize(glm::vec3(0.5f, 0.8f, 0.5f));
        dirLight->color = glm::vec3(1.f, 1.f, 1.f);
        dirLight->intensity = 5.f;
        auto transform = light->CreateComponent<Six::STransform>();
        transform->position = {1.f,1.f,1.f};
    }
    {
        auto transform = square->CreateComponent<Six::STransform>();
        transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
        square->CreateComponent<Six::SRenderRes>(mesh, Six::SMaterial::MakeDefault(shader));
        square->CreateProgram<MyProgram>();
    }
    {
        auto transform = camera->CreateComponent<Six::STransform>();
        transform->position = glm::vec3(0.0f, 0.0f, -3.0f);
        transform->rotation = glm::radians(glm::vec3(0.f, 0.f, -1.f));
        auto camcom = camera->CreateComponent<Six::SCamera>();
        camcom->is_main = true;
    }
	m_scene->CreateSystem<Six::SRenderSystem>();
	// 基类 OnInit 负责 m_activeScene->Init(),这里不要再手动 Init,否则会 Init 两次
	ok &= Six::SApplication::OnInit();
    return ok;
}

void Game::OnShutdown() {
	m_win->OnShutdown();
	m_scene->End();
	m_scene->Delete();
}

bool Game::ShouldClose() {
	return m_win->ShouldClose() || !m_running;
}
