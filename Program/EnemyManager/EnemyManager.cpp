#include "EnemyManager.h"

EnemyManager::~EnemyManager()
{

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

}

void EnemyManager::Initialize(const std::vector<Model*>& models,
	const std::vector<Material*>& materials)
{

	models_ = models;
	materials_ = materials;

	Restart();

}

void EnemyManager::Update()
{

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
		});

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

}

void EnemyManager::Draw(const ViewProjection& viewProjection)
{

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection);
	}

}

void EnemyManager::AddEnemy(const Vector3& position)
{

	Enemy* enemy = new Enemy();
	enemy->Initialize(models_, materials_, position);

	enemies_.push_back(enemy);

	colliderDebugDraw_->AddCollider(enemy->GetCollider());

}

void EnemyManager::Restart()
{

	enemies_.remove_if([](Enemy* enemy) {
		return true;
		});

	uint32_t enemyMax = 5;
	Vector3 position = { 0.0f,0.0f,0.0f };
	for (uint32_t i = 0; i < enemyMax; i++) {
		position = { i * -20.0f, 0.0f, -30.0f + i * -20.0f };
		AddEnemy(position);
	}

}
