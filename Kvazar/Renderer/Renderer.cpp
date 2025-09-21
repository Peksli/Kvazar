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

	void Renderer::Init()
	{
		switch (s_RendererAPI->GetApiType())
		{
		case ApiType::None:		break;
		case ApiType::OpenGL:	break;
		case ApiType::Metal:	break;
		case ApiType::DX:		break;
		case ApiType::Vulkan:	s_RendererAPI = new VulkanRendererAPI(); s_RendererAPI->Init(); break;
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
		s_RendererAPI->BeginFrame(); // acquire -> signal acquire sem
	}

	void Renderer::BeginRendering()
	{
		s_RendererAPI->BeginCommandBuffer(); // begin cmd buffer
	}

	void Renderer::EndRendering()
	{
		s_RendererAPI->ExecuteCommandBuffer(); // end + submit command buffer
	}

	void Renderer::EndScene()
	{
		s_RendererAPI->EndFrame(); // present result
	}

	void Renderer::ClearImage()
	{
		s_RendererAPI->ClearImage();
	}

}