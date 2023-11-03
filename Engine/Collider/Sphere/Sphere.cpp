#include "Sphere.h"

void Sphere::Initialize(const Vector3& center, float radius)
{

	Collider::Initialize();

	center_ = center; //中心点
	radius_ = radius;   //半径

}

void Sphere::worldTransformUpdate()
{

	worldTransform_.transform_.translate = center_;
	worldTransform_.transform_.scale = { radius_ * 2.0f, radius_ * 2.0f, radius_ * 2.0f };
	worldTransform_.UpdateMatrix();

}
