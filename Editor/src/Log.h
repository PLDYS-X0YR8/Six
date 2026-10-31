#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "PanelBase.h"
namespace SixEditor {
	enum class SELogLevel {
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};
	struct SELogMessage {
		SELogLevel level;
		std::string content;
		std::string category;
		std::string filename;
		std::string time;
		int line;
	};
	class SELogWin :public SEPanel{
	public:
		SELogLevel m_filterLevel = SELogLevel::Debug;
		bool show_filename = false;
		bool show_line = false;
		bool show_time = false;
	public:
		static SELogWin& Get() {
			static SELogWin instance;
			return instance;
		}
		void Render() override;
		const char* GetTitle() override { return "Log"; }
		void Clear();
		void Send(SELogLevel level, const std::string& content, const std::string& category, const std::string& filename,const std::string& time, int line);
		void Send(SELogMessage message);
	private:
		SELogWin() = default;
		std::vector<SELogMessage> m_messages;
	};
}

#define SE_LOG(level, content, category) \
    Send(SE_MAKE_LOG_MESSAGE(level, content, category, __FILE__, __LINE__)())

#define SE_MAKE_LOG_MESSAGE(level, content, category, file, line) \
    []() { \
        return SELogMessage{ level, content, category, file, \
                             Six::STimer::GetNowTime().toStringTime(), line }; \
    }

/**/