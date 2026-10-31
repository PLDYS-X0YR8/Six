#include "spch.h"
#include "Timer.h"

namespace Six {
	bool STimer::Init() {
		m_start = std::chrono::steady_clock::now();
		deltatime = 0.0f;
		nowtime = 0.0f;
		lasttime = 0.0f;
		m_counter = 0;
		return true;
	}

	void STimer::Update() {
		auto current = std::chrono::steady_clock::now();
		nowtime = std::chrono::duration<float>(current - m_start).count();
		deltatime = nowtime - lasttime;
		lasttime = nowtime;
		if (m_setfps > 0.f) {
			auto sleepMs = std::chrono::milliseconds(1000 / (int)m_setfps);
			std::this_thread::sleep_for(sleepMs);
			current = std::chrono::steady_clock::now();
			nowtime = std::chrono::duration<float>(current - m_start).count();
			deltatime = nowtime - lasttime;
			lasttime = nowtime;
		}
		if (deltatime > 1e-6f) m_fps = 1.f / deltatime;
		else m_fps = 0.f;
	}

	void STimer::Reset() {
		m_start = std::chrono::steady_clock::now();
		deltatime = 0.0f;
		nowtime = 0.0f;
		m_counter = 0;
	}

	STimeParts STimer::GetNowTime() {
		STimeParts time_parts;
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm_buf{};
#ifdef SIX_WINDOWS
		localtime_s(&tm_buf, &t);
#else
		localtime_r(&t, &tm_buf);
#endif
		time_parts.year = tm_buf.tm_year + 1900;
		time_parts.month = tm_buf.tm_mon+1;
		time_parts.day = tm_buf.tm_mday;
		time_parts.hour = tm_buf.tm_hour;
		time_parts.minute = tm_buf.tm_min;
		time_parts.second = tm_buf.tm_sec;
		time_parts.millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
		time_parts.weekday = tm_buf.tm_wday;
		time_parts.yearday = tm_buf.tm_yday;
		time_parts.isdst = tm_buf.tm_isdst;
		return time_parts;
	}
	int64_t STimer::GetTime()
	{
		auto now = std::chrono::system_clock::now();
		// 转换为自 1970-01-01 以来的毫秒数
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()
		).count();
		return ms;
	}
}