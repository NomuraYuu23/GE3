#include "SceneManager.h"
#include "../../base/TextureManager.h"

void SceneManager::Initialize()
{

	// シーンファクトリー
	sceneFacyory_ = SceneFactory::GetInstance();

	// シーン(タイトル)
	scene_.reset(sceneFacyory_->CreateScene(kTitle));
	scene_->Initialize();

	// 初期シーン
	currentSceneNo_ = kTitle;
	prevSceneNo_ = kTitle;

	// シーンの静的初期化
	scene_->StaticInitialize();

}

void SceneManager::Update()
{

	// シーンのチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = scene_->GetSceneNo();

	// シーン変更チェック
	if (prevSceneNo_ != currentSceneNo_) {
		TextureManager::GetInstance()->ResetTexture();
		scene_.reset(sceneFacyory_->CreateScene(currentSceneNo_));
		scene_->Initialize();
	}

	// 更新処理
	scene_->Update();

}

void SceneManager::Draw()
{
	// 描画処理
	scene_->Draw();
}
