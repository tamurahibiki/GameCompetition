#include "stdafx.h"
#include "Yuka.h"

Yuka::Yuka()
{

}

Yuka::~Yuka()
{

}

bool Yuka::Start()
{
	//ÉÇÉfÉãÇì«Ç›çûÇﬁ
	m_modelRender.Init("Assets/modelData/yuka.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();

	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	return true;
}

void Yuka::Updata()
{
	m_modelRender.Update();
}

void Yuka::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}