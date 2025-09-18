#pragma once

#include "Platform/Windows/WindowsWindow.h"

#include <memory>


namespace Kvazar {

	class Application {
	public:
		Application();
		Application(Application&&) = delete;
		Application(const Application&) = delete;
		Application& operator=(Application&&) = delete;
		Application& operator=(const Application&) = delete;

		virtual ~Application();

		void Init();
		void Run();
		void Shutdown();

	private:
		std::shared_ptr<WindowsWindow> m_WindowsWindow = nullptr;
	};

	Application* CreateApplication();

}