#include "stdafx.h"
#include "Model.h"
#include "LoaderMesh.h"
#include "LoaderAnim.h"

nModel::nModel(const std::string& meshesFilename) :
	defaultModelAABBoxes({ nModel::getDefaultModelLocalAABBox() }),
	activeAnimation(nullptr),
	isModelAnimated(false),
	stopAnimationAtLastFrame(false),
	shadowBehavior(ShadowBehavior::RECEIVER_AND_CASTER),
	cullBehavior(CullBehavior::CULL),
	originalVerticesOrUvUpdated(false) {
	if (!meshesFilename.empty()) 
	{
		LoaderUrchinMesh loader;
		auto constMeshes = loader.loadFromFile(meshesFilename, {});
		meshes = std::make_unique<Meshes>(std::move(constMeshes));
	}
	initialize();
}

nModel::nModel(std::unique_ptr<Meshes> meshes) :
	defaultModelAABBoxes({ nModel::getDefaultModelLocalAABBox() }),
	meshes(std::move(meshes)),
	activeAnimation(nullptr),
	isModelAnimated(false),
	stopAnimationAtLastFrame(false),
	shadowBehavior(ShadowBehavior::RECEIVER_AND_CASTER),
	cullBehavior(CullBehavior::CULL),
	originalVerticesOrUvUpdated(false) {
	initialize();
}

nModel::nModel(const nModel& model) :
	Octreeable(model),
	defaultModelAABBoxes({ nModel::getDefaultModelLocalAABBox() }),
	activeAnimation(nullptr),
	isModelAnimated(false),
	stopAnimationAtLastFrame(false),
	transform(model.getTransform()),
	shadowBehavior(model.getShadowBehavior()),
	cullBehavior(model.getCullBehavior()),
	originalVerticesOrUvUpdated(model.isOriginalVerticesOrUvUpdated()) {
	if (model.meshes) {
		meshes = std::make_unique<Meshes>(model.meshes->copyConstMeshesRef());
	}
	initialize();
}

std::unique_ptr<nModel> nModel::fromMeshesFile(const std::string& meshesFilename) {
	return std::unique_ptr<nModel>(new nModel(meshesFilename));
}

std::unique_ptr<nModel> nModel::fromMemory(std::unique_ptr<Meshes> meshes) {
	return std::unique_ptr<nModel>(new nModel(std::move(meshes)));
}

void nModel::initialize() {
	if (meshes) {
		meshesUpdated.resize(meshes->getNumberMeshes(), false);
		meshes->onMoving(transform);
	}
}

const AABBox<float>& nModel::getDefaultModelLocalAABBox() const {
	//Use a default size of 1, 1, 1: allow to define zone in game through model without mesh. Scale can be used to size up/down the zone.
	static AABBox defaultModelLocalAABBox(Point3<float>(-0.5f, -0.5f, -0.5f), Point3<float>(0.5f, 0.5f, 0.5f));
	return defaultModelLocalAABBox;
}

void nModel::loadAnimation(const std::string& animationName, const std::string& filename) {
	if (!meshes) {
		throw std::runtime_error("Cannot add animation on model without mesh");
	}
	else if (hasAnimationLoaded(animationName)) {
		throw std::runtime_error("Animation with name " + animationName + " already exist on model: " + meshes->getConstMeshes().getMeshesName());
	}

	//load and add the anim to the std::map
	LoaderUrchinAnim loader;
	auto constAnimation = loader.loadFromFile(filename, {});
	animations.try_emplace(animationName, std::make_unique<Animation>(constAnimation, *meshes));
	animations[animationName]->onMoving(transform);

	//both files must have the same number of bones
	if (meshes->getConstMeshes().getConstMesh(0).getNumberBones() != constAnimation->getNumberBones()) {
		throw std::runtime_error("Both files have not the same number of bones. Meshes filename: " + meshes->getConstMeshes().getMeshesName() + ", Animation filename: " + constAnimation->getAnimationFilename() + ".");
	}

	//check with mesh[0] && frame[0]
	for (unsigned int i = 0; i < meshes->getConstMeshes().getConstMesh(0).getNumberBones(); ++i) {
		//bones must have the same parent index
		if (meshes->getConstMeshes().getConstMesh(0).getBaseBone(i).parent != constAnimation->getBone(0, i).parent) {
			throw std::runtime_error("Bones have not the same parent index. Meshes filename: " + meshes->getConstMeshes().getMeshesName() + ", Animation filename: " + constAnimation->getAnimationFilename() + ".");
		}

		//bones must have the same name
		if (meshes->getConstMeshes().getConstMesh(0).getBaseBone(i).name != constAnimation->getBone(0, i).name) {
			throw std::runtime_error("Bones have not the same name. Meshes filename: " + meshes->getConstMeshes().getMeshesName() + ", Animation filename: " + constAnimation->getAnimationFilename() + ".");
		}
	}
}

bool nModel::hasAnimationLoaded(std::string_view animationName) const {
	return animations.contains(animationName);
}

