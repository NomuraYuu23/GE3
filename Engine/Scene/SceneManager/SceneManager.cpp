#include "SceneManager.h"
#include "../../base/TextureManager.h"

void SceneManager::Initialize()
{

	// シーンファクトリー
	sceneFacyory_ = SceneFactory::GetInstance();

	// シーンの静的初期化
	scene_->StaticInitialize(this);

	// シーン(タイトル)
	scene_.reset(sceneFacyory_->CreateScene(kTitle));
	scene_->Initialize();

	// 初期シーン
	currentSceneNo_ = kTitle;

	requestSeneNo_ = kTitle; // リクエストシーン
	prevRequestSeneNo_ = kTitle; // 前のリクエストシーン

	// シーン遷移ファクトリー
	sceneTransitionFactory_ = SceneTransitionFactory::GetInstance();

	// シーン遷移を保持するメンバ変数
	sceneTransition_ = nullptr;

	AudioInitialize();

	Audio::GetInstance()->PlayWave(Audio::AudioHandleIndex::kBGM, true, 0.2f);

}

void SceneManager::Update()
{

	// シーンのチェック
	currentSceneNo_ = scene_->GetSceneNo();

	prevRequestSeneNo_ = requestSeneNo_; // 前のリクエストシーン
	requestSeneNo_ = scene_->GetRequestSceneNo(); // リクエストシーン

	// シーン変更チェック
	//if (prevSceneNo_ != currentSceneNo_) {
	//	TextureManager::GetInstance()->ResetTexture();
	//	scene_.reset(sceneFacyory_->CreateScene(currentSceneNo_));
	//	scene_->Initialize();
	//}

	// リクエストシーンが変わったか
	if (requestSeneNo_ != prevRequestSeneNo_) {
		//シーン遷移開始（初期化）
		sceneTransition_.reset(sceneTransitionFactory_->CreateSceneTransition(currentSceneNo_, requestSeneNo_));
		sceneTransition_->Initialize();
	}

	//シーン遷移中
	if (sceneTransition_) {
		// シーン遷移更新
		sceneTransition_->Update();
		if (sceneTransition_->GetSwitchScene()) {
			// シーン切り替え
			TextureManager::GetInstance()->ResetTexture();
			TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance());
			currentSceneNo_ = requestSeneNo_;
			scene_.reset(sceneFacyory_->CreateScene(currentSceneNo_));
			scene_->Initialize();
			sceneTransition_->SetSwitchScene(false);
			if (currentSceneNo_ == kGame) {
				static_cast<GameScene*>(scene_->GetInstance())->LoadStage(stageNum_);
			}
				
		}
		else if (!sceneTransition_->GetTransitioning()) {
			sceneTransition_.release();
			sceneTransition_ = nullptr;
		}
	}

	// 更新処理
	scene_->Update();

}

void SceneManager::Draw()
{
	// 描画処理
	scene_->Draw();
	if (sceneTransition_) {
		sceneTransition_->Draw();
	}

}

void SceneManager::AudioInitialize()
{

	Audio* audio = Audio::GetInstance();

	//	kBGM,
	audio->Audio::LoadWave("TD2_November/audio/BGM.wav");
	//	kTitleButton,
	audio->Audio::LoadWave("TD2_November/audio/title.wav");
	//	kRecoveryItem,
	audio->Audio::LoadWave("TD2_November/audio/recoveryItem.wav");
	//	kCollectibleItem,
	audio->Audio::LoadWave("TD2_November/audio/collectibleItem.wav");
	//	kBreakBox,
	audio->Audio::LoadWave("TD2_November/audio/breakBox.wav");
	//	kGoal,
	audio->Audio::LoadWave("TD2_November/audio/goal.wav");
	// kExplosion
	audio->Audio::LoadWave("TD2_November/audio/explosion.wav");

}
