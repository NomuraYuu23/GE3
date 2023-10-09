#include "CollisionManager.h"

void CollisionManager::Initialize(Player* player, FloorManager* floorManager)
{

	player_ = player;

	floorManager_ = floorManager;

}

void CollisionManager::Collision()
{

	// プレイヤーと床
	for (Floor* floor : floorManager_->GetFloors()) {

		// あたり判定確認
		if (PlayerAndFloor(floor)) {
			player_->OnCollision(floor->GetWorldTransformAdress());
		}
		
	}

}

bool CollisionManager::PlayerAndFloor(Floor* floor)
{

	// 床情報
	float floorMaxX = floor->GetWorldTransform().transform_.translate.x + floor->GetColliderSize().x;
	float floorMinX = floor->GetWorldTransform().transform_.translate.x - floor->GetColliderSize().x;
	float floorMaxZ = floor->GetWorldTransform().transform_.translate.z + floor->GetColliderSize().z;
	float floorMinZ = floor->GetWorldTransform().transform_.translate.z - floor->GetColliderSize().z;
	float floorHeight = floor->GetWorldTransform().transform_.translate.y;

	// プレイヤー
	Vector3 playerPos = { player_->GetWorldTransform().worldMatrix_.m[3][0], player_->GetWorldTransform().worldMatrix_.m[3][1], player_->GetWorldTransform().worldMatrix_.m[3][2] };
	float playerSize = player_->GetColliderRadius();


	if (playerPos.x >= floorMinX && playerPos.x <= floorMaxX &&
		playerPos.z >= floorMinZ && playerPos.z <= floorMaxZ &&
		playerPos.y + playerSize >= floorHeight && playerPos.y <= floorHeight) {
		return true;
	}

	return false;

}
