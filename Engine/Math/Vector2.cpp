#include "Vector2.h"
#include <cmath>

Vector2Calc* Vector2Calc::GetInstance()
{
    static Vector2Calc instance;
    return &instance;
}

Vector2 Vector2Calc::Add(const Vector2& v1, const Vector2& v2)
{
    Vector2 result;

    result = { v1.x + v2.x, v1.y + v2.y };

    return result;
}

Vector2 Vector2Calc::Subtract(const Vector2& v1, const Vector2& v2)
{
    Vector2 result;

    result = { v1.x - v2.x, v1.y - v2.y };

    return result;
}

Vector2 Vector2Calc::Multiply(float scalar, const Vector2& v)
{
    Vector2 result;

    result = { scalar * v.x, scalar * v.y };

    return result;
}

float Vector2Calc::Dot(const Vector2& v1, const Vector2& v2)
{
    float result;

    result = v1.x * v2.x + v1.y * v2.y;

    return result;
}

float Vector2Calc::Length(const Vector2& v)
{
    float result;

    result = std::sqrtf(Dot(v, v));

    return result;
}

Vector2 Vector2Calc::Normalize(const Vector2& v)
{
	Vector2 result;
	float norm;

	norm = Length(v);

	if (v.x != 0.0) {
		result.x = v.x / norm;
	}
	else {
		result.x = 0.0f;
	}

	if (v.y != 0.0) {
		result.y = v.y / norm;
	}
	else {
		result.y = 0.0f;
	}

	return result;
}
