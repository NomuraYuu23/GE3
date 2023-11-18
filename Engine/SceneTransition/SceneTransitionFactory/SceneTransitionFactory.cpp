#include "SceneTransitionFactory.h"
#include "../../Scene/IScene/IScene.h"

SceneTransitionFactory* SceneTransitionFactory::GetInstance()
{
	static SceneTransitionFactory instance;
	return &instance;
}

ISceneTransition* SceneTransitionFactory::CreateSceneTransition(int sceneName, int requestSeneName)
{

	ISceneTransition* newSceneTransition = nullptr;

	// タイトルからゲーム
	if (sceneName == SceneName::kTitle && requestSeneName == SceneName::kGame) {

	}
	else {
		assert(0);
	}

	return newSceneTransition;
}
