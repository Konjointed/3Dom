#ifndef IENTITY_H
#define IENTITY_H

using EntityId = unsigned int;

class IEntity {
public:
	EntityId m_ID;
	bool m_Active = true;

	IEntity() = default;
	virtual ~IEntity() = default;

	bool operator==(const IEntity& other) const {
		return m_ID == other.m_ID;
	}

	bool operator!=(const IEntity& other) const {
		return !(*this == other);
	}
};

#endif 