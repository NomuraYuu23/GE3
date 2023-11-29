#include "CollisionManager.h"
#include "Collision.h"
#include "../../Engine/Audio/Audio.h"
#include "../../Engine/Particle/ParticleManager.h"

void CollisionManager::Initialize(Player* player, FloorManager* floorManager, 
	BoxManager* boxManager, BreakBoxManager* breakBoxManager, 
	RecoveryItemManager* recoveryItemManager, EnemyManager* enemyManager,
	CollectibleItemManager* collectibleItemManager, CheckPointManager* checkPointManager//,
	/*BurningBoxManager* burningBoxManager*/, Goal* goal,FenceManager* fenceManager)
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

	fenceManager_ = fenceManager;

	goal_ = goal;

}

void CollisionManager::AllCollision()
{

	Audio* audio = Audio::GetInstance();
	ParticleManager* particleManager = ParticleManager::GetInstance();
	TransformStructure transformStructure = { { 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f}, {0.0f,0.0f,0.0f} };

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
			audio->PlayWave(Audio::AudioHandleIndex::kBreakBox, false, 1.0f);
		}

		for (Enemy* enemy : enemyManager_->GetEnemys_()) {
			if (Collision::IsCollision(breakBox_->GetCollider(), enemy->GetCollider())) {
				enemy->OnCollisionBox(breakBox_->GetWorldTransformAdress(), breakBox_->GetSize(), breakBox_->GetMoveFlag());
			}
		}
	}

	for (Fence* fence : fenceManager_->GetFences_()) {

		// あたり判定確認
		if (Collision::IsCollision(fence->GetCollider(), player_->GetCollider())) {
			player_->OnCollisionBox(fence->GetWorldTransformAdress(), { fence->GetSize().x,fence->GetSize().y * 300.0f,fence->GetSize().z }, false);
		}
		for (Enemy* enemy : enemyManager_->GetEnemys_()) {
			if (Collision::IsCollision(fence->GetCollider(), enemy->GetCollider())) {
				enemy->OnCollisionBox(fence->GetWorldTransformAdress(), fence->GetSize(), false);
			}
		}
	}
	
	for (RecoveryItem* item : recoveryItemManager_->GetItems_()) {
		if (Collision::IsCollision(item->GetCollider(), player_->GetCollider()) && !item->GetIsGet()) {
			player_->OnCollisionRecoveryItem(item->GetRecoveryValue());
			item->OnCollisionPlayer();
			audio->PlayWave(Audio::AudioHandleIndex::kRecoveryItem, false, 1.0f);
			transformStructure.translate = { item->GetDrawWorldTransform().worldMatrix_.m[3][0],item->GetDrawWorldTransform().worldMatrix_.m[3][1], item->GetDrawWorldTransform().worldMatrix_.m[3][2] };
			particleManager->EmitterCreate(transformStructure, 10, 0.01f, 0.02f, 1, 6);
		}
	}
	for (CollectibleItem* item : collectibleItemManager_->GetItems_()) {
		if (Collision::IsCollision(item->GetCollider(), player_->GetCollider())&&!item->GetIsGet()) {
			player_->OnCollisionCollectibleItem();
			item->OnCollisionPlayer();
			audio->PlayWave(Audio::AudioHandleIndex::kCollectibleItem, false, 1.0f);
			transformStructure.translate = { item->GetDrawWorldTransform().worldMatrix_.m[3][0],item->GetDrawWorldTransform().worldMatrix_.m[3][1], item->GetDrawWorldTransform().worldMatrix_.m[3][2]};
			particleManager->EmitterCreate(transformStructure, 10, 0.01f, 0.02f, 1, 5);
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
		for (Enemy* enemy2 : enemyManager_->GetEnemys_()) {
			if (enemy == enemy2)
				continue;
			if (Collision::IsCollision(enemy->GetCollider(), enemy2->GetCollider())) {
				enemy->OnCollisionEnemy(enemy2->GetWorldTransformAddress(), enemy2->GetColliderRadius());
			}
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
