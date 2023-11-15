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
			
			ImGui::TreePop();
		}
		i++;
	}
	
	
}

void BoxManager::AddBox(TransformStructure box, bool isMoving, bool isVertical){
	Box* box_ = new Box();
	box_->Initialize(model_, material_, box, isMoving, isVertical);

	boxes_.push_back(box_);

	colliderDebugDraw_->AddCollider(&box_->GetCollider());
}

void BoxManager::SaveFile(){
	//保存
	json root;
	root = json::object();
	int i = 0;

	for (Box* box : boxes_) {
		root[kItemName_][i][0] = json::array(
			{ box->GetDrawWorldTransform().transform_.scale.x,
			  box->GetDrawWorldTransform().transform_.scale.y,
			  box->GetDrawWorldTransform().transform_.scale.z
			});
		root[kItemName_][i][1] = json::array(
			{ box->GetDrawWorldTransform().transform_.rotate.x,
			  box->GetDrawWorldTransform().transform_.rotate.y,
			  box->GetDrawWorldTransform().transform_.rotate.z
			});
		root[kItemName_][i][2] = json::array(
			{ box->GetDrawWorldTransform().transform_.translate.x,
			  box->GetDrawWorldTransform().transform_.translate.y,
			  box->GetDrawWorldTransform().transform_.translate.z
			});
		std::filesystem::path dir(kDirectoryPath);
		if (!std::filesystem::exists(kDirectoryName)) {
			std::filesystem::create_directory(kDirectoryName);
		}
		// 書き込むjsonファイルのフルパスを合成する
		std::string filePath = kDirectoryPath + kItemName_ + ".json";
		// 書き込み用ファイルストリーム
		std::ofstream ofs;
		// ファイルを書き込みように開く
		ofs.open(filePath);
		//ファイルオープン失敗
		if (ofs.fail()) {
			std::string message = "Failed open data file for write.";
			MessageBoxA(nullptr, message.c_str(), "Element", 0);
			assert(0);
			break;
		}
		//ファイルにjson文字列を書き込む(インデント幅4)
		ofs << std::setw(4) << root << std::endl;
		//ファイルを閉じる
		ofs.close();
		i++;
	}
}

void BoxManager::ChackFiles(){
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Box", 0);
		assert(0);
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);

	for (const std::filesystem::directory_entry& entry : dir_it) {
		//ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		if (LoadChackItem(kDirectoryPath, filePath.stem().string())) {
			chackOnlyNumber = 1;
		}

		if (fileName.size() != 0) {
			bool noneFail = true;
			for (size_t i = 0; i < fileName.size(); i++) {
				if (fileName[i].c_str() == filePath.stem().string()) {
					noneFail = false;
				}
			}
			if (noneFail) {
				fileName.push_back(filePath.stem().string());
			}

		}
		else {
			//ファイルの名前を取得
			fileName.push_back(filePath.stem().string());
		}
	}
}

void BoxManager::LoadFiles(){

	if (!LoadChackItem(kDirectoryPath, kItemName_))
		return;
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Animetion", 0);
		assert(0);
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);

	for (const std::filesystem::directory_entry& entry : dir_it) {
		//ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		if (filePath.stem().string() == kItemName_) {
			//ファイル読み込み
			LoadFile(filePath.stem().string());
			return;
		}

		
	}
}

void BoxManager::LoadFile(const std::string& groupName){
	//読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//読み込み用のファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Adjustment_Item", 0);
		assert(0);
		return;
	}
	boxes_.clear();

	nlohmann::json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();
	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);
	//未登録チェック
	assert(itGroup != root.end());
	//各アイテムについて
	for (const auto& i:root[groupName]){
		int count = 0;
		TransformStructure newTrans{};
		for (const auto& j:i){
			Vector3 v{};
			from_json(j, v);
			if (count == 0) {
				newTrans.scale = v;
			}
			else if (count == 1) {
				newTrans.rotate = v;
			}
			else if (count == 2) {
				newTrans.translate = v;
			}
			count++;

		}

		Box* box_ = new Box();
		box_->Initialize(model_, material_, newTrans, false, false);

		boxes_.push_back(box_);

		colliderDebugDraw_->AddCollider(&box_->GetCollider());
	}

}

bool BoxManager::LoadChackItem(const std::string& directoryPath, const std::string& itemName){

	// 書き込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + itemName + ".json";
	//読み込み用のファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Adjustment_Item", 0);
		ifs.close();
		return false;
	}
	nlohmann::json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();
	//グループを検索
	nlohmann::json::iterator itGroup = root.find(itemName);
	//未登録チェック
	if (itGroup != root.end()) {
		return true;
	}
	else {
		return false;
	}
}

void BoxManager::from_json(const json& j, Vector3& v){
	v.x = j.at(0).get<float>();
	v.y = j.at(1).get<float>();
	v.z = j.at(2).get<float>();
}
