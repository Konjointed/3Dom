#include "SystemManager.h"

#include "ISystem.h"

SystemManager gSystemManager;

void SystemManager::Update(float timestep)
{
    for (auto& system : m_systems) {
        if (system->m_Enabled) {
            system->Update(timestep);
        }
    }
}


