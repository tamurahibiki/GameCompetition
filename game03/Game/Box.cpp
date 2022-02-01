#include "stdafx.h"
#include "Box.h"


Box::Box()
{

}

Box::~Box()
{

}

bool Box::Start()
{
	//çÇÇ≥Ç2000.0fè„Ç∞ÇÈ
	m_position.y += 2000.0f;

	//ÉÇÉfÉãÇì«Ç›çûÇﬁ
	m_modelRender.Init("Assets/modelData/box.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();


	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	return true;
}

void Box::Updata()
{
	m_modelRender.Update();
}

void Box::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}