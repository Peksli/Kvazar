#pragma once

#include <GLFW/glfw3.h>


namespace Kvazar {

	class Window {
	public:
		Window(int width, int height, const char* title);
		virtual ~Window() = default;

		virtual void Create() = 0;

		struct GLFWwindow* GetGLFWWindow() const { return m_WindowData.window; }
		int GetWidth() const { return m_WindowData.m_Width;  }
		int GetHeight() const { return m_WindowData.m_Height; }
		const char* GetTitle() const { return m_WindowData.m_Title; }

	protected:
		struct WindowData 
		{
			struct GLFWwindow* window = nullptr;	
			int m_Width = 0;
			int m_Height = 0;
			const char* m_Title = nullptr;
		} m_WindowData;
	};

}