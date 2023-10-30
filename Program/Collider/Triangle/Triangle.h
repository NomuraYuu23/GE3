#pragma once
#include "../Collider.h"

class Triangle : public Collider
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& vertices1, const Vector3& vertices2, const Vector3& vertices3);

public: // アクセッサ

	/// <summary>
	/// セッター
	/// </summary>
	/// <returns></returns>
	void SetVertices(const Vector3& vertices1, const Vector3& vertices2, const Vector3& vertices3);
	
	/// <summary>
	/// ゲッター
	/// </summary>
	/// <returns></returns>
	Vector3* GetVertices() { return vertices_; }

private:

	Vector3 vertices_[3];

};

