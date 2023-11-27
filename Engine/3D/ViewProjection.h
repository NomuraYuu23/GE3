#pragma once
#include "TransformStructure.h"
#include "../Math/Matrix4x4.h"

struct  ViewProjection
{

	TransformStructure transform_{ { 1.0f, 1.0f, 1.0f},{ 0.0f, 0.0f, 0.0f},{ 0.0f, 0.0f, -10.0f} };

	float fovY_;

	float aspectRatio_;

	float nearClip_;

	float farClip_;

	Matrix4x4 viewProjectionMatrix_;

	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;

	// 回転行列
	Matrix4x4 rotateMatrix_;

	// 回転行列
	Matrix4x4 rotateAngleMatrix_;

	// 回転行列
	Matrix4x4 rotateDirectionMatrix_;

	// 方向ベクトルで回転行列
	//bool usedRotate_;

	void Initialize();

	void UpdateMatrix();

};
