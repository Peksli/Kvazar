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

		Shutdown();
		m_ContextBuilder->Destroy();
		m_ContextBuilder.reset();
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
			Shutdown();
			return;
		}

		GraphicsContextSpecification specification;
		specification.m_FramesInFlight = static_cast<uint16_t>(2);
		specification.m_Window = m_WindowData.window;

		m_ContextBuilder = GraphicsContextBuilder::GetTypeOfContext(specification);
		if (!m_ContextBuilder)
		{
			KVAZAR_CRITICAL("Didn't create context");
		}
		else
		{
			m_ContextBuilder->Build();
		}
	}

	void WindowsWindow::Run() 
	{
		while (static_cast<bool>(glfwWindowShouldClose(m_WindowData.window)) != true) 
		{
			glfwPollEvents();
		}
	}

	void WindowsWindow::Shutdown()
	{
		glfwTerminate();
	}

}