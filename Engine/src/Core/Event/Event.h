#pragma once
#include "spch.h"
#include "Core/Defines/AllDefines.h"
#define SIX_EVENT(cc) \
	public: \
		static size_t GetStaticTypeId() { \
			return StaticTypeId<cc>(); \
		} \
		virtual size_t GetTypeId() const override { return GetStaticTypeId(); }
namespace Six{
	class SIXAPI SEvent {
	public:
		virtual ~SEvent() = default;
		virtual size_t GetTypeId() const = 0;
		template <typename T>
		static size_t StaticTypeId() {
			static size_t id = m_nextid++;
			return id;
		}

		virtual std::string toString() const = 0;
		template <typename T>
		bool is() const { return GetTypeId() == T::GetStaticTypeId(); }
	private:
		static size_t m_nextid;
	};

	class SIXAPI SWinClosedEvent : public SEvent {
		SIX_EVENT(SWinClosedEvent)
	public:
		std::string toString() const override { return "Window Closed"; }
	};

	class SIXAPI SWinResizedEvent : public SEvent {
		SIX_EVENT(SWinResizedEvent)
	public:
		SWinResizedEvent(uint32_t width, uint32_t height) :m_width(width), m_height(height){}
		inline uint32_t GetWidth() const { return m_width; }
		inline uint32_t GetHeight() const { return m_height; }
		std::string toString() const override {
			std::stringstream ss;
			ss << "Window Resized:" << m_width << "*" << m_height;
			return ss.str();
		}
	private:
		uint32_t m_width;
		uint32_t m_height;
	};
	class SIXAPI SKeyPressedEvent : public SEvent {
		SIX_EVENT(SKeyPressedEvent)
	public:
		SKeyPressedEvent(SKeyCodes key,int repeat):m_key(key),m_repeat(repeat){}
		inline SKeyCodes GetKeyCode() const { return m_key; }
		inline int GetRepeat() const { return m_repeat; }
		std::string toString() const override {
			std::stringstream ss;
			ss << "Key Pressed: " << S_Key2Str(m_key) << "(" << m_repeat << ")";
			return ss.str();
		}
	private:
		SKeyCodes m_key;
		int m_repeat;
	};

	class SIXAPI SKeyReleasedEvent : public SEvent {
		SIX_EVENT(SKeyReleasedEvent)
	public:
		SKeyReleasedEvent(SKeyCodes key) :m_key(key){}
		inline SKeyCodes GetKeyCode() const { return m_key; }
		std::string toString() const override {
			std::stringstream ss;
			ss << "Key Released: " << S_Key2Str(m_key);
			return ss.str();
		}
	private:
		SKeyCodes m_key;
	};

	class SIXAPI SKeyTypedEvent : public SEvent {
		SIX_EVENT(SKeyTypedEvent)
	public:
		SKeyTypedEvent(uint32_t key) :m_codepoint(key) {}
		inline char GetCharCode() const { return (char)m_codepoint; }
		inline uint32_t GetCode() const { return m_codepoint; }
		std::string toString() const override {
			std::stringstream ss;
			ss << "Key Typed: " << (char)m_codepoint;
			return ss.str();
		}
	private:
		uint32_t m_codepoint;
	};
	class SIXAPI SMouseButtonPressedEvent : public SEvent {
		SIX_EVENT(SMouseButtonPressedEvent)
	public:
		SMouseButtonPressedEvent(SMouseButton mb,int repeat):m_button(mb),m_repeat(repeat){}
		inline SMouseButton GetMouseButton() const { return m_button; }
		inline int GetRepeat() const { return m_repeat; }
		std::string toString() const override {
			std::stringstream ss;
			ss << "Mouse Pressed: " << S_MouseButton2Str(m_button)<<"("<<m_repeat<<")";
			return ss.str();
		}
	private:
		SMouseButton m_button;
		int m_repeat;
	};

	class SIXAPI SMouseButtonReleasedEvent : public SEvent {
		SIX_EVENT(SMouseButtonReleasedEvent)
	public:
		SMouseButtonReleasedEvent(SMouseButton mb) :m_button(mb) {}
		inline SMouseButton GetMouseButton() const { return m_button; }
		std::string toString() const override {
			std::stringstream ss;
			ss << "Mouse Released: " << S_MouseButton2Str(m_button);
			return ss.str();
		}
	private:
		SMouseButton m_button;
	};

	class SIXAPI SMouseMovedEvent : public SEvent {
		SIX_EVENT(SMouseMovedEvent)
	public:
		SMouseMovedEvent(int X,int Y):m_x(X),m_y(Y){}
		inline int GetX() const { return m_x; }
		inline int GetY() const { return m_y; }
		std::string toString() const override {
			std::stringstream ss;
			ss << "Mouse Moved:" << m_x << "," << m_y;
			return ss.str();
		}
	private:
		int m_x;
		int m_y;
	};

	class SIXAPI SMouseScrolledEvent : public SEvent {
		SIX_EVENT(SMouseScrolledEvent)
	public:SMouseScrolledEvent(double X, double Y) :m_ox(X), m_oy(Y) {}
		inline double GetX() const { return m_ox; }
		inline double GetY() const { return m_oy; }
		std::string toString() const override {
			std::stringstream ss;
			ss << "Mouse Scrolled:" << m_ox << "," << m_oy;
			return ss.str();
		}
	private:
		double m_ox;
		double m_oy;
	};
}

