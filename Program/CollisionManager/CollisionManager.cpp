#include "CollisionManager.h"

void CollisionManager::Initialize(Player* player, FloorManager* floorManager, Goal* goal)
{

	player_ = player;

	floorManager_ = floorManager;

	goal_ = goal;

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

	if (PlayerAndGoal()) {
		player_->Restart();
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

bool CollisionManager::PlayerAndGoal()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	// プレイヤー
	Vector3 playerPos = { player_->GetWorldTransform().worldMatrix_.m[3][0], player_->GetWorldTransform().worldMatrix_.m[3][1], player_->GetWorldTransform().worldMatrix_.m[3][2] };
	// ゴール
	//2つの球の中心点間の距離を求める
	float distance = v3Calc->Length(v3Calc->Subtract(playerPos, goal_->GetWorldTransform().transform_.translate));
	
	//半径の合計よりも短ければ衝突
	if (distance <= player_->GetColliderRadius() + goal_->GetRadius()) {
		return true;
	}

	return false;

}
