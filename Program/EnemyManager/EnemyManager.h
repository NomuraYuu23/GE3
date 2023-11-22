#pragma once
#include "../Enemy/Enemy.h"
#include "../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"
#include <list>
#include <vector>

/// <summary>
/// エネミーマネージャー
/// </summary>
class EnemyManager
{

public: // サブクラス

	enum EnemyModel {
		kBody,
		kRightArm,
		kLeftArm,
		kCountOfEnemyModel
	};


public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(const std::vector<Model*>& models,
		const std::vector<Material*>& materials);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 敵追加
	/// </summary>
	void AddEnemy(const Vector3& position);

	/// <summary>
	/// リスタート関数
	/// </summary>
	void Restart();

public: // アクセッサ

	/// <summary>
	/// 床リストゲッター
	/// </summary>
	/// <returns></returns>
	std::list<Enemy*> GetEnemies() { return enemies_; }

	/// <summary>
	/// デバッグ描画セッター
	/// </summary>
	/// <param name="colliderDebugDraw"></param>
	void SetColliderDebugDraw(ColliderDebugDraw* colliderDebugDraw) { colliderDebugDraw_ = colliderDebugDraw; }

private:

	// モデル
	std::vector<Model*> models_;
	// モデル
	std::vector<Material*> materials_;

	// 敵リスト
	std::list<Enemy*> enemies_;

	// デバッグ描画
	ColliderDebugDraw* colliderDebugDraw_ = nullptr;

};

