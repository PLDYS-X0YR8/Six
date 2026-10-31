#pragma once
#include "spch.h"
#include "Component.h"
#include "System.h"
#include "Program.h"
namespace Six {
    enum class SEntityType {
        GameObject,
        Prefab,
        Scene //Only System (NO COMPONENTS)
    };
    class SIXAPI SEntity {
    public:
        SEntity() = default;
        SEntity(std::string name);
        SEntity(const SEntity&) = delete;
        SEntity& operator=(const SEntity&) = delete;
        SEntity(SEntity&&) = default;
        SEntity& operator=(SEntity&&) = default;
        bool IsActive() const { return m_active; }
        bool IsEnable() const { return m_enable; }
        void MarkToDisable() { m_active = false; }
        void SetEnable(bool enabled) { m_enable = enabled; }
        const std::string& GetName() const { return m_name; }
        template <typename T, typename... Args,
            typename = std::enable_if_t<std::is_base_of_v<SComponent, T>>>
        T* CreateComponent(Args&&... args) {
            if (m_type == SEntityType::Scene) { return nullptr; }
            for (auto& com : m_components) {
                if (com->is<T>()) {
                    return static_cast<T*>(com.get());
                }
            }

            auto uPtr = std::make_unique<T>(std::forward<Args>(args)...);
            T* rawPtr = uPtr.get();
            rawPtr->m_owner = this;
            m_components.push_back(std::move(uPtr));
            return rawPtr;
        }
        void DeleteComponent(SComponent* com);

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SComponent, T>>>
        T* GetComponent() {
            if (m_type == SEntityType::Scene) { return nullptr; }
            for (auto& com : m_components) {
                if (com->is<T>()) {
                    return static_cast<T*>(com.get());
                }
            }

            return nullptr; // 没找到返回空指针
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SComponent, T>>>
        T* GetComponent() const{
            if (m_type == SEntityType::Scene) { return nullptr; }
            for (auto& com : m_components) {
                if (com->is<T>()) {
                    return static_cast<T*>(com.get());
                }
            }

            return nullptr; // 没找到返回空指针
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SComponent, T>>>
        bool HasComponent() {
            return GetComponent<T>() != nullptr;
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SComponent, T>>>
        bool HasComponent() const{
            return GetComponent<T>() != nullptr;
        }

        template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<SSystem, T>>>
        T* CreateSystem() {

            if (m_type != SEntityType::Scene) { return nullptr; }
            for (auto& sys : m_systems) {
                if (sys->is<T>()) {
                    return static_cast<T*>(sys.get());
                }
            }

            auto uPtr = std::make_unique<T>();
            T* rawPtr = uPtr.get();
            rawPtr->m_owner = this;
            m_systems.push_back(std::move(uPtr));
            return rawPtr;
        }
        void DeleteSystem(SSystem* sys);

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SSystem, T>>>
        T* GetSystem() {
            if (m_type != SEntityType::Scene) { return nullptr; }
            for (auto& sys : m_systems) {
                if (sys->is<T>()) {
                    return static_cast<T*>(sys.get());
                }
            }

            return nullptr; // 没找到返回空指针
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SSystem, T>>>
        T* GetSystem() const {
            if (m_type != SEntityType::Scene) { return nullptr; }
            for (auto& sys : m_systems) {
                if (sys->is<T>()) {
                    return static_cast<T*>(sys.get());
                }
            }

            return nullptr; // 没找到返回空指针
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SSystem, T>>>
        bool HasSystem() {
            return GetSystem<T>() != nullptr;
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SSystem, T>>>
        bool HasSystem() const {
            return GetSystem<T>() != nullptr; // 没找到返回 false
        }
        template <typename T, typename... Args,
            typename = std::enable_if_t<std::is_base_of_v<SProgram, T>>>
        T* CreateProgram(Args&&... args) {
            if (m_type == SEntityType::Scene) { return nullptr; }
            for (auto& prg : m_programs) {
                if (prg->is<T>()) {
                    return static_cast<T*>(prg.get());
                }
            }

            auto uPtr = std::make_unique<T>(std::forward<Args>(args)...);
            T* rawPtr = uPtr.get();
            rawPtr->m_owner = this;
            m_programs.push_back(std::move(uPtr));
            return rawPtr;
        }
        void DeleteProgram(SProgram* prg);

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SProgram, T>>>
        T* GetProgram() {
            if (m_type == SEntityType::Scene) { return nullptr; }
            for (auto& prg : m_programs) {
                if (prg->is<T>()) {
                    return static_cast<T*>(prg.get());
                }
            }

            return nullptr; // 没找到返回空指针
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SProgram, T>>>
        T* GetProgram() const {
            if (m_type == SEntityType::Scene) { return nullptr; }
            for (auto& prg : m_programs) {
                if (prg->is<T>()) {
                    return static_cast<T*>(prg.get());
                }
            }

            return nullptr; // 没找到返回空指针
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SProgram, T>>>
        bool HasProgram() {
            return GetProgram<T>() != nullptr; // 没找到返回 false
        }
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SProgram, T>>>
        bool HasProgram() const {
            return GetProgram<T>() != nullptr;
        }
        template <typename Predicate>
        std::vector<SEntity*> Filter(Predicate predicate)
        {
            std::vector<SEntity*> res;
            for (auto& up : m_children)
            {
                // 注意: unique_ptr 的 operator bool 是 explicit 的,
                // 不能参与内置 && 的重载决议(需要隐式转换序列) -> 必须用 .get()
                if (up.get() != nullptr && predicate(*up))
                {
                    res.push_back(up.get());
                }
            }
            return res;
        }
        template <typename Predicate>
        std::vector<SEntity*> Filter(Predicate predicate) const
        {
            std::vector<SEntity*> res;
            for (auto& up : m_children)
            {
                // 注意: unique_ptr 的 operator bool 是 explicit 的,
                // 不能参与内置 && 的重载决议(需要隐式转换序列) -> 必须用 .get()
                if (up.get() != nullptr && predicate(*up))
                {
                    res.push_back(up.get());
                }
            }
            return res;
        }
        bool Init();
        void Update();
        void End();
        SEntity* GetParent() const;
        SEntity* GetChild(int index) const;
        SEntity* GetChild(std::string name) const;
        void SetParent(SEntity* p);
        void AddChild(SEntity* c);
        bool IsAncestor(SEntity* ancestor, SEntity* descendant) const;
        static SEntity* Create(const std::string& name, SEntityType type = SEntityType::GameObject);
        SEntity* CreateChild(const std::string& name, SEntityType type = SEntityType::GameObject);
        SEntity* CreateParent(const std::string& name, SEntityType type = SEntityType::GameObject);
        void Delete();
        void Delete(const std::string& name);
        SEntityType GetType() { return m_type; }
        std::vector<SEntity*> GetChildren() const;
        std::vector<SEntity*> GetChildren();
    private:
        // 从父节点摘出自己,但【不】用 erase:
        // erase 会把后面的兄弟前移并析构最后一个 -> 误删兄弟;而且 erase 位置上的
        // unique_ptr 析构会把 this 也 delete 掉 -> 之后再 push_back(this) 就是双重所有权
        void DetachFromParent();
        SEntityType m_type = SEntityType::GameObject;
        SEntity* m_parent = nullptr;
        std::vector<std::unique_ptr<SEntity>> m_children;
        std::vector<std::unique_ptr<SComponent>> m_components;
        std::vector<std::unique_ptr<SProgram>> m_programs;
        std::vector<std::unique_ptr<SSystem>> m_systems;
        std::string m_name;
        bool m_active = true;
        bool m_enable = true;
    };
}