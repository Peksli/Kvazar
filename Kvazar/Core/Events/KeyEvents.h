#pragma once

#include "Events/Event.h"


namespace Kvazar {

	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int32_t keycode, uint16_t repeatCount)
			: m_KeyCode(keycode), m_RepeatCount(repeatCount) { }

		IMPL_EVENT(KeyPressed, Keyboard)
		
		std::string ToString() const override {
			return std::string("KeyPressedEvent: ") + std::to_string(m_KeyCode) + " (" + std::to_string(m_RepeatCount) + " repeats)";
		}

		inline int32_t GetKeyCode() const { return m_KeyCode; }
		inline uint16_t GetRepeatCount() const { return m_RepeatCount; }

	private:
		int32_t m_KeyCode;
		uint16_t m_RepeatCount;
	};


	class KeyReleasedEvent : public Event {
	public:
		KeyReleasedEvent(int32_t keycode)
			: m_KeyCode(keycode) { }

		IMPL_EVENT(KeyReleased, Keyboard)
		
		std::string ToString() const override {
			return std::string("KeyReleasedEvent: ") + std::to_string(m_KeyCode);
		}

		inline int32_t GetKeyCode() const { return m_KeyCode; }

	private:
		int32_t m_KeyCode;
	};


	class KeyTypedEvent : public Event {
	public:
		KeyTypedEvent(int32_t keycode)
			: m_KeyCode(keycode) { }

		IMPL_EVENT(KeyTyped, Keyboard)
		
		std::string ToString() const override {
			return std::string("KeyTypedEvent: ") + std::to_string(m_KeyCode);
		}

		inline int32_t GetKeyCode() const { return m_KeyCode; }

	private:
		int32_t m_KeyCode;
	};
}