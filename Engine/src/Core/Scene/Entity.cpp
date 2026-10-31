#include "spch.h"
#include "Entity.h"

namespace Six {
	SEntity::SEntity(std::string name){
		m_name = name;
		m_enable = true;
		m_active = true;
	}

	void SEntity::DeleteComponent(SComponent* com) {
		if (m_type == SEntityType::Scene) { return; }
		auto it = std::find_if(m_components.begin(), m_components.end(),
			[com](const std::unique_ptr<SComponent>& ptr) {
				return ptr.get() == com;
			});

		if (it != m_components.end()) {
			// 1. 如果组件有特定的销毁逻辑，先手动调用
			// (*it)->OnDestroy();

			// 2. 从容器中移除。unique_ptr 离开容器时，会自动 delete 底层内存
			m_components.erase(it);
		}
	}

	void SEntity::DeleteSystem(SSystem* sys) {

		if (m_type != SEntityType::Scene) { return; }
		auto it = std::find_if(m_systems.begin(), m_systems.end(),
			[sys](const std::unique_ptr<SSystem>& ptr) {
				return ptr.get() == sys;
			});

		if (it != m_systems.end()) {
			// 1. 如果组件有特定的销毁逻辑，先手动调用
			// (*it)->OnDestroy();

			// 2. 从容器中移除。unique_ptr 离开容器时，会自动 delete 底层内存
			m_systems.erase(it);
		}
	}

	void SEntity::DeleteProgram(SProgram* prg) {
		if (m_type == SEntityType::Scene) { return; }
		auto it = std::find_if(m_programs.begin(), m_programs.end(),
			[prg](const std::unique_ptr<SProgram>& ptr) {
				return ptr.get() == prg;
			});

		if (it != m_programs.end()) {
			// 1. 如果组件有特定的销毁逻辑，先手动调用
			// (*it)->OnDestroy();

			// 2. 从容器中移除。unique_ptr 离开容器时，会自动 delete 底层内存
			m_programs.erase(it);
		}
	}


	bool SEntity::Init() {
		bool ok = true;
		if (m_type == SEntityType::Scene)
		{
			for (size_t i = 0; i < m_systems.size(); i++)
			{
				ok &= m_systems[i]->Init();
			}
		}
		else {
			for (size_t i = 0; i < m_programs.size(); i++)
			{
				ok &= m_programs[i]->Init();
			}
		}
		return ok;
	}

	void SEntity::Update() {
		if (m_type == SEntityType::Scene)
		{
			for (size_t i = 0; i < m_systems.size(); i++)
			{
				if (m_systems[i] && m_systems[i]->IsEnable()) m_systems[i]->Update();
			}
			for (size_t i = 0; i < m_children.size(); i++)
			{
				// 防御: m_children 里理论上不该有 null(DetachFromParent 会清干净),
				// 但一旦有, m_children[i]->IsEnable() 就是空指针解引用
				if (m_children[i] && m_children[i]->IsEnable()) m_children[i]->Update();
			}
		}
		else {
			for (size_t i = 0; i < m_programs.size(); i++)
			{
				if (m_programs[i] && m_programs[i]->IsEnable()) m_programs[i]->Update();
			}
		}
	}

	void SEntity::End() {
		if (m_type == SEntityType::Scene)
		{
			for (size_t i = 0; i < m_systems.size(); i++)
			{
				m_systems[i]->End();
			}
		}
		else {
			for (size_t i = 0; i < m_programs.size(); i++)
			{
				m_programs[i]->End();
			}
		}
	}

	SEntity* SEntity::GetParent() const {
		return m_parent;
	}

	SEntity* SEntity::GetChild(int index) const
	{
		return m_children[index].get();
	}

	SEntity* SEntity::GetChild(std::string name) const
	{
		// m_children 是 vector<unique_ptr<SEntity>>,lambda 必须接 const unique_ptr&
		// (按值接会触发 unique_ptr 已删除的拷贝构造)
		auto it = std::find_if(
			m_children.begin(),
			m_children.end(),
			[name](const std::unique_ptr<SEntity>& e) {
				return e && e->GetName() == name;
			}
		);
		return it != m_children.end() ? it->get() : nullptr;
	}

	void SEntity::SetParent(SEntity* p)
	{
		if (m_type == SEntityType::Scene) {
			return;
		}

		// 不能设自己为父节点
		if (p == this) {
			return;
		}

		// 如果 p 已经是自己的直接子节点，拒绝（防止直接循环）
		// m_children 持 unique_ptr,lambda 接 const unique_ptr& 再用 .get() 比较
		auto it = std::find_if(m_children.begin(), m_children.end(),
			[p](const std::unique_ptr<SEntity>& c) { return c.get() == p; });
		if (it != m_children.end()) {
			return;
		}

		// 如果 p 是自己的后代节点，拒绝（防止深层循环）
		if (p && IsAncestor(this, p)) {
			return;
		}

		// 从原父节点中摘出自己(不 erase,否则会误删兄弟 + delete this)
		DetachFromParent();

		// 设置新父节点 —— 所有权安全转移给 p
		m_parent = p;
		if (p) {
			p->m_children.push_back(std::unique_ptr<SEntity>(this));
		}
	}

