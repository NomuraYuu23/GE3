#include "FloorManager.h"

FloorManager::~FloorManager()
{

	// 解放
	for (Floor* floor : floors_) {
		delete floor;
	}

}

void FloorManager::Initialize(Model* model, Material* material, ViewProjection* viewProjection)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

	// ワールド変換データの初期化
	viewProjection_ = viewProjection;

}

void FloorManager::Update()
{

	for (Floor* floor : floors_) {
		floor->Update();
	}

}

void FloorManager::Draw()
{

	for (Floor* floor : floors_) {
		floor->Draw();
	}

}

void FloorManager::AddFloor(Vector3 position, Vector3 rotate, bool isMoving)
{

	Floor* floor = new Floor();
	floor->Initialize(model_, material_, viewProjection_, position, rotate, isMoving);

	floors_.push_back(floor);

}
