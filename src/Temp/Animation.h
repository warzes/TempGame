#pragma once

#include "ConstAnimation.h"
#include "Meshes.h"

enum class AnimRepeat {
	INFINITE,
	ONCE
};

enum class AnimStart {
	AT_CURRENT_FRAME,
	AT_FIRST_FRAME
};

class Animation {
public:
	Animation(std::shared_ptr<ConstAnimation>, const Meshes&);

	const std::vector<Bone>& getSkeleton() const;
	const AABBox<float>& getFramesAABBox() const;
	const std::vector<AABBox<float>>& getFramesSplitAABBoxes() const;
	const AABBox<float>& getLocalFramesAABBox() const;

	const ConstAnimation& getConstAnimation() const;
	unsigned int getCurrentFrame() const;
	float getAnimationProgression() const;
	const std::vector<std::size_t>& getAnimatedMeshIndices() const;

	void animate(float);
	bool gotoFrame(unsigned int);

	void onMoving(const nTransform<float>&);

private:
	void computeNextFrame();

	mutable std::shared_ptr<ConstAnimation> constAnimation;
	const Meshes& meshes;

	std::vector<std::size_t> animatedMeshIndices;
	AnimationInformation animationInformation;
	std::vector<Bone> skeleton;
	AABBox<float> framesBBox;
	std::vector<AABBox<float>> framesSplitBBoxes;
};