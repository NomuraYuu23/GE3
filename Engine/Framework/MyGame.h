#pragma once

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix4x4.h"
#include "../3D/VertexData.h"
#include "../3D/TransformationMatrix.h"
#include "../3D/TransformStructure.h"
#include "../3D/MaterialData.h"
#include "../3D/DirectionalLightData.h"
#include "../base/SafeDelete.h"
#include "../base/GraphicsPipelineState.h"
//クラス化
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "../base/TextureManager.h"
#include "../2D/Sprite.h"
#include "../3D/Model.h"
#include "../3D/Material.h"
#include "../3D/DirectionalLight.h"
#include "../base/D3DResourceLeakChecker.h"

// サウンド再生
#include "../Audio/Audio.h"

//入力デバイス
#include "../Input/Input.h"

//デバッグカメラ
#include "../Camera/DebugCamera.h"

// シーンマネージャー
#include "../Scene/SceneManager/SceneManager.h"

// ImGui
#include "../2D/ImGuiManager.h"

// グローバル変数
#include "../GlobalVariables/GlobalVariables.h"

class MyGame
{

public: // メンバ関数

	void Initialize();

	void Finalize();

	void Update();

	void Draw();

public: // アクセッサ

	bool GetEndRequst() { return endRequst_; }

private: // メンバ変数

	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;

	Audio* audio = nullptr;
	Input* input = nullptr;

	ImGuiManager* imGuiManager = nullptr;

	std::unique_ptr<SceneManager> sceneManager = nullptr;

	bool endRequst_ = false;

};

