#include "ClearScene.h"

void ClearScene::Initialize()
{
}

void ClearScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = kGame;
	}

}

void ClearScene::Draw()
{

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList());

	//3D�I�u�W�F�N�g�͂���

	Model::PostDraw();

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//�w�i
	//�O�i�X�v���C�g�`��

	// �O�i�X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion

}