	void SEntity::AddChild(SEntity* c) {
		if (!c) return;
		// SetParent 内部已经会 push_back + 转移所有权,
		// 这里再 push 一次会导致同一实体出现在两个 children 里(双重所有权)
		c->SetParent(this);
	}

	bool SEntity::IsAncestor(SEntity* ancestor, SEntity* descendant) const
	{
		if (!ancestor || !descendant) return false;
		SEntity* current = descendant->GetParent();
		while (current) {
			if (current == ancestor) return true;
			current = current->GetParent();
		}
		return false;
	}

	SEntity* SEntity::Create(const std::string& name, SEntityType type)
	{
		auto e = new SEntity();
		e->m_type = type;
		e->m_active = true;
		e->m_enable = true;
		e->m_name = name;
		e->m_parent = nullptr;
		return e;
	}

	SEntity* SEntity::CreateChild(const std::string& name, SEntityType type)
	{
		auto e = Create(name, type);
		e->SetParent(this);
		return e;
	}

	SEntity* SEntity::CreateParent(const std::string& name, SEntityType type)
	{
		auto* newParent = Create(name, type);
		SEntity* oldParent = m_parent;

		// 1. 把自己从旧父节点彻底摘干净(vector 里不能留 null)
		DetachFromParent();

		// 2. newParent 顶替"我"在旧父节点下的位置
		if (oldParent) {
			oldParent->m_children.push_back(std::unique_ptr<SEntity>(newParent));
			newParent->m_parent = oldParent;
		}

		// 3. 我挂到 newParent 下
		m_parent = newParent;
		newParent->m_children.push_back(std::unique_ptr<SEntity>(this));
		return newParent;
	}

	void SEntity::Delete()
	{
		// 1. 先把子节点全部搬出来 —— 不能在遍历 m_children 时让子节点自己改 m_children
		//    (子节点 Delete() 会把自己从父的 m_children 里摘掉 -> 迭代器失效)
		while (!m_children.empty()) {
			// ★ 必须用 release() 拿裸指针,不能用 std::move 把所有权接进局部 unique_ptr:
			//   Delete() 内部最后是 delete this,对象当场就没了。
			//   若这里用 move 接进局部 unique_ptr,离开作用域时 unique_ptr 会对同一个
			//   指针再 delete 一次 -> double free -> 堆损坏
			//   (表现:关程序时在 ~SEntity 里访问违例 0xC0000005,栈上是
			//    default_delete -> ~SEntity -> default_delete -> ~SEntity 的二次析构链)
			SEntity* child = m_children.back().release();
			m_children.pop_back();
			if (child) {
				child->m_parent = nullptr;   // 断开,让它自己负责 delete 自己
				child->Delete();
			}
		}

		// 2. 从父节点摘出自己(不 erase —— erase 会前移兄弟并析构最后一个 -> 误删兄弟)
		DetachFromParent();

		// 3. 顶层 entity (无 parent): Scene.m_entities / Game 仍持 raw
		//    这里清完 children 后 delete 自己。caller 需保证 this 不再被 raw 引用
		//    否则 Scene.m_entities 会留 dangling ptr
		delete this;
	}
	void SEntity::Delete(const std::string& name)
	{
		// m_children 是 vector<unique_ptr<SEntity>>,lambda 接 unique_ptr&
		auto it = std::find_if(
			m_children.begin(),
			m_children.end(),
			[&name](const std::unique_ptr<SEntity>& c) {
				return c && c->GetName() == name;
			}
		);
		if (it != m_children.end()) {
			// 委托给被找到的 child 的 Delete()(递归子树 + 从自己 erase + delete)
			(*it)->Delete();
		}
	}

	// 必须按值返回。返回局部 vector 的引用 = 悬垂引用:
	// 函数一返回 res 就析构了,调用方 range-for 会从已释放的堆内存读出垃圾 begin/end
	// -> 随机地"列表变空"或直接访问违例
	std::vector<SEntity*> SEntity::GetChildren() const
	{
		std::vector<SEntity*> res;
		res.reserve(m_children.size());
		for (auto& c : m_children) {
			res.push_back(c.get());
		}
		return res;
	}
	std::vector<SEntity*> SEntity::GetChildren()
	{
		std::vector<SEntity*> res;
		res.reserve(m_children.size());
		for (auto& c : m_children) {
			res.push_back(c.get());
		}
		return res;
	}

	// 从父节点摘出自己,但【不】用 erase:
	// erase 会把后面的兄弟前移并析构最后一个 -> 误删兄弟;而且 erase 位置上的
	// unique_ptr 析构会把 this 也 delete 掉 -> 之后再 push_back(this) 就是双重所有权
	void SEntity::DetachFromParent() {
		if (!m_parent) return;
		auto& siblings = m_parent->m_children;
		// 1. 放弃所有权(不 delete this)
		for (auto& s : siblings) {
			if (s.get() == this) {
				s.release();
				break;
			}
		}
		// 2. 必须把这个 null 元素真正移出 vector
		//    只 release 的话 vector 里会留下 null,后续 m_children[i]->xxx() 直接空指针崩
		//    这里用 remove+erase 而不是单个 erase:
		//    单个 erase 会把后面的兄弟前移并析构最后一个 -> 误删兄弟
		siblings.erase(std::remove(siblings.begin(), siblings.end(), nullptr), siblings.end());
		m_parent = nullptr;
	}
}