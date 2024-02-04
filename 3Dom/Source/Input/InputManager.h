#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <unordered_map>

#include <SDL2/SDL_keycode.h>
#include <SDL_mouse.h>

#include "Event/Event.h"

struct UpdateEvent {
	float m_timestep;
};

struct KeyPressEvent {
	SDL_Keycode m_keycode;
};

struct KeyReleaseEvent {
	SDL_Keycode m_keycode;
};

struct ButtonPressEvent {
	int m_button;
};

struct ButtonReleaseEvent {
	int m_button;
};

struct MouseMoveEvent {
	int m_mouseX;
	int m_mouseY;
};

class InputManager {
public:
	void StartUp();
	void ShutDown();

	void Update();

	bool IsKeyDown(int key);
	bool IsMouseButtonDown(int button);

	int DeltaMouseX();
	int DeltaMouseY();

	Event m_luaKeyPressEvent;
	Event m_luaKeyReleaseEvent;
	Event m_luaMouseButtonPressEvent;
	Event m_luaMouseButtonReleaseEvent;
	Event m_luaMouseMoveEvent;
	Event m_luaUpdateEvent;
private:
	void onKeyPressed(const KeyPressEvent& event);
	void onKeyReleased(const KeyReleaseEvent& event);
	void onMouseButtonPressed(const ButtonPressEvent& event);
	void onMouseButtonReleased(const ButtonReleaseEvent& event);
	void onMouseMoved(const MouseMoveEvent& event);
	void onUpdate(const UpdateEvent& event); // TODO: Move out of input. Perhaps ScriptManager could handle events regarding lua?
private:
	int m_currentMouseX, m_currentMouseY;
	int m_previousMouseX, m_previousMouseY;

	std::unordered_map<int, bool> m_keyState;
	std::unordered_map<int, bool> m_buttonState;
};

extern InputManager gInputManager;

#endif 