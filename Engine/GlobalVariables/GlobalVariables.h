#pragma once
#include <iostream>
#include <variant>
#include <string>
#include <map>
#include "../Math/Vector3.h"

class GlobalVariables {

public:

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// �O���[�v�̍쐬
	/// </summary>
	/// <param name="groupName">�O���[�v��</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// �t�@�C���ɏ����o��
	/// </summary>
	/// <param name="groupName">�O���[�v</param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// �l�̃Z�b�g
	/// </summary>
	/// <param name="groupName">�O���[�v��</param>
	/// <param name="key">���ږ�</param>
	/// <param name="value">�l</param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// �l�̃Z�b�g
	/// </summary>
	/// <param name="groupName">�O���[�v��</param>
	/// <param name="key">���ږ�</param>
	/// <param name="value">�l</param>
	void SetValue(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// �l�̃Z�b�g
	/// </summary>
	/// <param name="groupName">�O���[�v��</param>
	/// <param name="key">���ږ�</param>
	/// <param name="value">�l</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �f�B���N�g���̑S�t�@�C���ǂݍ���
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// �t�@�C������ǂݍ���
	/// </summary>
	/// <param name="groupName">�O���[�v</param>
	void LoadFile(const std::string& groupName);

	// ���ڂ̒ǉ�(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	// ���ڂ̒ǉ�(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	// ���ڂ̒ǉ�(Vector3)
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

	// �l�̎擾(int)
	int32_t GetIntValue(const std::string& groupName, const std::string& key);
	// �l�̎擾(float)
	float GetFloatValue(const std::string& groupName, const std::string& key);
	// �l�̎擾(Vector3)
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

	//����
	//struct Item {
		// ���ڂ̒l
	//	std::variant<int32_t, float, Vector3> value;
	//};

	//�O���[�v
	//struct Group {
	//	std::map<std::string, Item> items;
	//};

	using Item = std::variant<int32_t, float, Vector3>;
	using Group = std::map<std::string, Item>;

	// �S�f�[�^
	std::map<std::string, Group> datas_;

	// �O���[�o���ϐ��̕ۑ���t�@�C���p�X
	const std::string kDirectoryPath = "Resources/GlobalVariables/";

};
