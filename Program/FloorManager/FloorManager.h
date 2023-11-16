#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/Material.h"
#include "../../Engine/3D/WorldTransform.h"
#include "../Floor/Floor.h"

#include <vector>
#include "../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画
#include"../../externals/nlohmann/json.hpp"
#include<fstream>
#include<iostream>


class FloorManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FloorManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Material* material);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 床追加
	/// </summary>
	void AddFloor(Vector3 position, Vector3 rotate, bool isMoving, bool isVertical);

	/// <summary>
	/// imgui表示
	/// </summary>
	void DrawImgui();

public: // アクセッサ

	/// <summary>
	/// 床リストゲッター
	/// </summary>
	/// <returns></returns>
	std::vector<Floor*> GetFloors() { return floors_; }

	/// <summary>
	/// デバッグ描画セッター
	/// </summary>
	/// <param name="colliderDebugDraw"></param>
	void SetColliderDebugDraw(ColliderDebugDraw* colliderDebugDraw) { colliderDebugDraw_ = colliderDebugDraw; }

public:
	using json = nlohmann::json;

	void SaveFile(const std::vector<std::string>& stages);

	void FileOverWrite(const std::string& stage);

	void ChackFiles();

	void LoadFiles(const std::string& stage);

	void LoadFile(const std::string& groupName, const std::string& stage);

	bool LoadChackItem(const std::string& directoryPath, const std::string& itemName);

private:


	void from_json(const json& j, Vector3& v);

private:
	//ファイル保存関連
	int chackOnlyNumber = 0;

	const std::string kDirectoryPath = "Resources/Stages/";

	const std::string kDirectoryName = "Resources/Stages";

	const std::string kItemName_ = "Floor";

	std::string Name_ = "\0";

	char ItemName_[256]{};

	std::vector<std::string> fileName;
private:

	// モデル
	Model* model_ = nullptr;
	// マテリアル
	Material* material_ = nullptr;

	// 床リスト
	std::vector<Floor*> floors_;

	// デバッグ描画
	ColliderDebugDraw* colliderDebugDraw_ = nullptr;

};
