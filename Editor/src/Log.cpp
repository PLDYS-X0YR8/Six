#include "Log.h"

namespace SixEditor {
	void SELogWin::Render() {
		if(m_open){
			if (ImGui::Begin("Log", &m_open))
			{
				static std::string filter;
				ImGui::SetNextItemWidth(-150.0f);
				ImGui::InputTextWithHint("##ContentFilter", "filter text", &filter);
				const char* items[] = { "Debug","Info","Warning","Error","Fatal" };
				static int current_item = 0;
				ImGui::SameLine();
				ImGui::TextUnformatted("log type:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(80.f);
				ImGui::Combo("##logType", &current_item, items, IM_ARRAYSIZE(items));
				m_filterLevel = static_cast<SELogLevel>(current_item);
				ImGui::Checkbox("Show Filename", &show_filename);ImGui::SameLine();
				ImGui::Checkbox("Show Time", &show_time);ImGui::SameLine();
				ImGui::Checkbox("Show Line", &show_line);ImGui::SameLine();
				if (ImGui::Button("Clear", ImVec2(80, 0)))
				{
					Clear();
				}
				ImGui::Separator();
				int columnCount = 1;
				if (show_time)     ++columnCount;
				if (show_filename) ++columnCount;
				if (show_line)     ++columnCount;

				ImVec2 tableSize(0, -FLT_MIN);
				if (ImGui::BeginTable("##LogMessages", columnCount,
					ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY,
					tableSize))
				{
					if (show_time)     ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 70.0f);
					ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
					if (show_filename) ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_WidthFixed, 120.0f);
					if (show_line)     ImGui::TableSetupColumn("Line", ImGuiTableColumnFlags_WidthFixed, 50.0f);

					// 表头（可选，但建议加）
					ImGui::TableHeadersRow();
					for (const auto& msg : m_messages) {
						if (!filter.empty() && msg.content.find(filter) == std::string::npos)
						{
							continue;
						}
						if ((int)msg.level < (int)m_filterLevel)
						{
							continue;
						}
						ImGui::TableNextRow();
						int idx = 0;

						if (show_time) {
							ImGui::TableSetColumnIndex(idx++);
							ImGui::TextUnformatted(msg.time.c_str());
						}

						ImGui::TableSetColumnIndex(idx++);
						ImGui::TextUnformatted(msg.content.c_str());

						if (show_filename) {
							ImGui::TableSetColumnIndex(idx++);
							ImGui::TextUnformatted(msg.filename.c_str());
						}

						if (show_line) {
							ImGui::TableSetColumnIndex(idx++);
							ImGui::Text("%d", msg.line);   // ← 用 Text 而不是 to_string
						}
					}

					ImGui::EndTable();
				}
			}
			ImGui::End();
		}
	}

	void SELogWin::Clear() {
		m_messages.clear();
	}

	void SELogWin::Send(SELogLevel level, const std::string& content, const std::string& category, const std::string& filename, const std::string& time, int line) {
		SELogMessage message{ level, content, category, filename, time, line };
		m_messages.push_back(message);
	}

	void  SELogWin::Send(SELogMessage message) {
		m_messages.push_back(message);
	}
}