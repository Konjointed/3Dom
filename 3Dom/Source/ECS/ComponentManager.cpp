#include "ComponentManager.h"

ComponentManager gComponentManager;

void ComponentManager::RemoveAllComponents(EntityId entity)
{
	m_components[entity].clear();
}
