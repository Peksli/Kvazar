#pragma once

#include "Platform/Window.h"
#include "Renderer/GraphicsContext.h"

#include <memory>


namespace Kvazar {

	class WindowsWindow : public Window 
	{
	public:
		WindowsWindow(int width, int height, const char* title);
		~WindowsWindow();

		void Create() override;
		void Run() override;
		void Shutdown() override;

	private:
		std::shared_ptr<GraphicsContextBuilder> m_ContextBuilder;
	};

}