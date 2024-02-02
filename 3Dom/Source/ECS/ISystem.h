#ifndef ISYSTEM_H
#define ISYSTEM_H

class ISystem {
public:
	bool m_Enabled = true;

	ISystem() = default;
	virtual ~ISystem() = default;

	virtual void Update(float timestep) = 0;
};

#endif 