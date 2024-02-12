#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "IEntity.h"

using ComponentId = unsigned int;

class IComponent {
public:
	ComponentId m_ID;
	EntityId m_Owner;
	bool m_Enabled = true;

	IComponent() = default;
	virtual ~IComponent() = default;

	bool operator==(const IComponent& other) const {
		return m_ID == other.m_ID;
	}

	bool operator!=(const IComponent& other) const {
		return !(*this == other);
	}
};

#endif 