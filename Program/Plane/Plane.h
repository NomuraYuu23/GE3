#pragma once
#include "../Collider/Collider.h"
class Plane :
    public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">センター</param>
	/// <param name="radius">半径</param>
	void Initialize(Vector3 normal, float distance);

public: // アクセッサ

	void SetNormal(Vector3 normal) { normal_ = normal; }

	Vector3 GetNormal() { return normal_; }

	void SetDistance(float distance) { distance_ = distance; }

	float GetDistance() { return distance_; }

private: // メンバ変数

	Vector3 normal_;//法線
	float distance_;//距離

};
