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
	for (CheckPoint* box : checkPoints_) {
		if (box->GetDeleteFlag()) {
			colliderDebugDraw_->DeleteCollider(&box->GetCollider());
		}
	}
	//ブレイクフラグの立ったブロックを削除
	checkPoints_.remove_if([](CheckPoint* breakBox) {
		if (breakBox->GetDeleteFlag()) {
			delete breakBox;

			return true;
		}
		return false;

		});

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

void CheckPointManager::SaveFile(const std::vector<std::string>& stages){
	//保存
	json root;
	root = json::object();
	root[kItemName_] = json::object();

	for (size_t j = 0; j < stages.size(); ++j) {
		int i = 0;
		for (CheckPoint* box : checkPoints_) {
			root[kItemName_][stages[j].c_str()][i][0] = json::array(
				{ box->GetDrawWorldTransform().transform_.scale.x,
				  box->GetDrawWorldTransform().transform_.scale.y,
				  box->GetDrawWorldTransform().transform_.scale.z
				});
			root[kItemName_][stages[j].c_str()][i][1] = json::array(
				{ box->GetDrawWorldTransform().transform_.rotate.x,
				  box->GetDrawWorldTransform().transform_.rotate.y,
				  box->GetDrawWorldTransform().transform_.rotate.z
				});
			root[kItemName_][stages[j].c_str()][i][2] = json::array(
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
	
}

void CheckPointManager::FileOverWrite(const std::string& stage){
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
	for (CheckPoint* box : checkPoints_) {
		overWrite[i][0] = json::array(
			{ box->GetDrawWorldTransform().transform_.scale.x,
				box->GetDrawWorldTransform().transform_.scale.y,
				box->GetDrawWorldTransform().transform_.scale.z
			});
		overWrite[i][1] = json::array(
			{ box->GetDrawWorldTransform().transform_.rotate.x,
				box->GetDrawWorldTransform().transform_.rotate.y,
				box->GetDrawWorldTransform().transform_.rotate.z
			});
		overWrite[i][2] = json::array(
			{ box->GetDrawWorldTransform().transform_.translate.x,
				box->GetDrawWorldTransform().transform_.translate.y,
				box->GetDrawWorldTransform().transform_.translate.z
			});
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

void CheckPointManager::ChackFiles(){
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

void CheckPointManager::LoadFiles(const std::string& stage){
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

void CheckPointManager::LoadFile(const std::string& groupName, const std::string& stage){
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
	for (CheckPoint* box : checkPoints_) {
		colliderDebugDraw_->DeleteCollider(&box->GetCollider());
	}

	checkPoints_.clear();


	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);
	//未登録チェック
	assert(itGroup != root.end());
	//各アイテムについて
	for (const auto& i : root[groupName][stage]) {
		int count = 0;
		TransformStructure newTrans{};
		for (const auto& j : i) {
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

		CheckPoint* box_ = new CheckPoint();
		box_->Initialize(model_, material_, newTrans);

		checkPoints_.push_back(box_);

		colliderDebugDraw_->AddCollider(&box_->GetCollider());
	}
	
}

bool CheckPointManager::LoadChackItem(const std::string& directoryPath, const std::string& itemName){

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

void CheckPointManager::from_json(const json& j, Vector3& v){
	v.x = j.at(0).get<float>();
	v.y = j.at(1).get<float>();
	v.z = j.at(2).get<float>();
}
