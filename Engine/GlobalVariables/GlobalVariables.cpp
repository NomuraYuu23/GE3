#include "GlobalVariables.h"
//#include <ImGuiManager.h>
//#include <json.hpp>
#include <fstream>

/// <summary>
/// �C���X�^���X�̎擾
/// </summary>
/// <returns></returns>
GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance;
	return &instance;
}

/// <summary>
/// �O���[�v�̍쐬
/// </summary>
/// <param name="groupName">�O���[�v��</param>
void GlobalVariables::CreateGroup(const std::string& groupName) {

	//�w�薼�̃I�u�W�F�N�g���Ȃ���Βǉ�����
	datas_[groupName];

}

/// <summary>
/// �t�@�C���ɏ����o��
/// </summary>
/// <param name="groupName">�O���[�v</param>
void GlobalVariables::SaveFile(const std::string& groupName) {

	//�O���[�v������
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	//���o�^�`�F�b�N
	assert(itGroup != datas_.end());

	//�O���[�v�o�^�m�F
	nlohmann::json root;

	root = nlohmann::json::object();

	// json�I�u�W�F�N�g�o�^
	root[groupName] = nlohmann::json::object();

	//�e���ڂɂ���
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
		itItem != itGroup->second.end(); ++itItem) {

		//���ږ����擾
		const std::string& itemName = itItem->first;
		//���ڂ̎Q�Ƃ��擾
		Item& item = itItem->second;

		// int32_t�^�̒l��ێ����Ă����
		if (std::holds_alternative<int32_t>(item)) {
			// int32_t�^�̒l��o�^
			root[groupName][itemName] = std::get<int32_t>(item);
		}

		// float�^�̒l��ێ����Ă����
		else if (std::holds_alternative<float>(item)) {
			// float�^�̒l��o�^
			root[groupName][itemName] = std::get<float>(item);
		}

		// Vector3�^�̒l��ێ����Ă����
		else if (std::holds_alternative<Vector3>(item)) {
			// float�^�̒l��o�^
			Vector3 value = std::get<Vector3>(item);
			root[groupName][itemName] = nlohmann::json::array({ value.x, value.y, value.z });
		}

		//�f�B���N�g�����Ȃ���΍쐬����
		std::filesystem::path dir(kDirectoryPath);
		if (!std::filesystem::exists(dir)) {
			std::filesystem::create_directory(dir);
		}

		// ��������JSON�t�@�C���̃t���p�X����������
		std::string filePath = kDirectoryPath + groupName + ".json";
		// �������ޗp�t�@�C���X�g���[��
		std::ofstream ofs;
		// �t�@�C�����������ݗp�ɊJ��
		ofs.open(filePath);
		// �t�@�C���I�[�v�����s
		if (ofs.fail()) {
			std::string message = "Failed open data file for write.";
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			assert(0);
			return;
		}

		// �t�@�C����json���������������(�C���f���g��4)
		ofs << std::setw(4) << root << std::endl;
		// �t�@�C�������
		ofs.close();

	}

}

/// <summary>
/// �l�̃Z�b�g
/// </summary>
/// <param name="groupName">�O���[�v��</param>
/// <param name="key">���ږ�</param>
/// <param name="value">�l</param>
void GlobalVariables::SetValue(
	const std::string& groupName, const std::string& key, int32_t value) {

	// �O���[�v�̎Q�Ƃ��擾
	Group& group = datas_[groupName];
	// �V�������ڂ̃f�[�^��ݒ�
	Item newItem{};
	newItem = value;
	// �ݒ肵�����ڂ�std::map�ɒǉ�
	group[key] = newItem;

}

/// <summary>
/// �l�̃Z�b�g
/// </summary>
/// <param name="groupName">�O���[�v��</param>
/// <param name="key">���ږ�</param>
/// <param name="value">�l</param>
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {

	// �O���[�v�̎Q�Ƃ��擾
	Group& group = datas_[groupName];
	// �V�������ڂ̃f�[�^��ݒ�
	Item newItem{};
	newItem = value;
	// �ݒ肵�����ڂ�std::map�ɒǉ�
	group[key] = newItem;

}

/// <summary>
/// �l�̃Z�b�g
/// </summary>
/// <param name="groupName">�O���[�v��</param>
/// <param name="key">���ږ�</param>
/// <param name="value">�l</param>
void GlobalVariables::SetValue(
	const std::string& groupName, const std::string& key, const Vector3& value) {

	// �O���[�v�̎Q�Ƃ��擾
	Group& group = datas_[groupName];
	// �V�������ڂ̃f�[�^��ݒ�
	Item newItem{};
	newItem = value;
	// �ݒ肵�����ڂ�std::map�ɒǉ�
	group[key] = newItem;

}