void nModel::animate(std::string_view animationName, AnimRepeat animLoop, AnimStart animStart) {
	auto itAnimation = animations.find(animationName);
	if (itAnimation == animations.end()) {
		throw std::invalid_argument("Unable to find " + std::string(animationName) + " animation on model: " + (getConstMeshes() ? getConstMeshes()->getMeshesName() : "[null]"));
	}
	activeAnimation = itAnimation->second.get();
	if (animStart == AnimStart::AT_FIRST_FRAME) {
		activeAnimation->gotoFrame(0);
	}
	isModelAnimated = true;
	stopAnimationAtLastFrame = (animLoop == AnimRepeat::ONCE);

	onMoving(transform);
	notifyObservers(this, nModel::ANIMATION_STARTED);
}

void nModel::stopAnimation(bool immediate) {
	if (immediate) {
		isModelAnimated = false;
	}
	else if (isAnimated()) {
		stopAnimationAtLastFrame = true;
	}
}

void nModel::resetAnimations() {
	//disable current animation
	stopAnimation(true);
	if (activeAnimation) {
		activeAnimation = nullptr;
		onMoving(transform);
	}

	//reset all animations to frame 0
	for (const auto& [animName, anim] : animations) {
		anim->gotoFrame(0);
	}

	//apply skeleton bind pose
	for (unsigned int meshIndex = 0; meshIndex < meshes->getNumberMeshes(); ++meshIndex) {
		meshes->getMesh(meshIndex).resetSkeleton();
	}
	notifyMeshVerticesUpdated();
}

void nModel::gotoAnimationFrame(std::string_view animationName, unsigned int animationFrameIndex) {
	const Animation* previousActiveAnimation = activeAnimation;
	auto itAnimation = animations.find(animationName);
	if (itAnimation == animations.end()) {
		throw std::invalid_argument("Unable to find " + std::string(animationName) + " animation on model: " + (getConstMeshes() ? getConstMeshes()->getMeshesName() : "[null]"));
	}
	activeAnimation = itAnimation->second.get();
	if (previousActiveAnimation != activeAnimation) {
		onMoving(transform);
	}

	bool frameUpdated = activeAnimation->gotoFrame(animationFrameIndex);
	if (frameUpdated) {
		notifyMeshVerticesUpdatedByAnimation();
	}
}

const Animation* nModel::getActiveAnimation() const {
	return activeAnimation;
}

bool nModel::hasActiveAnimation() const {
	return activeAnimation != nullptr;
}

bool nModel::isAnimated() const {
	return hasActiveAnimation() && isModelAnimated;
}

void nModel::onMoving(const nTransform<float>& newTransform) {
	//update the bounding box
	if (meshes) {
		meshes->onMoving(newTransform);
		if (hasActiveAnimation()) {
			activeAnimation->onMoving(newTransform);
		}
	}
	else {
		defaultModelAABBoxes[0] = nModel::getDefaultModelLocalAABBox().moveAABBox(transform);
	}

	if (getCullBehavior() == CullBehavior::CULL) {
		//inform the OctreeManager that the model should be updated in the octree
		this->notifyOctreeableMove();
	}
}

void nModel::notifyMeshVerticesUpdatedByAnimation() {
	std::fill(meshesUpdated.begin(), meshesUpdated.end(), false);
	for (std::size_t updatedMeshIndex : activeAnimation->getAnimatedMeshIndices()) {
		meshesUpdated[updatedMeshIndex] = true;
	}
	originalVerticesOrUvUpdated = true;
	notifyObservers(this, nModel::MESH_VERTICES_UPDATED);
}

void nModel::notifyMeshVerticesUpdated() {
	std::fill(meshesUpdated.begin(), meshesUpdated.end(), true);

	originalVerticesOrUvUpdated = true;
	notifyObservers(this, nModel::MESH_VERTICES_UPDATED);
}

void nModel::notifyMeshVerticesUpdated(unsigned int updatedMeshIndex) {
	std::fill(meshesUpdated.begin(), meshesUpdated.end(), false);
	meshesUpdated[updatedMeshIndex] = true;

	originalVerticesOrUvUpdated = true;
	notifyObservers(this, nModel::MESH_VERTICES_UPDATED);
}

void nModel::notifyMeshUvUpdated(unsigned int updatedMeshIndex) {
	std::fill(meshesUpdated.begin(), meshesUpdated.end(), false);
	meshesUpdated[updatedMeshIndex] = true;

	originalVerticesOrUvUpdated = true;
	notifyObservers(this, nModel::MESH_UV_UPDATED);
}

void nModel::notifyMeshMaterialUpdated(unsigned int updatedMeshIndex) {
	std::fill(meshesUpdated.begin(), meshesUpdated.end(), false);
	meshesUpdated[updatedMeshIndex] = true;

	notifyObservers(this, nModel::MATERIAL_UPDATED);
}

std::string nModel::getName() const {
	if (meshes) {
		return meshes->getConstMeshes().getName();
	}
	return "[no meshes]";
}

