#ifndef EVENT_COMPONENTS_H
#define EVENT_COMPONENTS_H

#include "../IComponent.h"

class KeyEventComponent : public IComponent {
public:
	int key;
	bool pressed;

	KeyEventComponent(int keyCode, bool isPressed) : key(keyCode), pressed(isPressed) {}
};

class MouseButtonEventComponent : public IComponent {
public:
	int button;
	int x, y;
	bool pressed;

	MouseButtonEventComponent(int btn, int posX, int posY, bool isPressed)
		: button(btn), x(posX), y(posY), pressed(isPressed) {}
};

class MouseMotionEventComponent : public IComponent {
public:
	int x, y;
	int xrel, yrel;

	MouseMotionEventComponent(int posX, int posY, int deltaX, int deltaY)
		: x(posX), y(posY), xrel(deltaX), yrel(deltaY) {}
};

#endif 