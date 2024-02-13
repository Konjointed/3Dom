#include "CameraController.h"

#include "Input/InputManager.h"
#include "Scene/Camera.h"

void CameraController::Update(Camera& camera, float timestep)
{
	const float speed = 5.0f;
	if (gInputManager.IsKeyDown(SDLK_w)) {
		camera.MoveForward(speed * timestep);
	}

	if (gInputManager.IsKeyDown(SDLK_s)) {
		camera.MoveBackward(speed * timestep);
	}

	if (gInputManager.IsKeyDown(SDLK_a)) {
		camera.StrafeLeft(speed * timestep);
	}

	if (gInputManager.IsKeyDown(SDLK_d)) {
		camera.StrafeRight(speed * timestep);
	}
}
