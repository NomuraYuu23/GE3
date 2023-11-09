#include"RecoveryItemManager.h"

RecoveryItemManager::~RecoveryItemManager(){
	//解放
	for (RecoveryItem* recoveryItem_ : recoveryItems_) {
		delete recoveryItem_;
	}
}

void RecoveryItemManager::Initialize(Model* model, Material* material){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;
}

void RecoveryItemManager::Update(){
	////ブレイクフラグの立ったブロックのコライダー描画を削除
	//for (RecoveryItem* breakBox : breakBoxes_) {
	//	if (breakBox->GetIsBreak()) {
	//		colliderDebugDraw_->DeleteCollider(&breakBox->GetCollider());
	//	}
	//}
	////ブレイクフラグの立ったブロックを削除
	//breakBoxes_.remove_if([](RecoveryItem* breakBox) {
	//	if (breakBox->GetIsBreak()) {
	//		delete breakBox;

	//		return true;
	//	}
	//	return false;

	//	});

	//for (BreakBox* box_ : breakBoxes_) {
	//	box_->Update();
	//}
}

void RecoveryItemManager::Draw(const ViewProjection& viewProjection){

}

void RecoveryItemManager::DrawImgui(){

}

void RecoveryItemManager::AddItem(TransformStructure Item, bool isMoving, bool isVertical){

}
