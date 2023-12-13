#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;

};

class QuaternionCalc {
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static QuaternionCalc* GetInstance();

	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	static Quaternion IdentityQuaternion();

	static Quaternion Conjugate(const Quaternion& quaternion);

	static float Norm(const Quaternion& quaternion);

	static Quaternion Normalize(const Quaternion& quaternion);

	static Quaternion Inverse(const Quaternion& quaternion);

	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

private:
	QuaternionCalc() = default;
	~QuaternionCalc() = default;
	QuaternionCalc(const QuaternionCalc&) = delete;
	QuaternionCalc& operator=(const QuaternionCalc&) = delete;

};
