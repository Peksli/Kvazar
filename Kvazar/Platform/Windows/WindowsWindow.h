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

	private:
		std::shared_ptr<Context> m_Context;
	};

}