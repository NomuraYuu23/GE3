#include "FloorManager.h"

FloorManager::~FloorManager()
{

	// 解放
	for (Floor* floor : floors_) {
		delete floor;
	}

}

void FloorManager::Initialize(Model* model, Material* material)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

}

void FloorManager::Update()
{

	for (Floor* floor : floors_) {
		floor->Update();
	}

}

void FloorManager::Draw(const ViewProjection& viewProjection)
{

	for (Floor* floor : floors_) {
		floor->Draw(viewProjection);
	}

}

void FloorManager::AddFloor(Vector3 position, Vector3 rotate, bool isMoving, bool isVertical)
{

	Floor* floor = new Floor();
	floor->Initialize(model_, material_, position, rotate, isMoving, isVertical);

	floors_.push_back(floor);

	colliderDebugDraw_->AddCollider(&floor->GetCollider());

}
