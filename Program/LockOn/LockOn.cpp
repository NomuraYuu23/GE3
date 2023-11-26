#include "LockOn.h"

void LockOn::Initialize(uint32_t textureHandle)
{

	//スプライトの生成関数
	lockOnMark_.reset(
		Sprite::Create(textureHandle, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));

}

void LockOn::Update(const std::list<Enemy*>& enemies, const ViewProjection& viewProjection)
{

	// ロックオン状態なら
	if (target_) {
		// ロックオン解除処理
	}
	else {
		// ロックオン対象の検索
	}

	// ロックオン状態なら
	if (target_) {
		// ロックオンマークの座標計算
	}

}

void LockOn::Draw()
{

	if (target_) {
		lockOnMark_->Draw();
	}

}
