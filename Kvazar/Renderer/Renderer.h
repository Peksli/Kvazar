#pragma once

#include "Renderer/RendererAPI.h"


namespace Kvazar {

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		static void Init(ApiType type);
		static void Shutdown();

		static void BeginScene();
		static void BeginRendering();
		static void EndRendering();
		static void EndScene();

		static ApiType GetApiType() { return RendererAPI::GetApiType(); }

	private:
		static RendererAPI* s_RendererAPI;
	};

}