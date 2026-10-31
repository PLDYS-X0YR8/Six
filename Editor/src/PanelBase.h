#pragma once

namespace SixEditor {
	class SEPanel {
	public:
		virtual void Render() = 0;
		virtual const char* GetTitle() = 0;
		virtual bool IsOpen() { return m_open; }
		virtual void SetOpen(bool open) { m_open = open; }
	protected:
		bool m_open = true;
	};
}