#pragma once

#include "Renderer/RendererAPI.h"


namespace Kvazar {

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		static void Init();
		static void Shutdown();
		static void BeginScene();
		static void EndScene();
		static void BeginRendering();
		static void EndRendering();
		static void ClearImage();

		static void		SetApiType(ApiType type)	{ s_RendererAPI->SetApiType(type);	}
		static ApiType	GetApiType()				{ return RendererAPI::GetApiType(); }

	private:
		static RendererAPI* s_RendererAPI;
	};

}