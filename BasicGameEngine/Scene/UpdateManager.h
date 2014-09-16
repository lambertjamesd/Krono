#pragma once

#include <vector>

class UpdateManager;

class Updatable
{
public:
	virtual ~Updatable();

	virtual void Update(float deltaTime) = 0;
protected:
	Updatable(UpdateManager& updateManager);

	UpdateManager& mUpdateManager;
};

class UpdateManager
{
public:
	UpdateManager(void);
	~UpdateManager(void);

	void Update(float deltaTime);
private:
	friend class Updatable;

	void AddUpdatable(Updatable* target);
	void RemoveUpdatable(Updatable* target);

	std::vector<Updatable*> mUpdateList;

	bool mIsUpdating;
};

