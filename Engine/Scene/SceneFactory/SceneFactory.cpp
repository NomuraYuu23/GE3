#include "SceneFactory.h"
#include "../TitleScene/TitleScene.h"
#include "../SelectScene/SelectScene.h"
#include "../GameScene/GameScene.h"
#include "../ClearScene/ClearScene.h"

SceneFactory* SceneFactory::GetInstance()
{
	static SceneFactory instance;
	return &instance;
}

IScene* SceneFactory::CreateScene(int sceneName)
{

    // 次のシーンを生成
    IScene* newScene = nullptr;

	switch (sceneName)
	{
	case kTitle:  // タイトル
		newScene = new TitleScene();
		break;
	case kSelect: // セレクト
		newScene = new SelectScene();
		break;
	case kGame:   // ゲーム
		newScene = new GameScene();
		break;
	case kClear:  // クリア
		newScene = new ClearScene();
		break;
	case kCountOfSceneName: // 使用不可
		break;
	default:
		break;
	}

    return newScene;
}
