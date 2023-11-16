#pragma once
#include "../../../Engine/3D/ViewProjection.h"
#include "../../../Engine/3D/WorldTransform.h"
#include "../BaseCamera.h"

class FollowCamera : public BaseCamera{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

private:

	//追従対象
	const WorldTransform* target_ = nullptr;

};

