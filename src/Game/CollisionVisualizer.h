#pragma once

#include "CollisionWorld.h"
#include "ManifoldResult.h"

/**
* Collision visualizer: can be used to debug problems in collision process. This class stock all data coming from collision process in
* order to be displayed in the 3D engine
*/
class CollisionVisualizer final : public Observer {
public:
	explicit CollisionVisualizer(CollisionWorld&);
	~CollisionVisualizer() override;

	void notify(Observable*, int) override;

	std::vector<ManifoldResult> getManifoldResults() const;

private:
	CollisionWorld& collisionWorld;
	mutable std::mutex visualizerDataMutex;

	std::vector<ManifoldResult> manifoldResults;
};