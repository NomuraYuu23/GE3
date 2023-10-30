#pragma once
#include "../Collider.h"
class OBB : public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& center,
		const Vector3& otientatuons1, const Vector3& otientatuons2, const Vector3& otientatuons3,
		const Vector3& size);

public: // アクセッサ

	void SetCenter(const Vector3& center) { center_ = center; }

	Vector3 GetCenter() { return center_; }

	void SetOtientatuons(const Vector3& otientatuons1, const Vector3& otientatuons2, const Vector3& otientatuons3);

	Vector3* GetOtientatuons() { return otientatuons_; }

	void SetSize(const Vector3& size) { size_ = size; }

	Vector3 GetSize() { return size_; }

private: // メンバ変数

	Vector3 center_;//中心
	Vector3 otientatuons_[3];//座標軸
	Vector3 size_;//座標軸方向の長さの半分

};

