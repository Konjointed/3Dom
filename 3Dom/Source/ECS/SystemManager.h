#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <vector>
#include <memory>
#include <map>

#include <spdlog/spdlog.h>

#include "Log/Logger.h"

class ISystem;
class IRenderSystem;
enum class RenderStage;

class SystemManager {
public:
	void StartUp();
	void ShutDown();

	// Updates all enabled systems in the database
	void Update(float timestep);

	void Render(RenderStage stage);

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
	template<typename TSystem>
	bool systemExists() const;
private:
	std::vector<std::unique_ptr<ISystem>> m_systems;
	std::map<RenderStage, std::vector<std::unique_ptr<IRenderSystem>>> m_renderSystems;
};

template<typename TSystem, typename... TArgs>
void SystemManager::AddSystem(TArgs&&... args)
{
	//static_assert(std::is_base_of<ISystem, TSystem>::value, "TSystem must derive from ISystem");

	// Add render system
	if constexpr (std::is_base_of<IRenderSystem, TSystem>::value) {
		auto system = std::make_unique<TSystem>(std::forward<TArgs>(args)...);
		RenderStage stage = system->GetRenderStage();
		m_renderSystems[stage].emplace_back(std::move(system));
	}
	// Add update system
	else {
		m_systems.emplace_back(std::make_unique<TSystem>(std::forward<TArgs>(args)...));
	}
}

template<typename TSystem>
void SystemManager::RemoveSystem() 
{
	//static_assert(std::is_base_of<ISystem, TSystem>::value, "TSystem must derive from ISystem");

	// Remove render system
	if constexpr (std::is_base_of<IRenderSystem, TSystem>::value) {
		for (auto& pair : m_renderSystems) {
			auto& systems = pair.second;
			systems.erase(std::remove_if(systems.begin(), systems.end(),
				[](const std::unique_ptr<IRenderSystem>& system) {
					return dynamic_cast<TSystem*>(system.get()) != nullptr;
				}),
				systems.end());
		}
	}
	// Remove update system
	else {
		m_systems.erase(std::remove_if(m_systems.begin(), m_systems.end(),
			[](const std::unique_ptr<ISystem>& system) {
				return dynamic_cast<TSystem*>(system.get()) != nullptr;
			}),
			m_systems.end());
	}
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

template<typename TSystem>
bool SystemManager::systemExists() const {
	static_assert(std::is_base_of<ISystem, TSystem>::value, "TSystem must derive from ISystem");

	return std::any_of(m_systems.begin(), m_systems.end(),
		[](const std::unique_ptr<ISystem>& system) {
			return dynamic_cast<TSystem*>(system.get()) != nullptr;
		});
}

extern SystemManager gSystemManager;

#endif 