#pragma once
#include "../BaseCharacter/BaseCharacter.h"
#include "../../Engine/Collider/Sphere/Sphere.h"
#include"../../externals/nlohmann/json.hpp"
#include<fstream>
#include<iostream>


class Goal : public BaseCharacter
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* models, Material* materials);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;

	void DrawImgui();

public: // アクセッサ

	/// <summary>
	/// 半径ゲッター
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	Sphere& GetCollider() { return collider_; }

private: // メンバ変数

	// 位置
	const Vector3 kPosition = { 0.0f, 1.0f, 60.0f };

	// 半径
	const float kRadius = 2.0f;
	float radius_;

	// コライダー
	Sphere collider_;

	// モデル
	Model* model_ = nullptr;
	//マテリアル
	Material* material_ = nullptr;
	
	//ファイル関連
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

	const std::string kItemName_ = "Goal";

	std::string Name_ = "\0";

	char ItemName_[256]{};

	std::vector<std::string> fileName;
};

