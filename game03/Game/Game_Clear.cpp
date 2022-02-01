#include "stdafx.h"
#include "Game_Clear.h"

#include "Title.h"
#include "Fade.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Game_Clear::Game_Clear()
{

}

Game_Clear::~Game_Clear()
{
	//BGM���폜����B
	DeleteGO(m_bgm);
}

bool Game_Clear::Start()
{
	//�摜��ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/gameclear.dds", 1920, 1105);
	m_pressButton.Init("Assets/sprite/button.dds", 1293, 646);
	m_pressButton.SetPosition(Vector3(0.0f, -220.0f, 0.0f));

	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/gameclear.wav");
	g_soundEngine->ResistWaveFileBank(17, "Assets/sound/senntaku.wav");

	//BGM�B
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(3);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.3f);
	
	return true;
}

void Game_Clear::Update()
{

	if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Title>(0, "title");
		//���g���폜����B
		DeleteGO(this);

			//���ʉ����Đ�����B
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(17);
			se->Play(false);
			se->SetVolume(0.6f);
	}


	//���l��ω�������B
	if (m_isWaitFadeout)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 20.5f;
	}
	else
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	}

	m_pressButton.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));

	//�摜�̍X�V�B
	m_spriteRender.Update();
	m_pressButton.Update();
}

void Game_Clear::Render(RenderContext& rc)
{
	//�摜�̕`��B
	m_spriteRender.Draw(rc);
	m_pressButton.Draw(rc);
}