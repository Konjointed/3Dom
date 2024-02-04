#include "InputManager.h"

#include "Log/Logger.h"
#include "Event/EventManager.h"

InputManager gInputManager;

void InputManager::StartUp()
{
    gEventManager.Connect<KeyPressEvent>(this, &InputManager::onKeyPressed);
    gEventManager.Connect<KeyReleaseEvent>(this, &InputManager::onKeyReleased);
}

void InputManager::ShutDown()
{
}

void InputManager::Update()
{
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

int InputManager::DeltaMouseX()
{
    return 0;
}

int InputManager::DeltaMouseY()
{
    return 0;
}

void InputManager::onKeyPressed(const KeyPressEvent& event)
{
    m_keyState[event.m_keycode] = true;
}

void InputManager::onKeyReleased(const KeyReleaseEvent& event)
{
    m_keyState[event.m_keycode] = false;
}
