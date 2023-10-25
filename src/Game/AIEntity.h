#pragma once

#include "Common.h"

class NavObject;

class AIEntity {
public:
	enum AIEntityType {
		OBJECT,
		TERRAIN
	};

	AIEntity(std::string, const nTransform<float>&, bool);
	virtual ~AIEntity() = default;

	void updateTransform(const Point3<float>&, const Quaternion<float>&);
	bool isToRebuild() const;
	void markRebuilt();

	virtual AIEntityType getType() const = 0;

	const std::string& getName() const;
	nTransform<float> getTransform() const;
	bool isObstacleCandidate() const;

	void addNavObject(const std::shared_ptr<NavObject>&);
	const std::vector<std::shared_ptr<NavObject>>& getNavObjects() const;
	void removeAllNavObjects();

private:
	mutable std::mutex mutex;
	std::atomic_bool bToRebuild;

	std::string name;
	nTransform<float> transform;
	bool bIsObstacleCandidate;

	std::vector<std::shared_ptr<NavObject>> navObjects;
};