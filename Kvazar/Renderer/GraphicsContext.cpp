#include "Renderer/GraphicsContext.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Renderer/Renderer.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	ContextCreator::ContextCreator()
	{
		KVAZAR_DEBUG("[ContextCreator] ContextCreator()...");
	}

	ContextCreator::~ContextCreator()
	{
		KVAZAR_DEBUG("[ContextCreator] ~ContextCreator()...");
	}

	std::shared_ptr<Context> ContextCreator::Create(const ContextSpec& spec)
	{
		switch (Renderer::GetApiType())
		{
		case ApiType::None:		KVAZAR_CRITICAL("Didn't select any API");		break;
		case ApiType::Vulkan:	return std::make_shared<VulkanContext>(spec);	break;
		default:				KVAZAR_CRITICAL("Incorrect api type!!!");
		}

		return nullptr;
	}

}