#pragma once
#include "../../Engine/2D/Sprite.h"
#include <memory>

// UI情報
struct UIData
{
	// スプライト
	std::unique_ptr<Sprite> sprite_;
	// テクスチャハンドル
	uint32_t textureHandle_;
	// 位置
	Vector2 position_;
	// 色
	Vector4 color_;

	void Initialize(uint32_t textureHandle);

	void Update();

};