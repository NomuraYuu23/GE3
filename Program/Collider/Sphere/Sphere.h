#pragma once
#include "../Collider.h"
class Sphere : public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">センター</param>
	/// <param name="radius">半径</param>
	void Initialize(const Vector3& center, float radius);

public: // アクセッサ

	void SetCenter(const Vector3& center) { center_ = center; }

	Vector3 GetCenter() { return center_; }

	void SetRadius(float radius) { radius_ = radius; }

	float GetRadius() { return radius_; }

private: // メンバ変数

	Vector3 center_; //中心点
	float radius_;   //半径

};

