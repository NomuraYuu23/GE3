#pragma once
#include <random>

class Random
{

public: // メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static Random* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ランダムエンジンゲッター
	/// </summary>
	/// <returns></returns>
	std::mt19937 GetRandomEngine() { return randomEngine_; }

private:
	Random() = default;
	~Random() = default;
	Random(const Random&) = delete;
	const Random& operator=(const Random&) = delete;

private: // メンバ変数

	std::random_device seedGenerator_;
	std::mt19937 randomEngine_;

};

