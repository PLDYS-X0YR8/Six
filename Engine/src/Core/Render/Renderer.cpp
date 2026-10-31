#include "spch.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Window.h"
#include "Core/Assets/FileSystem.h"
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace Six {
    void SRenderer::SetWindow(int width, int height, std::string title)
    {
        bool ok = true;
        m_window.reset(new SWindow(width, height, title, ok));
    }
    void SRenderer::SetWindow(SWindow* w)
    {
        m_window.reset(w);
    }

    bool SRenderer::Init() {
		m_renderApi.reset(SRenderAPI::Create());
        return m_renderApi->Init();
    }

    void SRenderer::Draw(SCamInfo& cinfo,SLightInfo& linfo) {
        if (!m_renderApi) {
            m_renderApi.reset(SRenderAPI::Create());
        }
        m_renderApi->SetClearColor(m_window->GetData().clearColor.x, m_window->GetData().clearColor.y, m_window->GetData().clearColor.z, m_window->GetData().clearColor.a);
        if (m_window) {
            const auto& d = m_window->GetData();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, d.width, d.height);
        }
        m_renderApi->Clear();
        for (auto& command : m_commands) {
            if (!command.material || !command.mesh) continue;
            command.material->Bind();
            command.material->shader->SetUniform("uTransform", command.transform);
            command.material->shader->SetUniform("uProjection", cinfo.projection);
            command.material->shader->SetUniform("uView", cinfo.view);
            command.material->shader->SetUniform("uDLightCount", (int)linfo.dirLights.size());
            command.material->shader->SetUniform("uViewPos", cinfo.pos);
            for (size_t i = 0; i < linfo.dirLights.size(); i++) {
                command.material->shader->SetUniform("uDLights["+std::to_string(i) + "].direction", linfo.dirLights[i]->dir);
                command.material->shader->SetUniform("uDLights["+std::to_string(i) + "].intensity", linfo.dirLights[i]->intensity);
                command.material->shader->SetUniform("uDLights["+std::to_string(i) + "].color", linfo.dirLights[i]->color);
            }
            command.mesh->Bind();
            command.mesh->Draw();
        }
        // Draw 不清 m_commands — 让后续 DrawTo 清(避免双画丢失)
    }

    void SRenderer::DrawTo(SViewport* vp, SCamInfo& cam, SLightInfo& light)
    {
        if (!m_renderApi) m_renderApi.reset(SRenderAPI::Create());
        glm::vec4 cc = m_window ? m_window->GetData().clearColor : glm::vec4(1.f, 1.f, 1.f, 1.f);
        m_renderApi->SetClearColor(cc.x, cc.y, cc.z, cc.w);

        // projection 用局部副本: DrawTo 不应该改调用方传进来的 cinfo
        glm::mat4 proj = cam.projection;

        if (vp) {
            vp->BindViewport();
            // 无条件按 FBO 实际宽高比重算 projection ——
            // Camera::GetProjectMat() 是按 window 尺寸算的,两者不一定一致
            if (vp->GetWidth() > 0 && vp->GetHeight() > 0) {
                float vpAspect = (float)vp->GetWidth() / (float)vp->GetHeight();
                proj = glm::perspective(glm::radians(cam.fov), vpAspect, cam.nearPlane, cam.farPlane);
            }
        } else if (m_window) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, m_window->GetData().width, m_window->GetData().height);
        }

        m_renderApi->Clear();

        for (auto& command : m_commands) {
            if (!command.material || !command.mesh) continue;
            command.material->Bind();
            command.material->shader->SetUniform("uTransform", command.transform);
            command.material->shader->SetUniform("uProjection", proj);
            command.material->shader->SetUniform("uView", cam.view);
            command.material->shader->SetUniform("uDLightCount", (int)light.dirLights.size());
            command.material->shader->SetUniform("uViewPos", cam.pos);
            for (size_t i = 0; i < light.dirLights.size(); i++) {
                command.material->shader->SetUniform("uDLights[" + std::to_string(i) + "].direction", light.dirLights[i]->dir);
                command.material->shader->SetUniform("uDLights[" + std::to_string(i) + "].intensity", light.dirLights[i]->intensity);
                command.material->shader->SetUniform("uDLights[" + std::to_string(i) + "].color", light.dirLights[i]->color);
            }
            command.mesh->Bind();
            command.mesh->Draw();
        }

        if (vp) vp->UnbindViewport();
        m_commands.clear();
    }

    void SRenderer::ClearDefaultFrameBuffer() {
        if (!m_renderApi) {
            m_renderApi.reset(SRenderAPI::Create());
        }
        if (!m_window) return;
        const auto& d = m_window->GetData();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, d.width, d.height);
        const glm::vec4& c = d.clearColor;
        m_renderApi->SetClearColor(c.x, c.y, c.z, c.w);
        m_renderApi->Clear();
    }

    void SRenderer::Submit(SRenderCommand& command)
    {
        m_commands.push_back(command);
    }

    void SRenderer::Shutdown() {
    }
}