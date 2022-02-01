#include "stdafx.h"
#include "Saku1.h"

Saku1::Saku1()
{

}

Saku1::~Saku1()
{

}

bool Saku1::Start()
{
	m_modelRender.Init("Assets/modelData/saku1.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();

	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	return true;
}

void Saku1::Updata()
{
	m_modelRender.Update();
}

void Saku1::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}