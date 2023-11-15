#pragma once
#include"../BreakBox/BreakBox.h"
#include<list>
#include "../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画
#include"../../externals/nlohmann/json.hpp"
#include<fstream>
#include<iostream>


class BreakBoxManager{
public:
	using json = nlohmann::json;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BreakBoxManager();

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
	/// imgui描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void DrawImgui();

	/// <summary>
	/// 床追加
	/// </summary>
	void AddBox(TransformStructure box, bool isMoving, bool isVertical);

public: // アクセッサ

	/// <summary>
	/// 床リストゲッター
	/// </summary>
	/// <returns></returns>
	std::list<BreakBox*> GetBoxes_() { return breakBoxes_; }

	/// <summary>
	/// デバッグ描画セッター
	/// </summary>
	/// <param name="colliderDebugDraw"></param>
	void SetColliderDebugDraw(ColliderDebugDraw* colliderDebugDraw) { colliderDebugDraw_ = colliderDebugDraw; }

public:
	void SaveFile();

	void ChackFiles();

	void LoadFiles();

	void LoadFile(const std::string& groupName);

	bool LoadChackItem(const std::string& directoryPath, const std::string& itemName);

private:


	void from_json(const json& j, Vector3& v);

private:
	//ファイル保存関連
	int chackOnlyNumber = 0;

	const std::string kDirectoryPath = "Resources/Stages/";

	const std::string kDirectoryName = "Resources/Stages";

	const std::string kItemName_ = "BreakBox";

	std::string Name_ = "\0";

	char ItemName_[256]{};

	std::vector<std::string> fileName;

private:

	// モデル
	Model* model_ = nullptr;
	// マテリアル
	Material* material_ = nullptr;

	// 床リスト
	std::list<BreakBox*> breakBoxes_;

	// デバッグ描画
	ColliderDebugDraw* colliderDebugDraw_ = nullptr;
};

