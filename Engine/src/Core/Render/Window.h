#pragma once
#include "spch.h"
#include <glm/glm.hpp>

struct GLFWwindow;
namespace Six {
class SGraphicsContext;
	class SIXAPI SWindow {
	private:
		struct SWindowData {
			int width;
			int height;
			std::string title;
			bool isvsync = false;   // ★ 以前没初始化, IsVsync() 返回的是垃圾值
			glm::vec4 clearColor{ 1.f,1.f,1.f,1.f };
		};
	public:
		~SWindow();
		SWindow(int w,int h,std::string t,bool& ok);
		bool OnInit();
		void OnUpdate();
		void OnShutdown();
		bool ShouldClose();

		bool IsVsync();
		void SetVsync(bool enable);
		void SetWidth(int w);
		void SetHeight(int h);
		void SetTitle(std::string t);
		const SWindowData& GetData() const;
		const SWindowData& GetData();

		inline GLFWwindow* GetNativeWindow() {
			return m_window;
		}
	private:
		GLFWwindow* m_window = nullptr;
		SGraphicsContext* m_context = nullptr;
		SWindowData m_data;
	};
}