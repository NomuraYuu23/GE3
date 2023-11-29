#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/Material.h"
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/Collider/AABB/AABB.h"

class BreakBox{
public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Material* material, TransformStructure transform_,
		Vector3 pos,bool isMoving, bool isVertical, Vector3 moveLength);

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
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 縦移動
	/// </summary>
	void verticalMove();


	void DrawImgui();

public: // アクセッサ

	WorldTransform GetWorldTransform()const { return worldTransform_; }

	WorldTransform GetDrawWorldTransform()const { return drawWorldTransform_; }

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	WorldTransform* GetDrawWorldTransformAdress() { return &drawWorldTransform_; }

	Vector3 GetSize() { return size_; }

	Vector3 GetPosition() { return position_; }

	Vector3 GetBaseMoveLength_() { return baseMoveLength_; }

	bool GetMoveFlag() { return isMoving_; }	

	bool GetVerticalFlag() { return isVertical_; }
	//壊れたかどうか
	bool GetIsBreak() { return isBreak_; }

	AABB& GetCollider() { return collider_; }

public:
	void SetIsBreak(bool isBreak){ isBreak_ = isBreak; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	//描画用のワールドトランスフォーム
	WorldTransform drawWorldTransform_;

	// モデル
	Model* model_ = nullptr;
	//マテリアル
	Material* material_ = nullptr;

	// 動くか
	bool isMoving_;

	//縦移動にするか
	bool isVertical_;

	//壊れるか
	bool isBreak_;


	// 移動用タイマー
	float moveTimer_;

	// サイズ
	Vector3 size_ = { 10.0f,0.1f, 10.0f };
	//床をどのくらい移動させるか
	Vector3 baseMoveLength_ = { 20.0f,0.0f,0.0f };
	// position
	Vector3 position_;

	AABB collider_;
};

