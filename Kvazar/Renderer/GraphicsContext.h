#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <cstdint>


namespace Kvazar {

	struct GraphicsContextSpecification
	{
		GLFWwindow* m_Window = nullptr;
		uint16_t m_FramesInFlight = 1;
	};

	class GraphicsContextBuilder
	{
	public:
		GraphicsContextBuilder() = default;
		virtual ~GraphicsContextBuilder() = default;

		virtual void Build() = 0;
		virtual void Destroy() = 0;

		static std::shared_ptr<GraphicsContextBuilder> GetTypeOfContext(const GraphicsContextSpecification& specification);

	protected:
		GLFWwindow* m_Window = nullptr;
	};

}