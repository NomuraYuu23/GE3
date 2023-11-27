#pragma once
struct Vector2 {

	float x;
	float y;

};

class Vector2Calc
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Vector2Calc* GetInstance();

	//加算
	Vector2 Add(const Vector2& v1, const Vector2& v2);
	//減算
	Vector2 Subtract(const Vector2& v1, const Vector2& v2);
	//スカラー倍
	Vector2 Multiply(float scalar, const Vector2& v);
	//内積
	float Dot(const Vector2& v1, const Vector2& v2);
	//長さ（ノルム）
	float Length(const Vector2& v);
	//正規化
	Vector2 Normalize(const Vector2& v);
	//クロス積
	//Vector3 Cross(const Vector2& v1, const Vector2& v2);

	// 線形補間
	//Vector3 Lerp(const Vector2& v1, const Vector2& v2, float t);

private:
	Vector2Calc() = default;
	~Vector2Calc() = default;
	Vector2Calc(const Vector2Calc&) = delete;
	Vector2Calc& operator=(const Vector2Calc&) = delete;

};
