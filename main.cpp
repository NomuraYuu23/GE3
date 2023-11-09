#include <Windows.h>

#include "Engine/Framework/MyGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	MyGame game;

	game.Initialize();

	//ウィンドウののボタンが押されるまでループ
	while (true) {
		
		game.Update();
		if (game.GetEndRequst()) {
			break;
		}

		game.Draw();

	}

	game.Finalize();

	return 0;
}
