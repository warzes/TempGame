#include "stdafx.h"
#include "Mesh.h"
#include "MeshService.h"
#include "Material.h"

nMesh::nMesh(const ConstMesh& constMesh) :
	constMesh(constMesh),
	material(constMesh.getInitialMaterialPtr()) {

}

void nMesh::updateSkeleton(const std::vector<Bone>& skeleton) {
	MeshService::ComputeVerticesNormalsTangents(constMesh, skeleton, vertices, normals, tangents);
}

void nMesh::resetSkeleton() {
	vertices = constMesh.getBaseVertices();
	normals = constMesh.getBaseNormals();
	tangents = constMesh.getBaseTangents();
}

void nMesh::updateVertices(const std::vector<Point3<float>>& vertices) {
	if (vertices.size() != constMesh.getNumberVertices()) {
		throw std::runtime_error("New vertices quantity (" + std::to_string(vertices.size()) + ") must be equals to original vertices quantity (" + std::to_string(constMesh.getNumberVertices()) + ")");
	}

	this->vertices = vertices;
	MeshService::ComputeNormalsTangents(constMesh, vertices, normals, tangents);
}

void nMesh::updateUv(const std::vector<Point2<float>>& uv) {
	if (uv.size() != constMesh.getNumberVertices()) {
		throw std::runtime_error("New UVs quantity (" + std::to_string(uv.size()) + ") must be equals to original vertices quantity (" + std::to_string(constMesh.getNumberVertices()) + ")");
	}

	this->uv = uv;
}

void nMesh::updateMaterial(std::shared_ptr<nMaterial> material) {
	if (this->getMaterial().GetUvScale() != material->GetUvScale()) {
		//not supported because it would require detecting if material UV scale changed in ModelInstanceDisplayer#notify() method
		throw std::runtime_error("Update material with a different UV scale is not supported");
	}
	this->material = std::move(material);
}

const std::vector<Point3<float>>& nMesh::getVertices() const {
	if (vertices.empty()) {
		return constMesh.getBaseVertices();
	}
	return vertices;
}

const std::vector<Point2<float>>& nMesh::getUv() const {
	if (uv.empty()) {
		return constMesh.getUv();
	}
	return uv;
}

const std::vector<Vector3<float>>& nMesh::getNormals() const {
	if (normals.empty()) {
		return constMesh.getBaseNormals();
	}
	return normals;
}

const std::vector<Vector3<float>>& nMesh::getTangents() const {
	if (tangents.empty()) {
		return constMesh.getBaseTangents();
	}
	return tangents;
}

const nMaterial& nMesh::getMaterial() const {
	return *material;
}

const std::shared_ptr<nMaterial>& nMesh::getMaterialPtr() const {
	return material;
}

void nMesh::drawBaseBones(GeometryContainer& geometryContainer, const Matrix4<float>& modelMatrix) {
	if (boneSphereModels) {
		geometryContainer.removeGeometry(*boneSphereModels);
	}

	std::vector<nSphere<float>> sphereBonePoints;
	sphereBonePoints.reserve(constMesh.getBaseSkeleton().size());
	for (const auto& bone : constMesh.getBaseSkeleton()) {
		sphereBonePoints.emplace_back(0.05f, bone.pos);
	}
	boneSphereModels = std::make_shared<SphereModel>(sphereBonePoints, 7);
	boneSphereModels->setAlwaysVisible(true);
	boneSphereModels->setPolygonMode(RasterizerFillMode::Solid);
	boneSphereModels->setModelMatrix(modelMatrix);
	geometryContainer.addGeometry(boneSphereModels);
}