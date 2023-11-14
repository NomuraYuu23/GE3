#include "BurningBoxManager.h"
#include"../../externals/imgui/imgui.h"
#include <iterator>


BurningBoxManager::~BurningBoxManager(){
	//解放
	for (BurningBox* BurningBox_: BurningBoxes_){
		delete BurningBox_;
	}
}

void BurningBoxManager::Initialize(Model* model, Material* material){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;
}

void BurningBoxManager::Update(){
	
	for (BurningBox* BurningBox_ : BurningBoxes_) {
		BurningBox_->Update();
	}
	
}

void BurningBoxManager::Draw(const ViewProjection& viewProjection){
	for (BurningBox* BurningBox_ : BurningBoxes_) {
		BurningBox_->Draw(viewProjection);
	}
}

void BurningBoxManager::DrawImgui(){	
	int i = 0;
	
	for (BurningBox* BurningBox : BurningBoxes_) {
		if (ImGui::TreeNode((std::to_string(i+1) + "個目の箱").c_str())) {
			BurningBox->DrawImgui();
			
			ImGui::TreePop();
		}
		i++;
	}
	
	
}

void BurningBoxManager::AddBurningBox(TransformStructure BurningBox, bool isMoving, bool isVertical){
	BurningBox* BurningBox_ = new BurningBox();
	BurningBox_->Initialize(model_, material_, BurningBox, isMoving, isVertical);

	BurningBoxes_.push_back(BurningBox_);

	colliderDebugDraw_->AddCollider(&BurningBox_->GetCollider());
}
