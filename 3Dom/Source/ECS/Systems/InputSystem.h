//#ifndef INPUT_SYSTEM_H
//#define INPUT_SYSTEM_H
//
//#include <vector>
//
//#include <glm/glm.hpp>
//
//#include "Log/Logger.h"
//#include "Input/InputManager.h"
//#include "ECS/ISystem.h"
//
//class InputSystem : public ISystem {
//public:
//	void Update(float timestep) override {
//        std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cInput, cMove>();
//		for (EntityId& entity : entities) {
//            cMove* move = gComponentManager.GetComponent<cMove>(entity);
//
//			glm::vec3 inputDir = glm::vec3(0.0f);
//
//            // Gather input
//            if (gInputManager.IsKeyDown(SDLK_w)) {
//                inputDir.z = -1;
//            }
//
//            if (gInputManager.IsKeyDown(SDLK_s)) {
//                inputDir.z = 1;
//            }
//
//            if (gInputManager.IsKeyDown(SDLK_a)) {
//                inputDir.x = 1;
//            }
//
//            if (gInputManager.IsKeyDown(SDLK_d)) {
//                inputDir.x = -1;
//            }
//
//            // Normalize to ensure consistent movement speed in all directions
//            if (glm::length(inputDir) > 0.0f) {
//                inputDir = glm::normalize(inputDir);
//            }
//
//            move->m_direction.x = inputDir.x;
//            move->m_direction.z = inputDir.z;
//		}
//	}
//};
//
//#endif 