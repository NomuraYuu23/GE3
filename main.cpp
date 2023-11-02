#include <Windows.h>

#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/3D/VertexData.h"
#include "Engine/3D/TransformationMatrix.h"
#include "Engine/3D/TransformStructure.h"
#include "Engine/3D/MaterialData.h"
#include "Engine/3D/DirectionalLightData.h"
#include "Engine/base/SafeDelete.h"
#include "Engine/base/GraphicsPipelineState.h"
//クラス化
#include "Engine/base/WinApp.h"
#include "Engine/base/DirectXCommon.h"
#include "Engine/base/TextureManager.h"
#include "Engine/2D/Sprite.h"
#include "Engine/3D/Model.h"
#include "Engine/3D/Material.h"
#include "Engine/3D/DirectionalLight.h"
#include "Engine/base/D3DResourceLeakChecker.h"

// サウンド再生
#include "Engine/Audio/Audio.h"

//入力デバイス
#include "Engine/Input/Input.h"

//デバッグカメラ
#include "Engine/Camera/DebugCamera.h"
#include "Engine/Scene/GameScene.h"

// ImGui
#include "Engine/2D/ImGuiManager.h"

// グローバル変数
#include "Engine/GlobalVariables/GlobalVariables.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;

	Audio* audio = nullptr;
	Input* input = nullptr;

	ImGuiManager* imGuiManager = nullptr;

	//ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	//DirectX初期化
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

	//入力デバイス
	input = Input::GetInstance();
	input->Initialize(win->GetHInstance(), win->GetHwnd());

	GraphicsPipelineState::InitializeGraphicsPipeline(dxCommon->GetDevice());

	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), GraphicsPipelineState::sRootSignature, GraphicsPipelineState::sPipelineState[GraphicsPipelineState::Sprite]);

	// モデル静的初期化
	Model::StaticInitialize(dxCommon->GetDevice(), GraphicsPipelineState::sRootSignature, GraphicsPipelineState::sPipelineState[GraphicsPipelineState::Model]);

	// マテリアル静的初期化
	Material::StaticInitialize(dxCommon->GetDevice());
	
	// 光源静的初期化
	DirectionalLight::StaticInitialize(dxCommon->GetDevice());

	//サウンド
	audio = Audio::GetInstance();
	audio->Initialize();

	// ImGuiマネージャー
	imGuiManager = ImGuiManager::GetInstance();
	imGuiManager->Initialize(win,dxCommon, TextureManager::GetInstance());

	//グローバル変数ファイル読み込み
	GlobalVariables::GetInstance()->LoadFiles();

	// リリースチェッカー
	D3DResourceLeakChecker leakChecker;

	//ゲームシーン
	std::unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();
	gameScene->Initialize();

	//ウィンドウののボタンが押されるまでループ
	while (true) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (win->ProcessMessage()) {
			break;
		}

		//入力デバイス
		input->Update();
		if (input->PushKey(DIK_SPACE)) {
			input->JoystickConnected(win->GetHwnd());
		}

		//ゲームの処理 
		//ImGui
		imGuiManager->Begin();

		//開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
		ImGui::ShowDemoWindow();
		// グローバル変数の更新
		GlobalVariables::GetInstance()->Update();

		// ゲームシーン更新
		gameScene->Update();

		//描画前処理
		dxCommon->PreDraw();

		//ゲームシーン描画処理
		gameScene->Draw();

		imGuiManager->End();

		// シェーダーリソースビューをセット
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(dxCommon->GetCommadList(), 2, 0);
		//実際のcommandListのImGuiの描画コマンドを積む
		imGuiManager->Draw();

		//描画後処理
		dxCommon->PostDraw();

	}

	//出力ウインドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	// サウンド後始末
	audio->Finalize();

	//色々な解放処理の前に書く
	imGuiManager->Finalize();

	//ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}
