#include "Quaternion.h"
#include "Vector3.h"

QuaternionCalc* QuaternionCalc::GetInstance()
{
	static QuaternionCalc instance;
	return &instance;
}

Quaternion QuaternionCalc::Multiply(const Quaternion& lhs, const Quaternion& rhs)
{

	Quaternion result = {};
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	Vector3 q = { lhs.x, lhs.y, lhs.z, };
	Vector3 r = { rhs.x, rhs.y, rhs.z, };
	Vector3 resultVector = v3Calc->Add(v3Calc->Cross(q, r), v3Calc->Add(v3Calc->Multiply(rhs.w,q), v3Calc->Multiply(lhs.w, r)));

	result.x = resultVector.x;
	result.y = resultVector.y;
	result.z = resultVector.z;
	result.w = lhs.w * rhs.w - v3Calc->Dot(q, r);

	return result;
}

Quaternion QuaternionCalc::IdentityQuaternion()
{
	Quaternion result = {0.0f,0.0f,0.0f,1.0f};
	return result;
}

Quaternion QuaternionCalc::Conjugate(const Quaternion& quaternion)
{
	Quaternion result = { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
	return result;
}

float QuaternionCalc::Norm(const Quaternion& quaternion)
{
	float result = sqrtf(quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w);

	return result;
}

Quaternion QuaternionCalc::Normalize(const Quaternion& quaternion)
{
	Quaternion result = {};
	float norm = Norm(quaternion);

	if (quaternion.x != 0.0) {
		result.x = quaternion.x / norm;
	}
	else {
		result.x = 0.0f;
	}

	if (quaternion.y != 0.0) {
		result.y = quaternion.y / norm;
	}
	else {
		result.y = 0.0f;
	}

	if (quaternion.z != 0.0) {
		result.z = quaternion.z / norm;
	}
	else {
		result.z = 0.0f;
	}

	if (quaternion.w != 0.0) {
		result.w = quaternion.w / norm;
	}
	else {
		result.w = 0.0f;
	}

	return result;

}

Quaternion QuaternionCalc::Inverse(const Quaternion& quaternion)
{

	Quaternion result = {};
	Quaternion conjugate = Conjugate(quaternion);

	float norm = Norm(quaternion);

	if (norm != 0.0f) {
		result.x = conjugate.x / (norm * norm);
		result.y = conjugate.y / (norm * norm);
		result.z = conjugate.z / (norm * norm);
		result.w = conjugate.w / (norm * norm);
	}

	return result;
}
