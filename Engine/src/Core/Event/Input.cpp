#include "spch.h"
#include "Input.h"

namespace Six {
	void SInput::Update() {
		auto events = Six::SEventQueue::Get().GetAll();
		for (auto* ev:events)
		{
			if (ev->is<SKeyPressedEvent>())
			{
				auto event = static_cast<const SKeyPressedEvent*>(ev);
				if (event->GetRepeat() == 0) {
					m_KeysPressed[(int)event->GetKeyCode()] = true;
				}
				m_KeysDown[(int)event->GetKeyCode()] = true;
			}
			else if (ev->is<SKeyReleasedEvent>()) {
				auto event = static_cast<const SKeyReleasedEvent*>(ev);
				m_KeysReleased[(int)event->GetKeyCode()] = true;
			}
			else if (ev->is<SMouseButtonPressedEvent>())
			{
				auto event = static_cast<const SMouseButtonPressedEvent*>(ev);
				m_MousePressed[(int)event->GetMouseButton()] = event->GetRepeat() == 0;
				m_MouseDown[(int)event->GetMouseButton()] = true;
			}
			else if (ev->is<SMouseButtonReleasedEvent>()) {
				auto event = static_cast<const SMouseButtonReleasedEvent*>(ev);
				m_MouseReleased[(int)event->GetMouseButton()] = true;
			}
			else if (ev->is<SMouseMovedEvent>())
			{
				auto event = static_cast<const SMouseMovedEvent*>(ev);
				m_MousePos = { event->GetX(),event->GetY() };
			}
			else if (ev->is<SMouseScrolledEvent>())
			{
				auto event = static_cast<const SMouseScrolledEvent*>(ev);
				m_MouseScroll = { event->GetX(),event->GetY() };
			}
		}
	}

	void SInput::EndFrame() {
		memset(m_KeysPressed, 0, sizeof(m_KeysPressed));
		memset(m_KeysReleased, 0, sizeof(m_KeysReleased));
		memset(m_MousePressed, 0, sizeof(m_MousePressed));
		memset(m_MouseReleased, 0, sizeof(m_MouseReleased));
		m_MouseScroll = { 0.0f, 0.0f };
		m_LastMousePos = m_MousePos;
	}

	bool SInput::IsKeyDown(int keycode) const {
		if (keycode < 0 || keycode >= 512) return false;
		return m_KeysDown[keycode];
	}

	bool SInput::IsKeyPressed(int keycode) const {
		if (keycode < 0 || keycode >= 512) return false;
		return m_KeysPressed[keycode];
	}

	bool SInput::IsKeyReleased(int keycode) const {
		if (keycode < 0 || keycode >= 512) return false;
		return m_KeysReleased[keycode];
	}
	bool SInput::IsMouseButtonDown(int button) const {
		if (button < 0 || button >= 8) return false;
		return m_MouseDown[button];
	}

	bool SInput::IsMouseButtonPressed(int button) const {
		if (button < 0 || button >= 8) return false;
		return m_MousePressed[button];
	}

	bool SInput::IsMouseButtonReleased(int button) const {
		if (button < 0 || button >= 8) return false;
		return m_MouseReleased[button];
	}

	bool SInput::IsKeyDown(SKeyCodes keycode) const{
		return IsKeyDown(static_cast<int>(keycode));
	}
	bool SInput::IsKeyPressed(SKeyCodes keycode) const {
		return IsKeyPressed(static_cast<int>(keycode));
	}
	bool SInput::IsKeyReleased(SKeyCodes keycode) const {
		return IsKeyReleased(static_cast<int>(keycode));
	}
	bool SInput::IsMouseButtonDown(SMouseButton keycode) const {
		return IsMouseButtonDown(static_cast<int>(keycode));
	}
	bool SInput::IsMouseButtonPressed(SMouseButton keycode) const {
		return IsMouseButtonPressed(static_cast<int>(keycode));
	}
	bool SInput::IsMouseButtonReleased(SMouseButton keycode) const {
		return IsMouseButtonReleased(static_cast<int>(keycode));
	}
}