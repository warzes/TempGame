#pragma once

#include "AIEntity.h"

class AIWorld {
public:
	AIWorld();

	void addEntity(const std::shared_ptr<AIEntity>&);
	void removeEntity(const std::shared_ptr<AIEntity>&);

	std::vector<std::shared_ptr<AIEntity>> getEntities() const;
	std::vector<std::shared_ptr<AIEntity>> getEntitiesToRemoveAndReset();

private:
	mutable std::mutex mutex;

	std::vector<std::shared_ptr<AIEntity>> entities;
	std::vector<std::shared_ptr<AIEntity>> entitiesToRemove;
};