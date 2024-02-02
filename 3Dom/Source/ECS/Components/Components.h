#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

#include "ECS/IComponent.h"

class Tag : public IComponent {
public:
	std::string name;

	Tag(const std::string& name = "unnamed entity") : name(name) {}
};

class Transform : public IComponent {

};

class Mover : public IComponent {

};

class Input : public IComponent {

};

#endif 