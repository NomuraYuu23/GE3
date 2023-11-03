#include "ColliderDebugDraw.h"

void ColliderDebugDraw::Initialize(const std::vector<Model*> models, Material* material)
{

	models_ = models;

	material_ = material;

	TransformStructure uvTransform = {
	{1.0f,1.0f,1.0f},
	{0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f},
	};
	Vector4 color = { 1.0f, 1.0f, 1.0f, 0.5f };
	material_->Update(uvTransform, color, None);

	spheres_.clear();

	aabbs_.clear();

	obbs_.clear();

}

void ColliderDebugDraw::Update()
{

	spheres_.remove_if([](Sphere* sphere) {
		if (!sphere) {
			return true;
		}
		return false;
	});

	aabbs_.remove_if([](AABB* aabb) {
		if (!aabb) {
			return true;
		}
		return false;
	});

	obbs_.remove_if([](OBB* obb) {
		if (!obb) {
			return true;
		}
		return false;
	});

}

void ColliderDebugDraw::Draw(const ViewProjection& viewProjection)
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	// 球
	for (Sphere* sphere : spheres_) {
		models_[static_cast<size_t>(ModelNo::kSphere)]->Draw(sphere->worldTransform_, viewProjection, material_);
	}

	// AABB
	for (AABB* aabb : aabbs_) {
		models_[static_cast<size_t>(ModelNo::kSphere)]->Draw(aabb->worldTransform_, viewProjection, material_);
	}
	// OBB
	for (OBB* obb : obbs_) {
		models_[static_cast<size_t>(ModelNo::kSphere)]->Draw(obb->worldTransform_, viewProjection, material_);
	}

}

void ColliderDebugDraw::AddCollider(Sphere* sphere)
{

	spheres_.push_back(sphere);

}

//void ColliderDebugDraw::AddCollider(Plane* plane)
//{
//}
//
//void ColliderDebugDraw::AddCollider(Triangle* triangle)
//{
//}

void ColliderDebugDraw::AddCollider(AABB* aabb)
{

	aabbs_.push_back(aabb);

}

void ColliderDebugDraw::AddCollider(OBB* obb)
{

	obbs_.push_back(obb);

}
