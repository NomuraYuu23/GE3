#include"RecoveryItemManager.h"
#include"../../Engine/2D/ImguiManager.h"

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
	//ブレイクフラグの立ったブロックのコライダー描画を削除
	for (RecoveryItem* item : recoveryItems_) {
		if (item->GetIsDelete()) {
			colliderDebugDraw_->DeleteCollider(&item->GetCollider());
		}
	}
	//ブレイクフラグの立ったブロックを削除
	recoveryItems_.remove_if([](RecoveryItem* item) {
		if (item->GetIsDelete()) {
			delete item;

			return true;
		}
		return false;

		});

	for (RecoveryItem* item : recoveryItems_) {
		item->Update();
	}
}

void RecoveryItemManager::Draw(const ViewProjection& viewProjection){
	for (RecoveryItem* item : recoveryItems_) {
		item->Draw(viewProjection);
	}
}

void RecoveryItemManager::DrawImgui(){
	int i = 0;

	for (RecoveryItem* item : recoveryItems_) {
		if (ImGui::TreeNode((std::to_string(i + 1) + "個目の回復アイテム").c_str())) {
			item->DrawImgui();
			
			ImGui::TreePop();
		}
		i++;
	}
}

void RecoveryItemManager::AddItem(TransformStructure Item, bool isMoving, bool isVertical){
	RecoveryItem* item_ = new RecoveryItem();
	item_->Initialize(model_, material_, Item);

	recoveryItems_.push_back(item_);

	colliderDebugDraw_->AddCollider(&item_->GetCollider());
}
