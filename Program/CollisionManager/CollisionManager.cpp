#include "CollisionManager.h"
#include "Collision.h"

void CollisionManager::Initialize(Player* player, FloorManager* floorManager, Goal* goal, Enemy* enemy)
{

	v3Calc = Vector3Calc::GetInstance();
	m4Calc = Matrix4x4Calc::GetInstance();

	player_ = player;

	floorManager_ = floorManager;

	goal_ = goal;

	enemy_ = enemy;

}

void CollisionManager::AllCollision()
{

	// プレイヤーと床
	for (Floor* floor : floorManager_->GetFloors()) {

		// あたり判定確認
		if (Collision::IsCollision(floor->GetCollider(), player_->GetCollider())) {
			player_->OnCollision(floor->GetWorldTransformAdress());
		}
		
	}

	if (Collision::IsCollision(goal_->GetCollider(), player_->GetCollider()) || 
		(Collision::IsCollision(enemy_->GetCollider(), player_->GetCollider()) &&
			!enemy_->GetIsDead())) {
		player_->Restart();
		enemy_->SetIsDead(false);
	}

	if (Collision::IsCollision(enemy_->GetCollider(), player_->GetAttackCollider()) &&
		player_->GetIsAttackJudgment()) {
		enemy_->SetIsDead(true);
	}

}
