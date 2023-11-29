#include "BreakBoxManager.h"
#include"../../externals/imgui/imgui.h"
#include <iterator>
#include "../ShadowManager/ShadowManager.h"


BreakBoxManager::~BreakBoxManager(){
	//解放
	for (BreakBox* box_ : breakBoxes_) {
		delete box_;
	}
}

void BreakBoxManager::Initialize(Model* model, Material* material){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;
}

void BreakBoxManager::Update(){
	//ブレイクフラグの立ったブロックのコライダー描画を削除
	for (BreakBox* breakBox : breakBoxes_) {
		if (breakBox->GetIsBreak()) {
			colliderDebugDraw_->DeleteCollider(&breakBox->GetCollider());
		}
	}
	//ブレイクフラグの立ったブロックを削除
	breakBoxes_.remove_if([](BreakBox* breakBox) {
		if (breakBox->GetIsBreak()){			
			delete breakBox;

			return true;
		}
		return false;
			
	});

	for (BreakBox* box_ : breakBoxes_) {
		 box_->Update();
	}
}

void BreakBoxManager::Draw(const ViewProjection& viewProjection){
	for (BreakBox* box_ : breakBoxes_) {
		box_->Draw(viewProjection);
	}
}

void BreakBoxManager::DrawImgui(){
	int i = 0;

	for (BreakBox* box : breakBoxes_) {
		if (ImGui::TreeNode((std::to_string(i + 1) + "個目の壊れる箱").c_str())) {
			box->DrawImgui();
			
			ImGui::TreePop();
		}
		i++;
	}
}

void BreakBoxManager::AddBox(TransformStructure box, bool isMoving, bool isVertical){
	BreakBox* box_ = new BreakBox();
	box_->Initialize(model_, material_, box, box.translate, isMoving, isVertical, { 20.0f,0.0f,0.0f });

	breakBoxes_.push_back(box_);

	colliderDebugDraw_->AddCollider(&box_->GetCollider());
}

void BreakBoxManager::SaveFile(const std::vector<std::string>& stages){
	//保存
	json root;
	root = json::object();
	root[kItemName_] = json::object();

	for (size_t j = 0; j < stages.size(); ++j) {
		int i = 0;

		for (BreakBox* box : breakBoxes_) {
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
				MessageBoxA(nullptr, message.c_str(), "Stages", 0);
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

void BreakBoxManager::FileOverWrite(const std::string& stage){
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
	for (BreakBox* box : breakBoxes_) {
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
		overWrite[i][3] = box->GetMoveFlag();
		overWrite[i][4] = box->GetVerticalFlag();
		overWrite[i][5] = json::array(
			{ box->GetPosition().x,
				box->GetPosition().y,
				box->GetPosition().z
			});
		overWrite[i][6] = json::array(
			{ box->GetBaseMoveLength_().x,
				box->GetBaseMoveLength_().y,
				box->GetBaseMoveLength_().z
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

void BreakBoxManager::ChackFiles(){
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "BreakBox", 0);
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

void BreakBoxManager::LoadFiles(const std::string& stage){
	if (!LoadChackItem(kDirectoryPath, kItemName_))
		return;
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "BreakBox", 0);
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

void BreakBoxManager::LoadFile(const std::string& groupName, const std::string& stage){
	//読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//読み込み用のファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "BreakBox", 0);
		assert(0);
		return;
	}

	nlohmann::json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();
	//元データの削除
	for (BreakBox* box : breakBoxes_) {
		colliderDebugDraw_->DeleteCollider(&box->GetCollider());
	}

	breakBoxes_.clear();
	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);
	//未登録チェック
	assert(itGroup != root.end());
	//各アイテムについて
	for (const auto& i : root[groupName][stage]) {
		int count = 0;
		TransformStructure newTrans{};
		Vector3 pos{};
		Vector3 baseLength{};
		bool isNewMove = false;
		bool isVerticalMove = false;
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
				isNewMove = j;
			}
			else if (count == 4) {
				isVerticalMove = j;
			}
			else if (count == 5) {
				from_json(j, v);
				pos = v;
			}
			else if (count == 6) {
				from_json(j, v);
				baseLength = v;
			}
			count++;

		}

		BreakBox* box_ = new BreakBox();
		box_->Initialize(model_, material_, newTrans, pos, isNewMove, isVerticalMove, baseLength);

		breakBoxes_.push_back(box_);

		colliderDebugDraw_->AddCollider(&box_->GetCollider());/*
		Vector3 size = box_->GetDrawWorldTransform().transform_.scale;
		size.x *= 2.0f;
		size.y *= 2.0f;
		size.z *= 2.0f;
		ShadowManager::GetInstance()->AddFloor(box_->GetDrawWorldTransformAdress(), size);*/

	}
	
}

bool BreakBoxManager::LoadChackItem(const std::string& directoryPath, const std::string& itemName){

	// 書き込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + itemName + ".json";
	//読み込み用のファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "BreakBox", 0);
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

void BreakBoxManager::from_json(const json& j, Vector3& v){
	v.x = j.at(0).get<float>();
	v.y = j.at(1).get<float>();
	v.z = j.at(2).get<float>();
}
