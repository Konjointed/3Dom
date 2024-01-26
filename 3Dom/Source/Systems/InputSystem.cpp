#include "InputSystem.h"

#include <iostream>

#include "../EntityManager.h"
#include "../Components.h"

void InputSystem::update(EntityManager& entityManager, float timestep)
{
    SingletonInputComponent& singletonInput = entityManager.getSingletonInput();

    for (auto& entity : entityManager.queryEntitiesWith<KeyEventComponent>()) {
        KeyEventComponent& keyEvent = entityManager.getComponent<KeyEventComponent>(entity);

        singletonInput.setKeyState(keyEvent.key, keyEvent.pressed);

        entityManager.destroyEntity(entity);
    }

    for (auto& entity : entityManager.queryEntitiesWith<MouseButtonEventComponent>()) {
        MouseButtonEventComponent& mouseButtonEvent = entityManager.getComponent<MouseButtonEventComponent>(entity);

        // You need to define the methods for handling mouse button events in SingletonInputComponent
        singletonInput.setMouseButtonState(mouseButtonEvent.button, mouseButtonEvent.pressed, mouseButtonEvent.x, mouseButtonEvent.y);

        entityManager.destroyEntity(entity);
    }

    for (auto& entity : entityManager.queryEntitiesWith<MouseMotionEventComponent>()) {
        MouseMotionEventComponent& mouseMotionEvent = entityManager.getComponent<MouseMotionEventComponent>(entity);

        // You need to define the methods for handling mouse motion events in SingletonInputComponent
        singletonInput.setMousePosition(mouseMotionEvent.x, mouseMotionEvent.y);
        singletonInput.setMouseMotion(mouseMotionEvent.xrel, mouseMotionEvent.yrel);

        entityManager.destroyEntity(entity);
    }
}
