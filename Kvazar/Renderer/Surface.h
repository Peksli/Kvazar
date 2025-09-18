#pragma once


namespace Kvazar {

	class Surface
	{
	public:
		Surface() = default;
		virtual ~Surface() = default;

		virtual void Create()	= 0;
		virtual void Shutdown() = 0;
	};

}