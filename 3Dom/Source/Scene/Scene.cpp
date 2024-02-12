#include "Scene.h"

#include "Log/Logger.h"

void AddObject(GameObject* object) {
	object->Print();
	gScene.objects.push_back(std::unique_ptr<GameObject>(object));
}

void CreateScene() {
	gScene.camera = std::make_unique<Camera>();
	AddObject(new GameObject("Player", "cube", "brick"));
}