#include "CollisionManager.h"
#include "Collision.h"

void CollisionManager::Initialize(Player* player, FloorManager* floorManager, Goal* goal, EnemyManager* enemyManager)
{

	v3Calc = Vector3Calc::GetInstance();
	m4Calc = Matrix4x4Calc::GetInstance();

	player_ = player;

	floorManager_ = floorManager;

	goal_ = goal;

	enemyManager_ = enemyManager;

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

	if (Collision::IsCollision(goal_->GetCollider(), player_->GetCollider())) {
		player_->Restart();
		enemyManager_->Restart();
	}
	for (Enemy* enemy : enemyManager_->GetEnemies()) {
		// あたり判定確認
		if (Collision::IsCollision(enemy->GetCollider(), player_->GetCollider()) && !enemy->GetIsDead()) {
			player_->Restart();
			enemyManager_->Restart();
		}

		if (Collision::IsCollision(enemy->GetCollider(), player_->GetAttackCollider()) &&
			player_->GetIsAttackJudgment()) {
			enemy->SetIsDead(true);
		}
	}

}
