#include "Renderer/RendererAPI.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	ApiType RendererAPI::s_ApiType;

	RendererAPI::RendererAPI()
	{
		KVAZAR_DEBUG("[RendererAPI] RendererAPI() called");
	}

	RendererAPI::~RendererAPI()
	{
		KVAZAR_DEBUG("[RendererAPI] ~RendererAPI() called");
	}

}