#pragma once
#include "spch.h"
#include "Event.h"

namespace Six {
	class SIXAPI SEventQueue {
	public:
		SEventQueue() = default;
		~SEventQueue() = default;

		// 禁用拷贝
		SEventQueue(const SEventQueue&) = delete;
		SEventQueue& operator=(const SEventQueue&) = delete;
		static SEventQueue& Get() {
			static SEventQueue eq;
			return eq;
		}
		void Push(SEvent* e);
		void Clear();
		size_t Size();
		bool Empty();
		std::vector<const SEvent*> Filter(std::function<bool(const SEvent&)> predicate);
		template <typename T, typename = std::enable_if_t<std::is_base_of_v<SEvent, T>>>
		bool Has() {
			for (const auto& event : m_events) {
				// 复用你 SEvent 里定义的 is<T>() 方法
				if (event->is<T>()) {
					return true;
				}
			}
			return false;
		}
		const SEvent* GetTop() const;
		std::unique_ptr<SEvent> Pop();
		std::vector<const SEvent*> GetAll();
	private:
		std::vector<std::unique_ptr<SEvent>> m_events;
	};
}