#include "LockOn.h"
#include "../../Engine/Input/Input.h"

void LockOn::Initialize(uint32_t textureHandle)
{

	//スプライトの生成関数
	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_.reset(
		Sprite::Create(textureHandle, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));

}

void LockOn::Update(const std::list<Enemy*>& enemies, const ViewProjection& viewProjection)
{

	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	// ロックオン状態なら
	if (target_) {
		// ロックオン解除処理
	}
	else {
		// ロックオン対象の検索
		if (input->TriggerJoystick(4)) {
			// ロックオン対象の検索
			// ロックオン対象の絞り込み
			// 目標
			std::list<std::pair<float, const Enemy*>> targets;

			// 全ての敵に対して順にロックオン判定
			for (Enemy* enemy : enemies) {
				// 敵ロックオン座標取得
				Vector3 positionWorld = {
					enemy->GetWorldTransform().worldMatrix_.m[3][0],
					enemy->GetWorldTransform().worldMatrix_.m[3][1],
					enemy->GetWorldTransform().worldMatrix_.m[3][2]
				};
				// ワールドからビュー座標変換
				Vector3 positionView = m4Calc->Transform(positionWorld, viewProjection.viewProjectionMatrix_);

				// 距離条件チェック
				if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

					// カメラ前方との角度を計算
					Vector3 normPositionView = v3Calc->Normalize(positionView);
					Vector3 referenceLineView = m4Calc->Transform(Vector3{ 0.0f,0.0f,1.0f }, viewProjection.viewProjectionMatrix_);
					float Dot = v3Calc->Dot(normPositionView, referenceLineView);
					if (1.0f - angleRange_ >= Dot) {
						targets.emplace_back(std::make_pair(positionView.z, enemy));
					}

				}

				// ロックオン対象をリセット
				target_ = nullptr;
				if (targets.size() > 0) {
					// 距離で昇順にソート
					targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
					// ソートの結果一番近い敵をロックオン対象とする
					target_ = targets.front().second;
				}

			}



		}
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
