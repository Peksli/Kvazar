#include "Events/Event.h"


namespace Kvazar {

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(uint16_t new_width, uint16_t new_height)
			: m_newWidth(new_width), m_newHeight(new_height) { }

		IMPL_EVENT(WindowResize, Application)

	private:
		uint16_t m_newWidth;
		uint16_t m_newHeight;
	};

	
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;

		IMPL_EVENT(WindowClose, Application)
	};

}