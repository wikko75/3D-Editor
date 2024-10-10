#ifndef _EVENTS_HPP_
#define _EVENTS_HPP_

#include <functional>

enum class EVENT_TYPE
{
    KEY_PRESSED,
    KEY_RELEASED,

    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_MOVED,
    MOUSE_SCROLLED,

    WINDOW_CLOSE,
    WINDOW_RESIZE,
    WINDOW_MOVED,
    WINDOW_IN_FOCUS,
    WINDOW_LOST_FOCUS
};


class EventDispacher;

class Event
{
public:
    friend EventDispacher;
    virtual auto getEventType() -> EVENT_TYPE = 0;
protected:
    bool m_is_handled = false;
};


class EventDispacher
{
public:
    EventDispacher(Event& event)
    : m_event{event}
    {}

    template<typename T>
    auto dispatch(std::function<bool(T&)> func) -> bool
    {
        if (m_event.getEventType() == T::staticGetEventType())
        {
            m_event.m_is_handled = func(dynamic_cast<T&>(m_event));
            return true;
        }
        return false;
    }

private:
    Event& m_event;
};



// KEYS
// ----------------------------------------------

class KeyEvent : public Event
{
public:
    auto getKeyCode() const -> int
    {
        return m_key_code;
    }

protected:
    KeyEvent(const int key_code)
    : m_key_code {key_code} 
    {};

protected:
    int m_key_code;
};


class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(const int key_code, bool repeat)
    : KeyEvent{key_code}, m_repeat{repeat}
    {}

    static auto staticGetEventType() -> EVENT_TYPE
    {
        return EVENT_TYPE::KEY_PRESSED;
    }

    virtual EVENT_TYPE getEventType() override
    {
        return staticGetEventType();
    }
    
private:
    bool m_repeat;
};


class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(const int key_code)
    : KeyEvent{key_code}
    {}


    static auto staticGetEventType() -> EVENT_TYPE
    {
        return EVENT_TYPE::KEY_RELEASED;
    }

    virtual EVENT_TYPE getEventType() override
    {
        return staticGetEventType();
    }
};

// ----------------------------------------------


// WINDOW
// ----------------------------------------------


class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(const int width, const int height)
    : m_width  {width}
    , m_height {height}
    {}

    auto getHeight() const -> int
    {
        return m_height;
    }

    auto getWidth() const -> int
    {
        return m_width;
    }


    static auto staticGetEventType() -> EVENT_TYPE
    {
        return EVENT_TYPE::WINDOW_RESIZE;
    }

    virtual EVENT_TYPE getEventType() override
    {
        return staticGetEventType();
    }

private:
    int m_width;
    int m_height;
};


class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent()
    {}

    static auto staticGetEventType() -> EVENT_TYPE
    {
        return EVENT_TYPE::WINDOW_CLOSE;
    }

    virtual EVENT_TYPE getEventType() override
    {
        return staticGetEventType();
    }
};

// ----------------------------------------------




// MOUSE
// ----------------------------------------------

class MouseButtonEvent : public Event
{
public:
    auto getMouseButton() const -> int
    {
        return m_button;
    }

protected:
    MouseButtonEvent(int button)
    : m_button {button}
    {}

protected:
    int m_button;
};


class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button)
    : MouseButtonEvent(button)
    {}

       static auto staticGetEventType() -> EVENT_TYPE
    {
        return EVENT_TYPE::MOUSE_BUTTON_PRESSED;
    }

    virtual EVENT_TYPE getEventType() override
    {
        return staticGetEventType();
    } 
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button)
    : MouseButtonEvent(button)
    {}

    static auto staticGetEventType() -> EVENT_TYPE
    {
        return EVENT_TYPE::MOUSE_BUTTON_RELEASED;
    }

    virtual EVENT_TYPE getEventType() override
    {
        return staticGetEventType();
    } 
};



class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
    : m_x {x}
    , m_y {y}
    {}

    auto getX() const -> float
    {
        return m_x;
    }

    auto getY() const -> float
    {
        return m_y;
    }

    static auto staticGetEventType() -> EVENT_TYPE
    {
        return EVENT_TYPE::MOUSE_MOVED;
    }

    virtual EVENT_TYPE getEventType() override
    {
        return staticGetEventType();
    } 


private:
    float m_x;
    float m_y;
};


class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float offset_x, float offset_y)
    : m_offset_x {offset_x}
    , m_offset_y {offset_y}
    {}


    auto getOffsetX() const -> float
    {
        return m_offset_x;
    }

    auto getOffsetY() const -> float
    {
        return m_offset_y;
    }

    static auto staticGetEventType() -> EVENT_TYPE
    {
        return EVENT_TYPE::MOUSE_SCROLLED;
    }

    virtual EVENT_TYPE getEventType() override
    {
        return staticGetEventType();
    } 


private:
    float m_offset_x;
    float m_offset_y;
};

// ----------------------------------------------

// 
#endif