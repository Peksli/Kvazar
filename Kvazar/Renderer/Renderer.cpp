#include "Renderer/Renderer.h"

#include "Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

#include "Core/LogSystem.h"


namespace Kvazar {

	Kvazar::RendererAPI* Renderer::s_RendererAPI = nullptr;

	Renderer::Renderer()
	{
		KVAZAR_DEBUG("[Renderer] Renderer() called");
	}

	Renderer::~Renderer()
	{
		KVAZAR_DEBUG("[Renderer] ~Renderer() called");
	}

	void Renderer::Init(ApiType type)
	{
		switch (type)
		{
		case ApiType::None:		break;
		case ApiType::OpenGL:	break;
		case ApiType::Metal:	break;
		case ApiType::DX:		break;
		case ApiType::Vulkan: RendererAPI::SetApiType(type); s_RendererAPI = new VulkanRendererAPI(); break;
		}
	}

	void Renderer::Shutdown()
	{
		KVAZAR_DEBUG("[Renderer] Shutdown() called");
		s_RendererAPI->Shutdown();
		delete s_RendererAPI;
	}

	void Renderer::BeginScene()
	{

	}

	void Renderer::BeginRendering()
	{

	}

	void Renderer::EndRendering()
	{

	}

	void Renderer::EndScene()
	{

	}

}