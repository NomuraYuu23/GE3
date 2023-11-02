#include "ImGuiManager.h"

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon, TextureManager* textureManager)
{
	
	dxCommon_ = dxCommon;

	textureManager_ = textureManager;

	//ImGuiの初期化。

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(dxCommon_->GetDevice(),
		2,								 // ダブルバッファ
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, // SRGB
		textureManager_->StaticGetDescriptorHeap(),
		textureManager_->StaticGetCPUDescriptorHandle(),
		textureManager_->StaticGetGPUDescriptorHandle());

}

void ImGuiManager::Finalize()
{
	//色々な解放処理の前に書く
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	//ImGuiの内部コマンドを生成する
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon_->GetCommadList());
}
