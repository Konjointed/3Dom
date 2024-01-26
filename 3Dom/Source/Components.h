#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

class TagComponent : public Component {
public:
	std::string tag;

	TagComponent(const std::string& tag) : tag(tag) {}
};

class TransformComponent : public Component {
public:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	TransformComponent(const glm::vec3& translation = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f),
		const glm::vec3& scale = glm::vec3(1.0f))
		: translation(translation), rotation(rotation), scale(scale) {}
};

class MeshComponent : public Component {
public:
	std::string meshName;

	MeshComponent(const std::string& meshName) : meshName(meshName) {}
};

class MaterialComponent : public Component {
public:
	std::string programName;

	MaterialComponent(const std::string& programName) : programName(programName) {}
};

class CameraComponent : public Component {
public:
	float speed = 5.0f;
	float sensitivity = 0.1f;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	float yaw;
	float pitch;
	float roll;

	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	CameraComponent() : forward(glm::vec3(0.0f, 0.0f, -1.0f)), right(glm::vec3(1.0f, 0.0f, 0.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
		yaw(0.0f), pitch(0.0f), roll(0.0f), fov(45.0f), aspectRatio(1.0f), nearPlane(0.1f), farPlane(500.0f) {}
};

class KeyEventComponent : public Component {
public:
	int key;
	bool pressed;

	KeyEventComponent(int keyCode, bool isPressed) : key(keyCode), pressed(isPressed) {}
};

class MouseButtonEventComponent : public Component {
public:
	int button;
	int x, y;
	bool pressed;

	MouseButtonEventComponent(int btn, int posX, int posY, bool isPressed)
		: button(btn), x(posX), y(posY), pressed(isPressed) {}
};

class MouseMotionEventComponent : public Component {
public:
	int x, y;
	int xrel, yrel;

	MouseMotionEventComponent(int posX, int posY, int deltaX, int deltaY)
		: x(posX), y(posY), xrel(deltaX), yrel(deltaY) {}
};

class SingletonInputComponent : public Component {
public:
    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> mouseButtonStates;
    int mouseX, mouseY;
    int mouseXrel, mouseYrel;

    SingletonInputComponent() : mouseX(0), mouseY(0), mouseXrel(0), mouseYrel(0) {}

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