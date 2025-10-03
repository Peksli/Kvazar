#include "Application.h"
#include "Renderer/Renderer.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	Application::Application() 
	{
		KVAZAR_DEBUG("[Application] Application()...");
	}

	Application::~Application()
	{
		KVAZAR_DEBUG("[Application] ~Application()...");
	}

	void Application::Init()
	{
		Renderer::SetApiType(ApiType::Vulkan);

		m_WindowsWindow = std::make_shared<WindowsWindow>(900, 700, "Vulkan engine"); 

		Renderer::Init();
	}

	void Application::Run()
	{
		while (!glfwWindowShouldClose(m_WindowsWindow->GetGLFWWindow()))
		{
			glfwPollEvents();

			Renderer::BeginScene();
			Renderer::BeginRendering();
			Renderer::ClearImage();
			Renderer::EndRendering();
			Renderer::EndScene();
		}
	}

	void Application::Shutdown()
	{
		Renderer::Shutdown();
	}

}