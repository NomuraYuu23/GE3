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
		if (input->TriggerJoystick(4)) {
			// ロックオンを外す
			target_ = nullptr;
		}
		// 範囲外判定
		else if (OutOfRangeJudgment(viewProjection)) {
			// ロックオンを外す
			target_ = nullptr;
		}

	}
	else {
		// ロックオン対象の検索
		if (input->TriggerJoystick(4)) {
			// ロックオン対象の検索
			// ロックオン対象の絞り込み
			// 目標
			std::list<std::pair<float, Enemy*>> targets;

			// 全ての敵に対して順にロックオン判定
			for (Enemy* enemy : enemies) {
				// 敵ロックオン座標取得
				Vector3 positionWorld = {
					enemy->GetWorldTransform().worldMatrix_.m[3][0],
					enemy->GetWorldTransform().worldMatrix_.m[3][1],
					enemy->GetWorldTransform().worldMatrix_.m[3][2]
				};
				// ワールドからビュー座標変換
				Vector3 positionView = m4Calc->Transform(positionWorld, viewProjection.viewMatrix_);

				// 距離条件チェック
				if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

					// カメラ前方との角度を計算
					//Vector3 normPositionView = v3Calc->Normalize(positionView);
					//Vector3 referenceLineView = v3Calc->Normalize(m4Calc->Transform(viewProjection.transform_.translate, viewProjection.viewMatrix_));
					//float Dot = v3Calc->Dot(normPositionView, referenceLineView);
					//if (1.0f - std::cosf(angleRange_) <= Dot) {
					//	targets.emplace_back(std::make_pair(positionView.z, enemy));
					//}

					Vector3 a = v3Calc->Normalize(v3Calc->Subtract(positionWorld, viewProjection.transform_.translate));
					Matrix4x4 m = m4Calc->MakeRotateXYZMatrix(viewProjection.transform_.rotate);
					Vector3 b = v3Calc->Normalize(m4Calc->Transform(Vector3{ 0.0f,0.0f,1.0f}, m));
					float Dot = v3Calc->Dot(a, b);
					if (1.0f - std::sinf(angleRange_) <= Dot) {
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
		// 敵のロックオン座標取得
		Vector3 positionWorld = {
			target_->GetWorldTransform().worldMatrix_.m[3][0],
			target_->GetWorldTransform().worldMatrix_.m[3][1],
			target_->GetWorldTransform().worldMatrix_.m[3][2]
		};
		// ワールド座標からスクリーン座標に変換
		Matrix4x4 viewPort = m4Calc->MakeViewportMatrix(0,0,WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matViewProjectionViewPort = m4Calc->Multiply(viewProjection.viewProjectionMatrix_, viewPort);
		Vector3 positionScreen = m4Calc->Transform(positionWorld, matViewProjectionViewPort);
		// Vector2に格納
		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);
		// スプライトの座標を設定
		lockOnMark_->SetPosition(positionScreenV2);
	}

}

void LockOn::Draw()
{

	if (target_) {
		lockOnMark_->Draw();
	}

}

bool LockOn::OutOfRangeJudgment(const ViewProjection& viewProjection)
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	// 敵のロックオン座標取得
	Vector3 positionWorld = {
		target_->GetWorldTransform().worldMatrix_.m[3][0],
		target_->GetWorldTransform().worldMatrix_.m[3][1],
		target_->GetWorldTransform().worldMatrix_.m[3][2]
	};

	// ワールドからビュー座標変換
	Vector3 positionView = m4Calc->Transform(positionWorld, viewProjection.viewMatrix_);

	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
	
		//// カメラ前方との角度を計算
		//Vector3 normPositionView = v3Calc->Normalize(positionView);
		//Vector3 referenceLineView = v3Calc->Normalize(m4Calc->Transform(viewProjection.transform_.translate, viewProjection.viewMatrix_));
		//float Dot = v3Calc->Dot(normPositionView, referenceLineView);
		//if (1.0f - std::cosf(angleRange_) <= Dot) {
		//	return false;
		//}

		Vector3 a = v3Calc->Normalize(v3Calc->Subtract(positionWorld, viewProjection.transform_.translate));
		Matrix4x4 m = m4Calc->MakeRotateXYZMatrix(viewProjection.transform_.rotate);
		Vector3 b = v3Calc->Normalize(m4Calc->Transform(Vector3{ 0.0f,0.0f,1.0f }, m));
		float Dot = v3Calc->Dot(a, b);
		if (1.0f - std::sinf(angleRange_) <= Dot) {
			return false;
		}

	}

	return true;

}
