#pragma once
#include "Event.h"
#include "EventQueue.h"
#include "spch.h"


namespace Six {
    struct SIXAPI SAxis {
        std::string name;
        SCodeButton negativeButton;
        SCodeButton positiveButton;
        SCodeButton alt_negativeButton;
        SCodeButton alt_positiveButton;
        float gravity, sensivity;
        bool snap;
        double calc;
    };
	class SIXAPI SInput {
	public:
		SInput() = default;
		~SInput() = default;
		SInput(const SInput&) = delete;
		SInput& operator=(const SInput&) = delete;
		static SInput& Get() {
			static SInput input;
			return input;
		}

        bool IsKeyDown(int keycode) const;
        bool IsKeyPressed(int keycode) const;
        bool IsKeyReleased(int keycode) const;
        bool IsMouseButtonDown(int button) const;
        bool IsMouseButtonPressed(int button) const;
        bool IsMouseButtonReleased(int button) const;
        bool IsKeyDown(SKeyCodes keycode) const;
        bool IsKeyPressed(SKeyCodes keycode) const;
        bool IsKeyReleased(SKeyCodes keycode) const;
        bool IsMouseButtonDown(SMouseButton button) const;
        bool IsMouseButtonPressed(SMouseButton button) const;
        bool IsMouseButtonReleased(SMouseButton button) const;

        std::pair<float,float> GetMousePosition() const { return m_MousePos; }
        std::pair<float, float> GetMouseScroll() const { return m_MouseScroll; }

        void Update();
        void EndFrame();
        double GetAxis(std::string name) const;
        std::vector<SAxis*> GetAxises() const;
        void AddAxis(std::string name,SAxis a);
        void DelAxis(std::string name);
    private:
        bool m_KeysDown[512] = { false };
        bool m_KeysPressed[512] = { false };
        bool m_KeysReleased[512] = { false };

        bool m_MouseDown[8] = { false };
        bool m_MousePressed[8] = { false };
        bool m_MouseReleased[8] = { false };

        std::pair<float,float> m_MousePos = { 0.0f, 0.0f };
        std::pair<float, float> m_LastMousePos = { 0.0f, 0.0f };
        std::pair<float, float> m_MouseScroll = { 0.0f,0.0f };
        std::vector<SAxis*> m_axises;
	};
}