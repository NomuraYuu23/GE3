#pragma once
#include"../Enemy/Enemy.h"
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/Material.h"
#include<list>

#include "../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画

class EnemyManager{
public:
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
	/// imgui描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void DrawImgui();

	/// <summary>
	/// 床追加
	/// </summary>
	void AddEnemy(TransformStructure enemy);

public: // アクセッサ

	/// <summary>
	/// 床リストゲッター
	/// </summary>
	/// <returns></returns>
	std::list<Enemy*> GetEnemys_() { return enemys_; }

	/// <summary>
	/// デバッグ描画セッター
	/// </summary>
	/// <param name="colliderDebugDraw"></param>
	void SetColliderDebugDraw(ColliderDebugDraw* colliderDebugDraw) { colliderDebugDraw_ = colliderDebugDraw; }

private:

	//モデルデータ配列
	std::vector<Model*> models_;
	//マテリアルデータ配列
	std::vector<Material*> materials_;

	// 床リスト
	std::list<Enemy*> enemys_;

	// デバッグ描画
	ColliderDebugDraw* colliderDebugDraw_ = nullptr;
};

