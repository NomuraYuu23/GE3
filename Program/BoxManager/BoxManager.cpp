#include "BoxManager.h"
#include"../../externals/imgui/imgui.h"
#include <iterator>


BoxManager::~BoxManager(){
	//解放
	for (Box* box_: boxes_){
		delete box_;
	}
}

void BoxManager::Initialize(Model* model, Material* material){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;
}

void BoxManager::Update(){
	
	for (Box* box_ : boxes_) {
		box_->Update();
	}
	
}

void BoxManager::Draw(const ViewProjection& viewProjection){
	for (Box* box_ : boxes_) {
		box_->Draw(viewProjection);
	}
}

void BoxManager::DrawImgui(){	
	int i = 0;
	
	for (Box* box : boxes_) {
		if (ImGui::TreeNode((std::to_string(i+1) + "個目の箱").c_str())) {
			box->DrawImgui();
			i++;
			ImGui::TreePop();
		}
	}
	
	
}

void BoxManager::AddBox(TransformStructure box, bool isMoving, bool isVertical){
	Box* box_ = new Box();
	box_->Initialize(model_, material_, box, isMoving, isVertical);

	boxes_.push_back(box_);

	colliderDebugDraw_->AddCollider(&box_->GetCollider());
}
