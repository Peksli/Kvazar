#pragma once

#define BIT(x) (1 << (x))
#define IMPL_EVENT(event, group) static EventType GetStaticType() {return EventType::event;} \
								 inline virtual EventType GetEventType() const override {return EventType::event;} \
								 inline virtual EventGroup GetEventGroup() const override {return EventGroup::group;}
								
namespace Kvazar {

	class Event 
	{
	public:
		enum class EventType 
		{
			WindowClose, WindowMoved, WindowResize, WindowFocus, WindowUnfocus,
			KeyPressed, KeyReleased, KeyTyped,
			MouseButtonPressed, MouseButtonHold, MouseButtonReleased, MouseMoved, MouseScrolled
		};

		enum class EventGroup : uint32_t 
		{
			Application = BIT(0),
			Keyboard	= BIT(1),
			Mouse		= BIT(2)
		};

		inline virtual EventType GetEventType() const = 0;
		inline virtual EventGroup GetEventGroup() const = 0;
		inline bool IsInGroup(EventGroup group) { return static_cast<uint32_t>(GetEventGroup()) & static_cast<uint32_t>(group); }

	protected:
		bool m_Handled = false;
	};


	class EventDispatcher 
	{
		template<typename T>
		using EventCallbackFn = std::function<bool(T*)>;
	public:
		EventDispatcher(Event* event)
			:m_Event(event) { }

		template<typename T>
		bool Dispatch(EventCallbackFn<T> callbackFn) {
			if (m_Event->GetEventType() == T::GetStaticType()) {
				return callbackFn(static_cast<T*>(m_Event));
			}
		}

	private:
		Event* m_Event;
	};

}