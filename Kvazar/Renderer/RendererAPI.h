#pragma once

#include <cstdint>


namespace Kvazar {

	enum class ApiType
	{
		None	= 0, 
		OpenGL	= 1,
		DX		= 2,
		Vulkan	= 3,
		Metal	= 4
	};

	class RendererAPI
	{
	public:
		RendererAPI();
		virtual ~RendererAPI();

		virtual void Init()					= 0;
		virtual void BeginFrame()			= 0;
		virtual void BeginCommandBuffer()	= 0;
		virtual void EndCommandBuffer()		= 0;
		virtual void ExecuteCommandBuffer() = 0;
		virtual void EndFrame()				= 0;
		virtual void Shutdown()				= 0;

		virtual void ClearImage()			= 0;

		static void SetApiType(ApiType type) { s_ApiType = type; }

		static ApiType GetApiType() { return s_ApiType; }

	private:
		static ApiType s_ApiType;
	};

}