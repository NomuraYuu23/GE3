#pragma once
#include"../Enemy/Enemy.h"
#include<list>
#include "../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画
#include"../../externals/nlohmann/json.hpp"
#include<fstream>
#include<iostream>



class EnemyManager{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(const std::vector<Model*>& models,
		const std::vector<Material*>& materials);

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
	void AddEnemy(TransformStructure enemy);


public: // アクセッサ

	/// <summary>
	/// 床リストゲッター
	/// </summary>
	/// <returns></returns>
	std::list<Enemy*> GetEnemys_() { return enemys_; }

	/// <summary>
	/// デバッグ描画セッター
	/// </summary>
	/// <param name="colliderDebugDraw"></param>
	void SetColliderDebugDraw(ColliderDebugDraw* colliderDebugDraw) { colliderDebugDraw_ = colliderDebugDraw; }

	void SetPlayer(Player* player) { player_ = player; }

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

	const std::string kItemName_ = "Enemy";

	std::string Name_ = "\0";

	char ItemName_[256]{};

	std::vector<std::string> fileName;

private:

	//モデルデータ配列
	std::vector<Model*> models_;
	//マテリアルデータ配列
	std::vector<Material*> materials_;

	// 床リスト
	std::list<Enemy*> enemys_;

	const Player* player_ = nullptr;

	// デバッグ描画
	ColliderDebugDraw* colliderDebugDraw_ = nullptr;
};

