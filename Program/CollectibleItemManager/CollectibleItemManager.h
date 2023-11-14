#pragma once
#include"../../Program/CollectibleItem/CollectibleItem.h"
#include<list>
#include "../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画


class CollectibleItemManager {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~CollectibleItemManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Material* material);

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
	void AddItem(TransformStructure Item, bool isFalling);

public: // アクセッサ

	/// <summary>
	/// 床リストゲッター
	/// </summary>
	/// <returns></returns>
	std::list<CollectibleItem*> GetItems_() { return collectibleItems_; }

	/// <summary>
	/// デバッグ描画セッター
	/// </summary>
	/// <param name="colliderDebugDraw"></param>
	void SetColliderDebugDraw(ColliderDebugDraw* colliderDebugDraw) { colliderDebugDraw_ = colliderDebugDraw; }

private:

	// モデル
	Model* model_ = nullptr;
	// マテリアル
	Material* material_ = nullptr;

	// 床リスト
	std::list<CollectibleItem*> collectibleItems_;

	// デバッグ描画
	ColliderDebugDraw* colliderDebugDraw_ = nullptr;
};