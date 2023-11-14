#include "CheckPointManager.h"
#include"../../externals/imgui/imgui.h"

CheckPointManager::~CheckPointManager(){
	//解放
	for (CheckPoint* checkpoint:checkPoints_){
		delete checkpoint;
	}
}

void CheckPointManager::Initialize(Model* model, Material* material){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;
}

void CheckPointManager::Update(){
	for (CheckPoint* checkpoint : checkPoints_) {
		checkpoint->Update();
	}
}

void CheckPointManager::Draw(const ViewProjection& viewProjection){
	for (CheckPoint* checkpoint : checkPoints_) {
		checkpoint->Draw(viewProjection);
	}
}

void CheckPointManager::DrawImgui(){
	int i = 0;

	for (CheckPoint* checkpoint : checkPoints_) {
		if (ImGui::TreeNode((std::to_string(i + 1) + "個目の箱").c_str())) {
			checkpoint->DrawImgui();

			ImGui::TreePop();
		}
		i++;
	}
}

void CheckPointManager::AddCheck(TransformStructure box){
	CheckPoint* checkpoint = new CheckPoint();
	checkpoint->Initialize(model_, material_, box);

	checkPoints_.push_back(checkpoint);

	colliderDebugDraw_->AddCollider(&checkpoint->GetCollider());
}
