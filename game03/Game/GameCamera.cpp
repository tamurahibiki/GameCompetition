#include "stdafx.h"
#include "GameCamera.h"
#include"Player.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos.Set(0.0f, 100.0f, -350.0f);

	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");
	/*m_springCamera.Init(
		*g_camera3D,
		1000.0f,
		true,
		5.0f
	);*/

	return true;
}

void GameCamera::UpdatePositionAndTarget()
{

	//�J�������X�V�B
	//�����_���v�Z����B
	Vector3 target = m_player->m_position;
	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target.y += 200.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	qRot.Apply(m_toCameraPos);
	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.5f * y);
	qRot.Apply(m_toCameraPos);
	//�J�����̉�]�̏�����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		//�J����������������B
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		//�J�����������������B
		m_toCameraPos = toCameraPosOld;
	}

	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;

	//�J�����̏����B
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

}

void GameCamera::Update()
{
	//���_�ƒ����_���X�V����B
	UpdatePositionAndTarget();

	//�J�����̍X�V�B
	m_springCamera.Update();
}