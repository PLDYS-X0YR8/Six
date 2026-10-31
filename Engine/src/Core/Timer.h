#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include "spch.h"
namespace Six{
	struct STimeParts {
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		int millisecond;
		int weekday;
		int yearday;
		int isdst;
		std::string toStringTime(const char* format = "%H:%M:%S") {
			std::tm tm_buf{};
			tm_buf.tm_year = year - 1900;
			tm_buf.tm_mon = month - 1;
			tm_buf.tm_mday = day;
			tm_buf.tm_hour = hour;
			tm_buf.tm_min = minute;
			tm_buf.tm_sec = second;
			tm_buf.tm_isdst = isdst;
			tm_buf.tm_wday = weekday;
			tm_buf.tm_yday = yearday;
			char buffer[256];
			std::strftime(buffer, sizeof(buffer), format, &tm_buf);
			return std::string(buffer);
		}
	};
	class SIXAPI STimer
	{
	public:
		void Update();
		bool Init();
		void Reset();
		void ResetCounter() {
			m_counter = 0;
		};
		int GetCounter() const {
			return m_counter;
		};
		void SetFPS(float fps) {
			m_setfps = fps;
		};
		static STimeParts GetNowTime();
		static int64_t GetTime(); //时间戳
	public:
		float deltatime = 0.1f;
		float nowtime = 0.f;
	private:
		float lasttime = 0.f;
		int m_counter = 0;
		float m_setfps = 60;
		float m_fps = 60;
		std::chrono::steady_clock::time_point m_start;
	};

	STimer* CreateTimer();
}

