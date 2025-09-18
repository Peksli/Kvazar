#include "Renderer/GraphicsContext.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Renderer/Renderer.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	std::shared_ptr<GraphicsContextBuilder> GraphicsContextBuilder::GetTypeOfContext(const GraphicsContextSpecification& specification)
	{
		switch (Renderer::GetApiType())
		{
		case ApiType::None:		KVAZAR_CRITICAL("Didn't select any API"); break;
		case ApiType::Vulkan:	return std::make_shared<VulkanGraphicsContextBuilder>(specification); break;
		default: KVAZAR_CRITICAL("Undefined erorr occured in GraphicsContext::CreateContext()");
		}

		return nullptr;
	}

}