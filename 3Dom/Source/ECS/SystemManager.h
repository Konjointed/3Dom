#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <vector>
#include <memory>

#include <spdlog/spdlog.h>

#include "Log/Logger.h"

class ISystem;

class SystemManager {
public:
	// Updates all enabled systems in the database
	void Update(float timestep);

	// Adds a system to the database
	template<typename TSystem, typename... TArgs>
	void AddSystem(TArgs&&... args);

	// Removes system from database
	template<typename TSystem>
	void RemoveSystem();

	// Resume updating a system
	template<typename TSystem>
	void EnableSystem();

	// Pause updating a system
	template<typename TSystem>
	void DisableSystem();
private:
	std::vector<std::unique_ptr<ISystem>> m_systems;
};

template<typename TSystem, typename... TArgs>
void SystemManager::AddSystem(TArgs&&... args)
{
	static_assert(std::is_base_of<ISystem, TSystem>::value, "TSystem must derive from ISystem");

	auto it = std::find_if(m_systems.begin(), m_systems.end(),
		[](const std::unique_ptr<ISystem>& system) { return dynamic_cast<TSystem*>(system.get()) != nullptr; });

	if (it == m_systems.end()) { // No system was found so add it
		spdlog::info("System added");
		m_systems.emplace_back(std::make_unique<TSystem>(std::forward<TArgs>(args)...));
	}
	else { // System found don't add it
		spdlog::warn("System of this type already added");
		return;
	}
}

template<typename TSystem>
void SystemManager::RemoveSystem() 
{
	m_systems.erase(
		std::remove_if(m_systems.begin(), m_systems.end(),
						[](const std::unique_ptr<ISystem>& system) { return dynamic_cast<TSystem*>(system.get()) != nullptr; }),
						m_systems.end());
}

template<typename TSystem>
void SystemManager::EnableSystem() 
{
	auto it = std::find_if(m_systems.begin(), m_systems.end(),
		[](const std::unique_ptr<ISystem>& system) -> bool {
			return dynamic_cast<TSystem*>(system.get()) != nullptr;
		});

	if (it != m_systems.end()) {
		(*it)->m_Enabled = true;
	}
}

template<typename TSystem>
void SystemManager::DisableSystem() 
{
	auto it = std::find_if(m_systems.begin(), m_systems.end(),
		[](const std::unique_ptr<ISystem>& system) -> bool {
			return dynamic_cast<TSystem*>(system.get()) != nullptr;
		});

	if (it != m_systems.end()) {
		(*it)->m_Enabled = false;
	}
}

extern SystemManager gSystemManager;

#endif 