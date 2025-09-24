#pragma once


namespace Kvazar {

	class Swapchain
	{
	public:
		Swapchain() = default;
		virtual ~Swapchain() = default;

		virtual void BeginFrame()	= 0;
		virtual void EndFrame()		= 0;
	};

}