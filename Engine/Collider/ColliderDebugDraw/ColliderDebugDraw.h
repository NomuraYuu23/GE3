#pragma once
#include "../Sphere/Sphere.h"
#include "../Plane/Plane.h"
#include "../Triangle/Triangle.h"
#include "../AABB/AABB.h"
#include "../OBB/OBB.h"

#include <list>
#include <vector>
#include "../../3D/Model.h"
#include "../../3D/Material.h"
#include "../../3D/ViewProjection.h"

/// <summary>
/// �R���C�_�[�̕`��֐��f�o�b�O�p
/// </summary>
class ColliderDebugDraw
{

public: // �T�u�N���X

	enum class ModelNo {
		kSphere,
		kPlane,
		kTriangle,
		kAABB,
		kOBB,
		kEnd // �g��Ȃ�
	};

public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="models"></param>
	/// <param name="material"></param>
	void Initialize(const std::vector<Model*> models, const Material* material);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection* viewProjection);

private: // �����o�ϐ�

	// ��
	std::list<Sphere*> spheres_;
	// ����
	std::list<Plane*> planes;
	// �O�p�`
	std::list<Triangle*> triangle;
	// AABB
	std::list<AABB*> AABBs_;
	// OBB
	std::list<OBB*> OBBs_;

	// ���f��
	std::vector<Model*> models_;

	// �}�e���A��
	Material* material_;


};

