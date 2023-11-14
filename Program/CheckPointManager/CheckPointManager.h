#pragma once
#include"../CheckPoint/CheckPoint.h"
#include<list>
#include "../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画

class CheckPointManager{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~CheckPointManager();

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
	void AddCheck(TransformStructure checkPoint);

public: // アクセッサ

	/// <summary>
	/// 床リストゲッター
	/// </summary>
	/// <returns></returns>
	std::list<CheckPoint*> GetCheckPoints_() { return checkPoints_; }

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

	// チェックポイントリスト
	std::list<CheckPoint*> checkPoints_;

	// デバッグ描画
	ColliderDebugDraw* colliderDebugDraw_ = nullptr;

};

