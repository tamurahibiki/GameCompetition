#include "stdafx.h"
#include "Kabe.h"

Kabe::Kabe()
{

}

Kabe::~Kabe()
{

}

bool Kabe::Start()
{
	//ÉÇÉfÉãÇì«Ç›çûÇﬁ
	m_modelRender.Init("Assets/modelData/kabe.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();

	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	return true;
}

void Kabe::Updata()
{
	m_modelRender.Update();
}

void Kabe::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}