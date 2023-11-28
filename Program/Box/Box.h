#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/Material.h"
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/Collider/AABB/AABB.h"
#include "../../Engine/3D/TransformStructure.h"

class Box{
public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Material* material, TransformStructure transform_, Vector3 pos, bool isMoving, bool isVertical);

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

	WorldTransform GetWorldTransform() { return worldTransform_; }

	WorldTransform GetDrawWorldTransform() { return drawWorldTransform_; }

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	WorldTransform* GetDrawWorldTransformAdress() { return &drawWorldTransform_; }

	bool GetMoveFlag() { return isMoving_; }

	bool GetVerticalFlag() { return isVertical_; }

	bool GetDeleteFlag() { return isDelete_; }

	Vector3 GetSize() { return size_; }

	Vector3 GetPosition() { return position_; }

	AABB& GetCollider() { return collider_; }

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
	bool isDelete_;

	// 移動用タイマー
	float moveTimer_;

	// サイズ
	Vector3 size_ = { 10.0f,0.1f, 10.0f };

	// position
	Vector3 position_;

	AABB collider_;

};