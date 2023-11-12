#include "BreakBoxManager.h"
#include"../../externals/imgui/imgui.h"
#include <iterator>


BreakBoxManager::~BreakBoxManager(){
	//解放
	for (BreakBox* box_ : breakBoxes_) {
		delete box_;
	}
}

void BreakBoxManager::Initialize(Model* model, Material* material){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;
}

void BreakBoxManager::Update(){
	//ブレイクフラグの立ったブロックのコライダー描画を削除
	for (BreakBox* breakBox : breakBoxes_) {
		if (breakBox->GetIsBreak()) {
			colliderDebugDraw_->DeleteCollider(&breakBox->GetCollider());
		}
	}
	//ブレイクフラグの立ったブロックを削除
	breakBoxes_.remove_if([](BreakBox* breakBox) {
		if (breakBox->GetIsBreak()){			
			delete breakBox;

			return true;
		}
		return false;
			
	});

	for (BreakBox* box_ : breakBoxes_) {
		 box_->Update();
	}
}

void BreakBoxManager::Draw(const ViewProjection& viewProjection){
	for (BreakBox* box_ : breakBoxes_) {
		box_->Draw(viewProjection);
	}
}

void BreakBoxManager::DrawImgui(){
	int i = 0;

	for (BreakBox* box : breakBoxes_) {
		if (ImGui::TreeNode((std::to_string(i + 1) + "個目の壊れる箱").c_str())) {
			box->DrawImgui();
			
			ImGui::TreePop();
		}
		i++;
	}
}

void BreakBoxManager::AddBox(TransformStructure box, bool isMoving, bool isVertical){
	BreakBox* box_ = new BreakBox();
	box_->Initialize(model_, material_, box, isMoving, isVertical);

	breakBoxes_.push_back(box_);

	colliderDebugDraw_->AddCollider(&box_->GetCollider());
}
