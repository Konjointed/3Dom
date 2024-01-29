#include "InputSystem.h"

#include "../EntityManager.h"
#include "../Components/Event.h"
#include "../Components/SingletonInput.h"

void InputSystem::update(float timestep)
{
    SingletonInput& singletonInput = m_entityManager.getSingletonInput();

    for (auto& entity : m_entityManager.queryEntitiesWith<KeyEventComponent>()) {
        KeyEventComponent& keyEvent = m_entityManager.getComponent<KeyEventComponent>(entity);

        singletonInput.setKeyState(keyEvent.key, keyEvent.pressed);

        m_entityManager.destroyEntity(entity);
    }

    for (auto& entity : m_entityManager.queryEntitiesWith<MouseButtonEventComponent>()) {
        MouseButtonEventComponent& mouseButtonEvent = m_entityManager.getComponent<MouseButtonEventComponent>(entity);

        // You need to define the methods for handling mouse button events in SingletonInputComponent
        singletonInput.setMouseButtonState(mouseButtonEvent.button, mouseButtonEvent.pressed, mouseButtonEvent.x, mouseButtonEvent.y);

        m_entityManager.destroyEntity(entity);
    }

    for (auto& entity : m_entityManager.queryEntitiesWith<MouseMotionEventComponent>()) {
        MouseMotionEventComponent& mouseMotionEvent = m_entityManager.getComponent<MouseMotionEventComponent>(entity);

        // You need to define the methods for handling mouse motion events in SingletonInputComponent
        singletonInput.setMousePosition(mouseMotionEvent.x, mouseMotionEvent.y);
        singletonInput.setMouseMotion(mouseMotionEvent.xrel, mouseMotionEvent.yrel);

        m_entityManager.destroyEntity(entity);
    }
}