/// <summary>
/// ���t���[������
/// </summary>
void GlobalVariables::Update() {

	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())
		return;

	//�e�O���[�v�ɂ���
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
		++itGroup) {
		// �O���[�v�����擾
		const std::string& groupName = itGroup->first;
		// �O���[�v�̎Q�Ƃ��擾
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		//�e���ڂɂ���
		for (std::map<std::string, Item>::iterator itItem = group.begin();
			itItem != group.end(); ++itItem) {

			//���ږ����擾
			const std::string& itemName = itItem->first;
			//���ڂ̎Q�Ƃ��擾
			Item& item = itItem->second;

			// int32_t�^�̒l��ێ����Ă����
			if (std::holds_alternative<int32_t>(item)) {
				int32_t* ptr = std::get_if<int32_t>(&item);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}

			// float�^�̒l��ێ����Ă����
			if (std::holds_alternative<float>(item)) {
				float* ptr = std::get_if<float>(&item);
				ImGui::SliderFloat(itemName.c_str(), ptr, -100.0f, 100.0f);
			}

			// Vector3�^�̒l��ێ����Ă����
			if (std::holds_alternative<Vector3>(item)) {
				Vector3* ptr = std::get_if<Vector3>(&item);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}

		}

		//���s
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}


		ImGui::EndMenu();

	}

	ImGui::EndMenuBar();
	ImGui::End();

}

/// <summary>
/// �f�B���N�g���̑S�t�@�C���ǂݍ���
/// </summary>
void GlobalVariables::LoadFiles() {

	// �ۑ���f�B���N�g���̃p�X�����[�J���ϐ��Ő錾����
	std::filesystem::path dir(kDirectoryPath);
	// �f�B���N�g�����Ȃ���΃X�L�b�v����
	if (!std::filesystem::exists(dir)) {
		return;
	}

	//�e�t�@�C���̏���
	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {

		// �t�@�C���p�X���擾
		const std::filesystem::path& filePath = entry.path();

		// �t�@�C���g���q���擾
		std::string extension = filePath.extension().string();
		// .json�t�@�C���ȊO�̓X�L�b�v
		if (extension.compare(".json") != 0) {
			continue;
		}

		// �t�@�C���ǂݍ���
		LoadFile(filePath.stem().string());

	}


}

/// <summary>
/// �t�@�C������ǂݍ���
/// </summary>
/// <param name="groupName">�O���[�v</param>
void GlobalVariables::LoadFile(const std::string& groupName) {

	// �ǂݍ���JSON�t�@�C���̃t���p�X����������
	std::string filePath = kDirectoryPath + groupName + ".json";
	// �ǂݍ��ݗp�t�@�C���X�g���[��
	std::ifstream ifs;
	// �t�@�C����ǂݍ��ݗp�ɕ���
	ifs.open(filePath);

	// �t�@�C���I�[�v�����s?
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	nlohmann::json root;

	// json�����񂩂�json�̃f�[�^�\���ɓW�J
	ifs >> root;
	// �t�@�C�������
	ifs.close();

	// �O���[�v������
	nlohmann::json::iterator itGroup = root.find(groupName);

	// ���o�^�`�F�b�N
	assert(itGroup != root.end());

	// �e�A�C�e���ɂ���
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {

		// �A�C�e�������擾
		const std::string& itemName = itItem.key();

		// int32_t�^�̒l��ێ����Ă����
		if (itItem->is_number_integer()) {
			// int�^�̒l��o�^
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}

		// float�^�̒l��ێ����Ă����
		else if (itItem->is_number_float()) {
			// int�^�̒l��o�^
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}
		// �v�f��3�̔z��ł����
		else if (itItem->is_array() && itItem->size() == 3) {
			// float�^��json�z��o�^
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);
		}

	}


}


// ���ڂ̒ǉ�(int)
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {

	// ���ڂ����o�^�Ȃ�
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}

}

// ���ڂ̒ǉ�(float)
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {

	// ���ڂ����o�^�Ȃ�
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}
}
// ���ڂ̒ǉ�(Vector3)
void GlobalVariables::AddItem(
	const std::string& groupName, const std::string& key, const Vector3& value) {

	// ���ڂ����o�^�Ȃ�
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}

}

// �l�̎擾(int)
int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) {

	assert(datas_.find(groupName) != datas_.end());
	//�O���[�v�̎Q�Ƃ��擾
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return std::get<0>(group[key]);
}

// �l�̎擾(float)
float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) {

	assert(datas_.find(groupName) != datas_.end());
	// �O���[�v�̎Q�Ƃ��擾
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return std::get<1>(group[key]);

}

// �l�̎擾(Vector3)
Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) {

	assert(datas_.find(groupName) != datas_.end());
	// �O���[�v�̎Q�Ƃ��擾
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return std::get<2>(group[key]);

}
