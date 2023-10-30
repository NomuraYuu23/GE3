#pragma once
#include "../Collider.h"
class AABB : public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">センター</param>
	/// <param name="radius">半径</param>
	void Initialize(const Vector3& min, const Vector3& max);

public: // アクセッサ

	void SetMin(const Vector3& min) { min_ = min; }

	Vector3 GetMin() { return min_; }

	void SetMax(const Vector3& max) { max_ = max; }

	Vector3 GetMax() { return max_; }

private: // メンバ変数

	Vector3 min_; // 最小
	Vector3 max_; // 最大

};

