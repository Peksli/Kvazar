#include "Platform/Windows/WindowsWindow.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	WindowsWindow::WindowsWindow(int width, int height, const char* title)
		: Window(width, height, title)
	{
		KVAZAR_DEBUG("[Windows Window] WindowsWindow() called");

		Create();
	}

	WindowsWindow::~WindowsWindow()
	{
		KVAZAR_DEBUG("[Windows Window] ~WindowsWindow() called");

		m_Context->Shutdown();
		m_Context.reset();
	}

	void WindowsWindow::Create()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		m_WindowData.window = glfwCreateWindow(m_WindowData.m_Width, m_WindowData.m_Height, m_WindowData.m_Title, nullptr, nullptr);

		if (!m_WindowData.window)
		{
			KVAZAR_CRITICAL("Failed to create GLFW window!");
			glfwTerminate();
			return;
		}

		ContextSpec spec;
		spec.m_Window = m_WindowData.window;

		m_Context = ContextCreator::Create(spec);
		m_Context->Setup();
	}

}
