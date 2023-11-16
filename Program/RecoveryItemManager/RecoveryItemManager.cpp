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

void RecoveryItemManager::AddItem(TransformStructure Item, int recoveryValue /*bool isMoving, bool isVertical*/){
	RecoveryItem* item_ = new RecoveryItem();
	item_->Initialize(model_, material_, Item, recoveryValue);

	recoveryItems_.push_back(item_);

	colliderDebugDraw_->AddCollider(&item_->GetCollider());
}

void RecoveryItemManager::SaveFile(const std::vector<std::string>& stages) {
	//保存
	json root;
	root = json::object();
	root[kItemName_] = json::object();

	for (size_t j = 0; j < stages.size(); ++j) {
		int i = 0;

		for (RecoveryItem* box : recoveryItems_) {
			root[kItemName_][stages[j].c_str()][i][0] = json::array(
				{ box->GetMakeWorldTransform().transform_.scale.x,
				  box->GetMakeWorldTransform().transform_.scale.y,
				  box->GetMakeWorldTransform().transform_.scale.z
				});
			root[kItemName_][stages[j].c_str()][i][1] = json::array(
				{ box->GetMakeWorldTransform().transform_.rotate.x,
				  box->GetMakeWorldTransform().transform_.rotate.y,
				  box->GetMakeWorldTransform().transform_.rotate.z
				});
			root[kItemName_][stages[j].c_str()][i][2] = json::array(
				{ box->GetMakeWorldTransform().transform_.translate.x,
				  box->GetMakeWorldTransform().transform_.translate.y,
				  box->GetMakeWorldTransform().transform_.translate.z
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
}

void RecoveryItemManager::FileOverWrite(const std::string& stage){
	//読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + kItemName_ + ".json";
	//読み込み用のファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);

	//上書き用に読み取り
	json root;
	ifs >> root;
	ifs.close();

	json overWrite;

	int i = 0;
	for (RecoveryItem* box : recoveryItems_) {
		overWrite[i][0] = json::array(
			{ box->GetMakeWorldTransform().transform_.scale.x,
				box->GetMakeWorldTransform().transform_.scale.y,
				box->GetMakeWorldTransform().transform_.scale.z
			});
		overWrite[i][1] = json::array(
			{ box->GetMakeWorldTransform().transform_.rotate.x,
				box->GetMakeWorldTransform().transform_.rotate.y,
				box->GetMakeWorldTransform().transform_.rotate.z
			});
		overWrite[i][2] = json::array(
			{ box->GetMakeWorldTransform().transform_.translate.x,
				box->GetMakeWorldTransform().transform_.translate.y,
				box->GetMakeWorldTransform().transform_.translate.z
			});
		overWrite[i][3] = box->GetRecoveryValue();
		i++;
	}

	root[kItemName_][stage] = overWrite;

	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);
	//ファイルオープン失敗
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Element", 0);
		assert(0);
		return;
	}
	//ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();
}

void RecoveryItemManager::ChackFiles() {
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

void RecoveryItemManager::LoadFiles(const std::string& stage) {

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
			LoadFile(filePath.stem().string(),stage);
			return;
		}

		
	}
}

void RecoveryItemManager::LoadFile(const std::string& groupName, const std::string& stage) {
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

	nlohmann::json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();
	//元データの削除
	for (RecoveryItem* box : recoveryItems_) {
		colliderDebugDraw_->DeleteCollider(&box->GetCollider());
	}

	recoveryItems_.clear();

	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);
	//未登録チェック
	assert(itGroup != root.end());
	//各アイテムについて
	for (const auto& i : root[groupName][stage]) {
		int count = 0;
		TransformStructure newTrans{};
		int newValue = 3;
		for (const auto& j : i) {
			Vector3 v{};
			if (count < 3) {
				from_json(j, v);
			}
			if (count == 0) {
				newTrans.scale = v;
			}
			else if (count == 1) {
				newTrans.rotate = v;
			}
			else if (count == 2) {
				newTrans.translate = v;
			}
			else if (count == 3) {
				newValue = j.get<int>();
			}
			count++;

		}

		RecoveryItem* box_ = new RecoveryItem();
		box_->Initialize(model_, material_, newTrans, newValue);

		recoveryItems_.push_back(box_);

		colliderDebugDraw_->AddCollider(&box_->GetCollider());
	}

}

bool RecoveryItemManager::LoadChackItem(const std::string& directoryPath, const std::string& itemName) {

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

void RecoveryItemManager::from_json(const json& j, Vector3& v) {
	v.x = j.at(0).get<float>();
	v.y = j.at(1).get<float>();
	v.z = j.at(2).get<float>();
}
