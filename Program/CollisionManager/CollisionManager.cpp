#include "CollisionManager.h"
#include "Collision.h"

void CollisionManager::Initialize(Player* player, FloorManager* floorManager, 
	BoxManager* boxManager, BreakBoxManager* breakBoxManager, 
	RecoveryItemManager* recoveryItemManager, EnemyManager* enemyManager,
	CollectibleItemManager* collectibleItemManager, CheckPointManager* checkPointManager//,
	/*BurningBoxManager* burningBoxManager*/, Goal* goal)
{

	v3Calc = Vector3Calc::GetInstance();
	m4Calc = Matrix4x4Calc::GetInstance();

	player_ = player;

	floorManager_ = floorManager;

	boxManager_ = boxManager;

	breakBoxManager_ = breakBoxManager;

	//burningBoxManager_ = burningBoxManager;

	recoveryItemManager_ = recoveryItemManager;

	collectibleItemManager_ = collectibleItemManager;

	enemyManager_ = enemyManager;

	checkPointManager_ = checkPointManager;

	goal_ = goal;

}

void CollisionManager::AllCollision()
{

	// プレイヤーと床
	for (Floor* floor : floorManager_->GetFloors()) {

		// あたり判定確認
		if (Collision::IsCollision(floor->GetCollider(), player_->GetCollider())) {
			player_->OnCollision(floor->GetWorldTransformAdress());
		}

		for (Enemy* enemy : enemyManager_->GetEnemys_()) {
			if (Collision::IsCollision(floor->GetCollider(), enemy->GetCollider())) {
				enemy->OnCollision(floor->GetWorldTransformAdress());
			}
		}
	}

	for (Box* box_ : boxManager_->GetBoxes_()) {

		// あたり判定確認
		if (Collision::IsCollision(box_->GetCollider(), player_->GetCollider())) {
			player_->OnCollisionBox(box_->GetWorldTransformAdress(), box_->GetSize(), box_->GetMoveFlag());
		}
		for (Enemy* enemy : enemyManager_->GetEnemys_()) {
			if (Collision::IsCollision(box_->GetCollider(), enemy->GetCollider())) {
				enemy->OnCollisionBox(box_->GetWorldTransformAdress(), box_->GetSize(), box_->GetMoveFlag());
			}
		}
	}

	for (BreakBox* breakBox_ : breakBoxManager_->GetBoxes_()) {
		// あたり判定確認
		if (Collision::IsCollision(breakBox_->GetCollider(), player_->GetCollider())) {
			player_->OnCollisionBox(breakBox_->GetWorldTransformAdress(), breakBox_->GetSize(), breakBox_->GetMoveFlag());
		}

		if (Collision::IsCollision(breakBox_->GetCollider(), player_->GetExplosionCollider())) {
			breakBox_->SetIsBreak(true);
		}

		for (Enemy* enemy : enemyManager_->GetEnemys_()) {
			if (Collision::IsCollision(breakBox_->GetCollider(), enemy->GetCollider())) {
				enemy->OnCollisionBox(breakBox_->GetWorldTransformAdress(), breakBox_->GetSize(), breakBox_->GetMoveFlag());
			}
		}
	}
	
	for (RecoveryItem* item : recoveryItemManager_->GetItems_()) {
		if (Collision::IsCollision(item->GetCollider(), player_->GetCollider())) {
			player_->OnCollisionRecoveryItem(item->GetRecoveryValue());
			item->OnCollisionPlayer();
		}
	}
	for (CollectibleItem* item : collectibleItemManager_->GetItems_()) {
		if (Collision::IsCollision(item->GetCollider(), player_->GetCollider())) {
			player_->OnCollisionCollectibleItem();
			item->OnCollisionPlayer();
		}
	}

	for (CheckPoint* check:checkPointManager_->GetCheckPoints_()){
		if (!check->GetStarting()) {
			if (Collision::IsCollision(check->GetCollider(), player_->GetCollider())) {
				player_->SetRestartPosition(check->GetWorldTransform().transform_.translate);
				check->OnCollisiin();
			}
		}
	}

	for (Enemy* enemy:enemyManager_->GetEnemys_()){
		if (Collision::IsCollision(enemy->GetCollider(),player_->GetCollider())){
			player_->OnCollisionEnemy();
		}
		if (Collision::IsCollision(enemy->GetSearchCollider(),player_->GetCollider())){
			enemy->SetPlayer(player_);
		}
		if (Collision::IsCollision(enemy->GetCollider(), player_->GetExplosionCollider())) {
			enemy->SetIsDead(true);
		}
	}

	if (Collision::IsCollision(goal_->GetCollider(), player_->GetCollider())){
		player_->OnCollisionGoal();
	}

	/*if (Collision::IsCollision(enemy_->GetCollider(), player_->GetAttackCollider()) &&
		player_->GetIsAttackJudgment()) {
		enemy_->SetIsDead(true);
	}*/

}
