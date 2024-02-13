#include "Scene.h"

#include "Scene/CameraController.h"

void AddObject(GameObject* object) {
	object->Print();
	gScene.objects.push_back(std::unique_ptr<GameObject>(object));
}

void CreateScene() {
	gScene.camera = std::make_unique<Camera>();
	gScene.camera.get()->SetController(new CameraController);

	AddObject(new GameObject("Player", "cube", "wood"));
}

void UpdateScene(float timestep) {
	gScene.camera.get()->Update(timestep);
}