#include "ShadowManager.h"

// 影
std::list<WorldTransform> ShadowManager::shadowWorldTransforms_;

// 影をつくるobj
std::list<WorldTransform*> ShadowManager::makerWorldTransforms_;

// 影をうつすobj
std::list<WorldTransform*> ShadowManager::floorWorldTransforms_;

// モデル
Model* ShadowManager::model_;

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
	std::list<WorldTransform>::iterator itr = shadowWorldTransforms_.begin();

	for (WorldTransform* makerWorldTransform : makerWorldTransforms_) {
		makerPosition = { 
			makerWorldTransform->worldMatrix_.m[3][0],
			makerWorldTransform->worldMatrix_.m[3][1], 
			makerWorldTransform->worldMatrix_.m[3][2] };
		makerRadius = {
			makerWorldTransform->transform_.scale.x / 2.0f,
			makerWorldTransform->transform_.scale.y / 2.0f,
			makerWorldTransform->transform_.scale.z / 2.0f };
		for (WorldTransform* floorWorldTransform : floorWorldTransforms_) {
			floorPosition = {
				floorWorldTransform->worldMatrix_.m[3][0],
				floorWorldTransform->worldMatrix_.m[3][1],
				floorWorldTransform->worldMatrix_.m[3][2] };
			floorRadius = {
				floorWorldTransform->transform_.scale.x / 2.0f ,
				floorWorldTransform->transform_.scale.y / 2.0f,
				floorWorldTransform->transform_.scale.z / 2.0f };

			// xとzで当たってるか
			if (makerPosition.x + makerRadius.x > floorPosition.x - floorRadius.x &&
				makerPosition.x - makerRadius.x < floorPosition.x + floorRadius.x &&
				makerPosition.z + makerRadius.z > floorPosition.z - floorRadius.z &&
				makerPosition.z - makerRadius.z < floorPosition.z + floorRadius.z ) {

				// yが m > f && f > a 
				if (makerPosition.y > floorPosition.y &&
					(!ansWorldTransform || floorPosition.y > ansWorldTransform->worldMatrix_.m[3][1])) {
					ansWorldTransform = floorWorldTransform;
				}

			}

		}
		
		//ansWorldTransformがnullじゃない
		if (ansWorldTransform) {
			*itr = *ansWorldTransform;
			itr->transform_.translate.y += floorRadius.y;
		}
		else {
			//描画しない
		}

		// 後処理
		itr++;
		ansWorldTransform = nullptr;
	}


	// ワールドトランスフォーム更新
	for (WorldTransform worldTransform : shadowWorldTransforms_) {
		worldTransform.UpdateMatrix();
	}

}

void ShadowManager::Draw(const ViewProjection& viewProjection)
{

	for (WorldTransform worldTransform : shadowWorldTransforms_) {
		model_->Draw(worldTransform, viewProjection);
	}

}

void ShadowManager::AddMeker(WorldTransform* worldTransform)
{

	makerWorldTransforms_.push_back(worldTransform);
	WorldTransform newWorldTransform;
	newWorldTransform.Initialize();
	newWorldTransform.parent_ = worldTransform;
	shadowWorldTransforms_.push_back(newWorldTransform);

}

void ShadowManager::AddFloor(WorldTransform* worldTransform)
{

	floorWorldTransforms_.push_back(worldTransform);

}

void ShadowManager::CheckIfItsGone()
{

	// 影
	shadowWorldTransforms_.remove_if([](WorldTransform worldTransform) {
		if (!worldTransform.parent_) {
			return true;
		}
		return false;
		});

	// 影をつくるobj
	makerWorldTransforms_.remove_if([](WorldTransform* worldTransform) {
		if (!worldTransform) {
			return true;
		}
		return false;
		});

	// 影をうつすobj
	floorWorldTransforms_.remove_if([](WorldTransform* worldTransform) {
		if (!worldTransform) {
			return true;
		}
		return false;
		});

}
