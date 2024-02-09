#include "InputManager.h"

#include "Log/Logger.h"
#include "Event/EventManager.h"

InputManager gInputManager;

void InputManager::StartUp()
{
    gEventManager.Connect<KeyPressEvent>(this, &InputManager::onKeyPressed);
    gEventManager.Connect<KeyReleaseEvent>(this, &InputManager::onKeyReleased);
    gEventManager.Connect<ButtonPressEvent>(this, &InputManager::onMouseButtonPressed);
    gEventManager.Connect<ButtonReleaseEvent>(this, &InputManager::onMouseButtonReleased);
    gEventManager.Connect<MouseMoveEvent>(this, &InputManager::onMouseMoved);
    gEventManager.Connect<MouseWheelEvent>(this, &InputManager::onMouseWheel);
}

void InputManager::ShutDown()
{
}

void InputManager::Update()
{
    m_previousMouseX = m_currentMouseX;
    m_previousMouseY = m_currentMouseY;
}

bool InputManager::IsKeyDown(int key)
{
    auto it = m_keyState.find(key);
    if (it != m_keyState.end()) {
        return it->second;
    }
    return false;
}

bool InputManager::IsMouseButtonDown(int button)
{
    auto it = m_buttonState.find(button);
    if (it != m_buttonState.end()) {
        return it->second;
    }
    return false;
}

glm::vec2 InputManager::GetMouseDelta()
{
    return glm::vec2(getDeltaMouseX(), getDeltaMouseY());
}

int InputManager::GetMouseWheelY()
{
    int value = m_mouseWheelY;
    m_mouseWheelY = 0;
    return value;
}

int InputManager::getDeltaMouseX()
{
    return m_currentMouseX - m_previousMouseX;
}

int InputManager::getDeltaMouseY()
{
    return m_currentMouseY - m_previousMouseY;
}


void InputManager::onKeyPressed(const KeyPressEvent& event)
{
    m_keyState[event.m_keycode] = true;
    m_luaKeyPressEvent(event.m_keycode);
}

void InputManager::onKeyReleased(const KeyReleaseEvent& event)
{
    m_keyState[event.m_keycode] = false;
    m_luaKeyReleaseEvent(event.m_keycode);
}

void InputManager::onMouseButtonPressed(const ButtonPressEvent& event)
{
    m_buttonState[event.m_button] = true;
}

void InputManager::onMouseButtonReleased(const ButtonReleaseEvent& event)
{
    m_buttonState[event.m_button] = false;
}

void InputManager::onMouseMoved(const MouseMoveEvent& event)
{
    m_currentMouseX = event.m_mouseX;
    m_currentMouseY = event.m_mouseY;
}

void InputManager::onMouseWheel(const MouseWheelEvent& event)
{
    m_mouseWheelY = event.m_y;
}
