#include "ShadowManager.h"

ShadowManager* ShadowManager::GetInstance()
{

	static ShadowManager instance;
	return &instance;

}

void ShadowManager::Initialize(Model* model)
{

	model_ = model;

}

void ShadowManager::Update()
{

	// 消えているかの確認
	CheckIfItsGone();

	// 影の位置(あたり判定)
	WorldTransform* ansWorldTransform = nullptr;
	Vector3 makerPosition = {};
	Vector3 makerRadius = {};
	Vector3 floorPosition = {};
	Vector3 floorRadius = {};
	std::list<Shadow>::iterator itr = shadows_.begin();

	for (const Maker& maker : makers_) {
		makerPosition = { 
			maker.worldTransform_->worldMatrix_.m[3][0],
			maker.worldTransform_->worldMatrix_.m[3][1],
			maker.worldTransform_->worldMatrix_.m[3][2] };
		makerRadius = {
			maker.size_.x / 2.0f ,
			maker.size_.y / 2.0f ,
			maker.size_.z / 2.0f };
		for (const Floor& floor : floors_) {
			floorPosition = {
				floor.worldTransform_->worldMatrix_.m[3][0],
				floor.worldTransform_->worldMatrix_.m[3][1],
				floor.worldTransform_->worldMatrix_.m[3][2] };
			floorRadius = {
				floor.size_.x / 2.0f ,
				floor.size_.y / 2.0f ,
				floor.size_.z / 2.0f };

			// xとzで当たってるか
			if (makerPosition.x + makerRadius.x > floorPosition.x - floorRadius.x &&
				makerPosition.x - makerRadius.x < floorPosition.x + floorRadius.x &&
				makerPosition.z + makerRadius.z > floorPosition.z - floorRadius.z &&
				makerPosition.z - makerRadius.z < floorPosition.z + floorRadius.z ) {

				// yが m > f && f > a 
				if (makerPosition.y >= floorPosition.y &&
					(!ansWorldTransform || floorPosition.y > ansWorldTransform->worldMatrix_.m[3][1])) {
					ansWorldTransform = maker.worldTransform_;
				}

			}

		}
		
		//ansWorldTransformがnullじゃない
		if (ansWorldTransform) {
			itr->worldTransform_ = *ansWorldTransform;
			itr->isDraw_ = true;
		}
		else {
			//描画しない
			itr->isDraw_ = false;
		}

		// 後処理
		itr++;
		ansWorldTransform = nullptr;
	}


	// ワールドトランスフォーム更新
	for (Shadow shadow : shadows_) {
		shadow.worldTransform_.UpdateMatrix();
	}

}

void ShadowManager::Draw(const ViewProjection& viewProjection)
{

	for (Shadow shadow : shadows_) {
		if (shadow.isDraw_) {
			model_->Draw(shadow.worldTransform_, viewProjection);
		}
	}

}

void ShadowManager::AddMeker(WorldTransform* worldTransform, Vector3 size)
{

	Maker maker;
	maker.worldTransform_ = worldTransform;
	maker.size_ = size;
	makers_.push_back(maker);

	Shadow shadow;
	shadow.worldTransform_.Initialize();
	shadow.worldTransform_.transform_.scale = size;
	shadow.worldTransform_.parent_ = maker.worldTransform_;
	shadow.isDraw_ = true;
	shadows_.push_back(shadow);

}

void ShadowManager::AddFloor(WorldTransform* worldTransform, Vector3 size)
{

	Floor floor;
	floor.worldTransform_ = worldTransform;
	floor.size_ = size;
	floors_.push_back(floor);

}

void ShadowManager::CheckIfItsGone()
{

	// 影
	shadows_.remove_if([](Shadow shadow) {
		if (!shadow.worldTransform_.parent_) {
			//return true;
		}
		return false;
		});

	// 影をつくるobj
	makers_.remove_if([](Maker maker) {
		if (!maker.worldTransform_) {
			return true;
		}
		return false;
		});

	// 影をうつすobj
	floors_.remove_if([](Floor floor) {
		if (!floor.worldTransform_) {
			return true;
		}
		return false;
		});

}
