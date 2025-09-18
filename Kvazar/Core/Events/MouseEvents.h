#pragma once

#include "Events/Event.h"


namespace Kvazar {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(uint16_t xPos, uint16_t yPos)
			: m_xPos(xPos), m_yPos(yPos) { }

		IMPL_EVENT(MouseMoved, Mouse)

	private:
		uint16_t m_xPos, m_yPos;
	};


	class MouseButtonPressedEvent : public Event {
	public:
		IMPL_EVENT(MouseButtonPressed, Mouse)
		// to be implemented ...
	private:
	};


	class MouseButtonReleasedEvent : public Event {
	public:
		IMPL_EVENT(MouseButtonReleased, Mouse)
		// to be implemented ...
	private:
	};


	class MouseScrolledEvent {
	public:
		IMPL_EVENT(MouseScrolled, Mouse)
		// to be implemented ...
	private:
	};

}