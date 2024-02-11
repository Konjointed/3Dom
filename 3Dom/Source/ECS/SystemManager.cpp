#include "SystemManager.h"

#include "Core/Resources.h"
#include "ECS/ISystem.h"
#include "ECS/IRenderSystem.h"
#include "ECS/SystemManager.h"
#include "ECS/RenderSystems/ShadowRenderSystem.h"
#include "ECS/RenderSystems/DirectionalLightSystem.h"
#include "ECS/Systems/InputSystem.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/MoveSystem.h"
#include "ECS/Systems/BehaviorSystem.h"

SystemManager gSystemManager;

void SystemManager::StartUp() {
    gSystemManager.AddSystem<BehaviorSystem>();
    gSystemManager.AddSystem<MoveSystem>();
    gSystemManager.AddSystem<CameraSystem>();
    gSystemManager.AddSystem<ShadowRenderSystem>();
    gSystemManager.AddSystem<DirectionalLightSystem>();
}

void SystemManager::ShutDown() {
    gSystemManager.RemoveSystem<ShadowRenderSystem>();
    gSystemManager.RemoveSystem<DirectionalLightSystem>();
    gSystemManager.RemoveSystem<BehaviorSystem>();
    gSystemManager.RemoveSystem<MoveSystem>();
    gSystemManager.RemoveSystem<CameraSystem>();
}

void SystemManager::Update(float timestep)
{
    for (auto& system : m_systems) {
        if (system->m_Enabled) {
            system->Update(timestep);
        }
    }
}

void SystemManager::Render(RenderStage stage)
{
    auto it = m_renderSystems.find(stage);
    if (it != m_renderSystems.end()) {
        for (auto& system : it->second) {
            dynamic_cast<IRenderSystem*>(system.get())->Render();
        }
    }
}


