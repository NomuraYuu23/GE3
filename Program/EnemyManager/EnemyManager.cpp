#include "EnemyManager.h"

EnemyManager::~EnemyManager(){
	//解放
	for (Enemy* enemy_:enemys_){
		delete enemy_;
	}
}

void EnemyManager::Initialize(const std::vector<Model*>& models,
	const std::vector<Material*>& materials){
	// nullポインタチェック
	//モデルデータ配列
	models_ = models;
	//マテリアルデータ配列
	materials_ = materials;
}

void EnemyManager::Update(){
	//ブレイクフラグの立ったブロックのコライダー描画を削除
	for (Enemy* enemy_ : enemys_) {
		if (enemy_->GetIsDead()) {
			colliderDebugDraw_->DeleteCollider(&enemy_->GetCollider());
		}
	}
	//ブレイクフラグの立ったブロックを削除
	enemys_.remove_if([](Enemy* breakBox) {
		if (breakBox->GetIsDead()) {
			delete breakBox;

			return true;
		}
		return false;

		});

	for (Enemy* enemy_ : enemys_) {
		enemy_->Update();
	}
}

void EnemyManager::Draw(const ViewProjection& viewProjection){
	for (Enemy* enemy_ : enemys_) {
		enemy_->Draw(viewProjection);
	}
}

void EnemyManager::DrawImgui(){

}

void EnemyManager::AddEnemy(TransformStructure enemy){
	Enemy* enemy_ = new Enemy();
	enemy_->Initialize(enemy, models_, materials_);

	enemys_.push_back(enemy_);

	colliderDebugDraw_->AddCollider(&enemy_->GetCollider());
}