const Meshes* nModel::getMeshes() const {
	return meshes.get();
}

const ConstMeshes* nModel::getConstMeshes() const {
	if (meshes) {
		return &meshes->getConstMeshes();
	}
	return nullptr;
}

const std::map<std::string, std::unique_ptr<Animation>, std::less<>>& nModel::getAnimations() const {
	return animations;
}

/**
* @return Bounding box regrouping all animation frames or the bounding box regrouping all meshes (transformed by the model matrix)
*/
const AABBox<float>& nModel::getAABBox() const {
	if (hasActiveAnimation()) {
		return activeAnimation->getFramesAABBox();
	}
	else if (meshes) {
		return meshes->getMeshesAABBox();
	}
	else {
		return defaultModelAABBoxes[0];
	}
}

/**
* @return Split bounding box regrouping all animation frames or the split bounding box regrouping all meshes (transformed by the model matrix).
*/
const std::vector<AABBox<float>>& nModel::getSplitAABBoxes() const {
	if (hasActiveAnimation()) {
		return activeAnimation->getFramesSplitAABBoxes();
	}
	else if (meshes) {
		return meshes->getMeshesSplitAABBoxes();
	}
	else {
		return defaultModelAABBoxes;
	}
}

/**
* @return Bounding box regrouping all animation frames or the bounding box regrouping all meshes (not transformed).
*/
const AABBox<float>& nModel::getLocalAABBox() const {
	if (hasActiveAnimation()) {
		return activeAnimation->getLocalFramesAABBox();
	}
	else if (meshes) {
		return meshes->getLocalMeshesAABBox();
	}
	else {
		return nModel::getDefaultModelLocalAABBox();
	}
}

void nModel::setPosition(const Point3<float>& position) {
	if (position != transform.getPosition()) {
		transform.setPosition(position);
		onMoving(transform);
	}
}

void nModel::setOrientation(const Quaternion<float>& orientation) {
	if (orientation != transform.getOrientation()) {
		transform.setOrientation(orientation);
		onMoving(transform);
	}
}

void nModel::setScale(const Vector3<float>& scale) {
	if (scale != transform.getScale()) {
		transform.setScale(scale);
		onMoving(transform);
		notifyObservers(this, nModel::SCALE_UPDATED);
	}
}

void nModel::setTransform(const nTransform<float>& transform) {
	if (transform != this->transform) {
		bool scaleUpdated = this->transform.getScale() != transform.getScale();
		this->transform = transform;
		onMoving(transform);
		if (scaleUpdated) {
			notifyObservers(this, NotificationType::SCALE_UPDATED);
		}
	}
}

const nTransform<float>& nModel::getTransform() const {
	return transform;
}

void nModel::setShadowBehavior(ShadowBehavior shadowBehavior) {
	this->shadowBehavior = shadowBehavior;
}

nModel::ShadowBehavior nModel::getShadowBehavior() const {
	if (!getMeshes()) {
		return ShadowBehavior::NONE;
	}
	return shadowBehavior;
}

void nModel::setCullBehavior(CullBehavior cullBehavior) {
	if (this->cullBehavior != cullBehavior) {
		this->cullBehavior = cullBehavior;
		notifyObservers(this, nModel::CULL_BEHAVIOR_UPDATED);
	}
}

nModel::CullBehavior nModel::getCullBehavior() const {
	return cullBehavior;
}

bool nModel::isOriginalVerticesOrUvUpdated() const {
	return originalVerticesOrUvUpdated;
}

bool nModel::isMeshUpdated(unsigned int meshIndex) const {
	return meshesUpdated[meshIndex];
}

void nModel::updateAnimation(float dt) {
	if (isAnimated()) {
		if (stopAnimationAtLastFrame && activeAnimation->getCurrentFrame() + 1 >= activeAnimation->getConstAnimation().getNumberFrames()) {
			stopAnimation(true);
			stopAnimationAtLastFrame = false;
			notifyObservers(this, nModel::ANIMATION_ENDED);
		}
		else {
			activeAnimation->animate(dt);
			notifyMeshVerticesUpdatedByAnimation();
		}
	}
}

void nModel::updateVertices(unsigned int meshIndex, const std::vector<Point3<float>>& vertices) {
	meshes->updateMesh(meshIndex, vertices);

	onMoving(transform);
	notifyMeshVerticesUpdated(meshIndex);
}

void nModel::updateUv(unsigned int meshIndex, const std::vector<Point2<float>>& uv) {
	meshes->updateUv(meshIndex, uv);

	notifyMeshUvUpdated(meshIndex);
}

void nModel::updateMaterial(unsigned int meshIndex, std::shared_ptr<nMaterial> material) {
	if (material.get() != meshes->getMesh(meshIndex).getMaterialPtr().get()) {
#ifdef _DEBUG
		assert(material->getId() != meshes->getMesh(meshIndex).getMaterial().getId());
#endif
		meshes->updateMaterial(meshIndex, std::move(material));
		notifyMeshMaterialUpdated(meshIndex);
	}
}