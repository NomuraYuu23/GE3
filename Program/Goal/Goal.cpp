#include "Goal.h"
#include"../../Engine/2D/ImguiManager.h"

void Goal::Initialize(Model* model, Material* material)
{

	//モデルデータ配列
	model_ = model;
	//マテリアルデータ配列
	material_ = material;
	//ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.transform_.translate = kPosition;
	worldTransform_.UpdateMatrix();

	radius_ = kRadius;

	collider_.Initialize(kPosition, radius_);

}

void Goal::Update(){
	collider_.center_ = worldTransform_.transform_.translate;
	collider_.radius_ = radius_;
	worldTransform_.transform_.scale = { radius_,radius_, radius_ };

	collider_.worldTransformUpdate();
	worldTransform_.UpdateMatrix();

}

void Goal::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection);
}

void Goal::DrawImgui(){
	ImGui::Begin("ゴール");
	ImGui::DragFloat3("座標", &worldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("回転", &worldTransform_.transform_.rotate.x, 0.1f);
	ImGui::DragFloat("大きさ", &radius_, 0.1f);
	ImGui::End();
	
}

void Goal::SaveFile(const std::vector<std::string>& stages){
	//保存
	json root;
	root = json::object();
	root[kItemName_] = json::object();

	for (size_t j = 0; j < stages.size(); ++j) {
		int i = 0;		
		root[kItemName_][stages[j].c_str()][i][0] = json::array(
			{ worldTransform_.transform_.scale.x,
				worldTransform_.transform_.scale.y,
				worldTransform_.transform_.scale.z
			});
		root[kItemName_][stages[j].c_str()][i][1] = json::array(
			{ worldTransform_.transform_.rotate.x,
				worldTransform_.transform_.rotate.y,
				worldTransform_.transform_.rotate.z
			});
		root[kItemName_][stages[j].c_str()][i][2] = json::array(
			{ worldTransform_.transform_.translate.x,
				worldTransform_.transform_.translate.y,
				worldTransform_.transform_.translate.z
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

void Goal::FileOverWrite(const std::string& stage){
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
	
	overWrite[i][0] = json::array(
		{ worldTransform_.transform_.scale.x,
			worldTransform_.transform_.scale.y,
			worldTransform_.transform_.scale.z
		});
	overWrite[i][1] = json::array(
		{ worldTransform_.transform_.rotate.x,
			worldTransform_.transform_.rotate.y,
			worldTransform_.transform_.rotate.z
		});
	overWrite[i][2] = json::array(
		{ worldTransform_.transform_.translate.x,
			worldTransform_.transform_.translate.y,
			worldTransform_.transform_.translate.z
		});

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

void Goal::ChackFiles(){
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

void Goal::LoadFiles(const std::string& stage){
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

void Goal::LoadFile(const std::string& groupName, const std::string& stage){
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

		worldTransform_.transform_ = newTrans;
		radius_ = newTrans.scale.x;
	}
}

bool Goal::LoadChackItem(const std::string& directoryPath, const std::string& itemName){

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

void Goal::from_json(const json& j, Vector3& v){
	v.x = j.at(0).get<float>();
	v.y = j.at(1).get<float>();
	v.z = j.at(2).get<float>();
}
