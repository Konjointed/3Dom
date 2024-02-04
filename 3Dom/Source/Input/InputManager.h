#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL_keycode.h>

#include <unordered_map>

struct KeyPressEvent {
	SDL_Keycode m_keycode;
};

struct KeyReleaseEvent {
	SDL_Keycode m_keycode;
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
private:
	void onKeyPressed(const KeyPressEvent& event);
	void onKeyReleased(const KeyReleaseEvent& event);
	//void onMouseButtonPressed();
	//void onMouseButtonReleased();
	//void onMouseMoved();
private:
	int m_currentMouseX, m_currentMouseY;
	int m_previousMouseX, m_previousMouseY;

	std::unordered_map<int, bool> m_keyState;
	std::unordered_map<int, bool> m_buttonState;
};

extern InputManager gInputManager;

#endif 