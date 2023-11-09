#include "CollisionManager.h"
#include "Collision.h"

void CollisionManager::Initialize(Player* player, FloorManager* floorManager, BoxManager* boxManager/*, Goal* goal, Enemy* enemy*/)
{

	v3Calc = Vector3Calc::GetInstance();
	m4Calc = Matrix4x4Calc::GetInstance();

	player_ = player;

	floorManager_ = floorManager;

	boxManager_ = boxManager;

	/*goal_ = goal;

	enemy_ = enemy;*/

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

	for (Box* box_ : boxManager_->GetBoxes_()) {

		// あたり判定確認
		if (Collision::IsCollision(box_->GetCollider(), player_->GetCollider())) {
			player_->OnCollisionBox(box_->GetWorldTransformAdress(), box_->GetSize().y);
		}

	}

	/*if (Collision::IsCollision(goal_->GetCollider(), player_->GetCollider()) || 
		(Collision::IsCollision(enemy_->GetCollider(), player_->GetCollider()) &&
			!enemy_->GetIsDead())) {
		player_->Restart();
		enemy_->SetIsDead(false);
	}

	if (Collision::IsCollision(enemy_->GetCollider(), player_->GetAttackCollider()) &&
		player_->GetIsAttackJudgment()) {
		enemy_->SetIsDead(true);
	}*/

}
