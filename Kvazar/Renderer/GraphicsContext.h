#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <cstdint>


namespace Kvazar {

	struct ContextSpec
	{
		GLFWwindow* m_Window = nullptr;
	};


	class Context
	{
	public:
		Context() = default;
		virtual ~Context() = default;

		virtual void Setup() = 0;
		virtual void Shutdown() = 0;

	private:
		GLFWwindow* m_Window = nullptr;
	};


	class ContextCreator
	{
	public:
		ContextCreator();
		virtual ~ContextCreator();

		static std::shared_ptr<Context> Create(const ContextSpec& spec);
	};

}