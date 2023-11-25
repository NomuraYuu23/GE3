#pragma once
#include "../../Engine/Particle/Particle.h"
class PlayerJumpParticle : public Particle
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="numInstance"></param>
	void Initialize(const Vector3& position, const Vector3& size) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cameraMatrix4x4"></param>
	void Update(const Matrix4x4& billBoardMatrix) override;

private:

	float speed_;

	Vector3 direction_;

};

