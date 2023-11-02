#include "ColliderDebugDraw.h"

void ColliderDebugDraw::Initialize(const std::vector<Model*> models, Material* material)
{

	models_ = models;

	material_ = material;

	spheres_.clear();

	aabbs_.clear();

	obbs_.clear();

}

void ColliderDebugDraw::Update()
{

	spheres_.remove_if([](Sphere* sphere) {
		if (sphere) {
			return true;
		}
		return false;
	});

	aabbs_.remove_if([](AABB* aabb) {
		if (aabb) {
			return true;
		}
		return false;
	});

	obbs_.remove_if([](OBB* obb) {
		if (obb) {
			return true;
		}
		return false;
	});


}

void ColliderDebugDraw::Draw(const ViewProjection& viewProjection)
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	WorldTransform worldTransform;
	worldTransform.Initialize();

	// ‹…
	for (Sphere* sphere : spheres_) {
		worldTransform.transform_.translate = sphere->center_;
		worldTransform.transform_.scale = { sphere->radius_ * 2.0f,sphere->radius_ * 2.0f, sphere->radius_ * 2.0f };
		worldTransform.UpdateMatrix();
		models_[static_cast<size_t>(ModelNo::kSphere)]->Draw(worldTransform, viewProjection, material_);
	}

	// AABB
	for (AABB* aabb : aabbs_) {
		worldTransform.transform_.translate = v3Calc->Multiply(0.5f, v3Calc->Add(aabb->max_, aabb->min_));
		worldTransform.transform_.scale = v3Calc->Subtract(aabb->max_, aabb->min_);
		worldTransform.UpdateMatrix();
		models_[static_cast<size_t>(ModelNo::kSphere)]->Draw(worldTransform, viewProjection, material_);
	}
	// OBB
	std::list<OBB*> obbs_;
	for (OBB* obb : obbs_) {
		worldTransform.transform_.translate = obb->center_;
		worldTransform.transform_.scale = v3Calc->Multiply(2.0f, obb->size_);
		worldTransform.UpdateMatrix();
		models_[static_cast<size_t>(ModelNo::kSphere)]->Draw(worldTransform, viewProjection, material_);
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
