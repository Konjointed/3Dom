#ifndef SINGLETON_INPUT_COMPONENT
#define SINGLETON_INPUT_COMPONENT

#include <unordered_map>

#include "../IComponent.h"

class SingletonInput : public IComponent {
public:
    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> mouseButtonStates;
    int mouseX, mouseY;
    int mouseXrel, mouseYrel;

    SingletonInput() : mouseX(0), mouseY(0), mouseXrel(0), mouseYrel(0) {}

    void setKeyState(int keyCode, bool isPressed) {
        keyStates[keyCode] = isPressed;
    }

    bool getKeyState(int keyCode) const {
        auto it = keyStates.find(keyCode);
        if (it != keyStates.end()) {
            return it->second;
        }
        return false;
    }

    void setMouseButtonState(int button, bool isPressed, int x, int y) {
        mouseButtonStates[button] = isPressed;
        mouseX = x;
        mouseY = y;
    }

    bool getMouseButtonState(int button) const {
        auto it = mouseButtonStates.find(button);
        if (it != mouseButtonStates.end()) {
            return it->second;
        }
        return false;
    }

    void setMousePosition(int x, int y) {
        mouseX = x;
        mouseY = y;
    }

    std::pair<int, int> getMousePosition() const {
        return { mouseX, mouseY };
    }

    void setMouseMotion(int xrel, int yrel) {
        mouseXrel = xrel;
        mouseYrel = yrel;
    }

    std::pair<int, int> getMouseMotion() const {
        return { mouseXrel, mouseYrel };
    }

    void resetMouseMotion() {
        mouseXrel = 0;
        mouseYrel = 0;
    }
};

#endif 