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

void CollectibleItemManager::SaveFile(const std::vector<std::string>& stages){
	//保存
	json root;
	root = json::object();
	root[kItemName_] = json::object();

	for (size_t j = 0; j < stages.size(); ++j) {
		int i = 0;

		for (CollectibleItem* box : collectibleItems_) {
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

void CollectibleItemManager::FileOverWrite(const std::string& stage){
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
	for (CollectibleItem* box : collectibleItems_) {
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
		overWrite[i][3] = box->GetisFalling();
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

void CollectibleItemManager::ChackFiles(){
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

void CollectibleItemManager::LoadFiles(const std::string& stage){
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
			LoadFile(filePath.stem().string(), stage);
			return;
		}

		
	}
}

void CollectibleItemManager::LoadFile(const std::string& groupName, const std::string& stage){
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
	for (CollectibleItem* box : collectibleItems_) {
		colliderDebugDraw_->DeleteCollider(&box->GetCollider());
	}

	collectibleItems_.clear();


	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);
	//未登録チェック
	assert(itGroup != root.end());
	//各アイテムについて
	for (const auto& i : root[groupName][stage]) {
		int count = 0;
		TransformStructure newTrans{};
		bool isFall = false;
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
				isFall = j;
			}
			count++;

		}

		CollectibleItem* box_ = new CollectibleItem();
		box_->Initialize(model_, material_, newTrans, isFall);

		collectibleItems_.push_back(box_);

		colliderDebugDraw_->AddCollider(&box_->GetCollider());
	}
}

bool CollectibleItemManager::LoadChackItem(const std::string& directoryPath, const std::string& itemName){
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

void CollectibleItemManager::from_json(const json& j, Vector3& v){
	v.x = j.at(0).get<float>();
	v.y = j.at(1).get<float>();
	v.z = j.at(2).get<float>();
}
