#include"CollectibleItemManager.h"
#include"../../Engine/2D/ImguiManager.h"

CollectibleItemManager::~CollectibleItemManager(){
	//解放
	for (CollectibleItem* CollectibleItem_ : collectibleItems_) {
		delete CollectibleItem_;
	}
}

void CollectibleItemManager::Initialize(Model* model, Material* material){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;
}

void CollectibleItemManager::Update(){
	//ブレイクフラグの立ったブロックのコライダー描画を削除
	for (CollectibleItem* item : collectibleItems_) {
		if (item->GetIsDelete()) {
			colliderDebugDraw_->DeleteCollider(&item->GetCollider());
		}
	}
	//ブレイクフラグの立ったブロックを削除
	collectibleItems_.remove_if([](CollectibleItem* item) {
		if (item->GetIsDelete()) {
			delete item;

			return true;
		}
		return false;

		});

	for (CollectibleItem* item : collectibleItems_) {
		item->Update();
	}
}

void CollectibleItemManager::Draw(const ViewProjection& viewProjection){
	for (CollectibleItem* item : collectibleItems_) {
		item->Draw(viewProjection);
	}
}

void CollectibleItemManager::DrawImgui(){
	int i = 0;

	for (CollectibleItem* item : collectibleItems_) {
		if (ImGui::TreeNode((std::to_string(i + 1) + "個目の回復アイテム").c_str())) {
			item->DrawImgui();
			
			ImGui::TreePop();
		}
		i++;
	}
}

void CollectibleItemManager::AddItem(TransformStructure Item, bool isFalling){
	CollectibleItem* item_ = new CollectibleItem();
	item_->Initialize(model_, material_, Item,isFalling);

	collectibleItems_.push_back(item_);

	colliderDebugDraw_->AddCollider(&item_->GetCollider());
}
