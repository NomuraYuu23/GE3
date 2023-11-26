#include "CollisionManager.h"
#include "Collision.h"
#include "../../Engine/3D/ParticleManager.h"

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
		if (Collision::IsCollision(floor->GetCollider(), *player_->GetCollider())) {
			player_->OnCollision(floor->GetWorldTransformAdress());
		}
		
	}

	if (Collision::IsCollision(goal_->GetCollider(), *player_->GetCollider())) {
		player_->Restart();
		enemyManager_->Restart();
	}
	for (Enemy* enemy : enemyManager_->GetEnemies()) {
		// あたり判定確認
		if (Collision::IsCollision(*enemy->GetCollider(), *player_->GetCollider()) && !enemy->GetDisappear()) {
			player_->Restart();
			enemyManager_->Restart();
		}

		if (Collision::IsCollision(*enemy->GetCollider(), *player_->GetAttackCollider()) &&
			player_->GetIsAttackJudgment()) {
			if (player_->GetComboIndex() == 2) {
				enemy->SetDisappear(true);
				Vector3 position = { player_->GetWorldTransform().worldMatrix_.m[3][0],player_->GetWorldTransform().worldMatrix_.m[3][1], player_->GetWorldTransform().worldMatrix_.m[3][2] };
				enemy->SetBlowOffDirection(position);
			}
			TransformStructure transformStructure = { 
				{1.0f,1.0f,1.0f},
				{0.0f,0.0f,0.0f}, 
				{0.0f,0.0f,0.0f},
			};
			ParticleManager::GetInstance()->EmitterCreate(transformStructure, 10.0f, 1);
		}
	}

}
