#include "transform.h"

NS_YY_BEGIN
Transform::Transform()
{
    m_pos.x = m_pos.y = m_pos.z = 0.0f;
    m_scale.x = m_scale.y = m_scale.z = 1.0f;
    m_angle.x = m_angle.y = m_angle.z = 0.0f;
}

Transform::~Transform(){}

void Transform::AddPosX(float x)
{
    m_pos.x += x;
}

void Transform::AddPosY(float y)
{
    m_pos.y += y;
}

void Transform::AddPosZ(float z)
{
    m_pos.z += z;
}

void Transform::GetRotateMatrix(YY::Mat4f* rotateM) const
{
    rotateM->RotateX(m_angle.x);
    rotateM->RotateY(m_angle.y);
    rotateM->RotateZ(m_angle.z);
    return;
}

void Transform::GetTranslateMatrix(YY::Mat4f* pMatrix) const
{
    pMatrix->Translate(m_pos.x, m_pos.y, m_pos.z);
}

void Transform::GetScaleMatrix(YY::Mat4f* pMatrix) const
{
    pMatrix->Scale(m_scale.x, m_scale.y, m_scale.z);
}


void Transform::GetTMMatrix(YY::Mat4f* pMatrix) const
{
    pMatrix->Ident();
    YY::Mat4f translate,scale,rotate;
    GetRotateMatrix(&rotate);
    GetScaleMatrix(&scale);
    GetTranslateMatrix(&translate);

    // 顺序is decided by matrix mutiply function：顶点先缩放，在旋转，再平移。
    *pMatrix = translate*rotate*scale;
}


void Transform::AddAngleX(float x)
{
    m_angle.x += x;
}

void Transform::AddAngleY(float y)
{
    m_angle.y += y;
}

void Transform::AddAngleZ(float z)
{
    m_angle.z += z;
}


void Transform::SetParent(ITransform* pTransform)
{
	m_parent = pTransform->GetID();
}

ITransform* Transform::FindParent()
{
	BaseObject* pObject = GetMgr()->Find(m_parent);
	if (NULL == pObject)
		return NULL;

	if (!pObject->IsInstanceOf("Transform"))
		return NULL;

	return (ITransform*)pObject;
}

VarList Transform::GetChildren()
{
	VarList args;
	for (int i = 0; i < m_children.size(); i++)
	{
		args << m_children[i];
	}
	return args;
}

void Transform::AddChild(ITransform* pTransform)
{
	std::vector<YY_OBJECTID>::iterator itor = std::find(m_children.begin(), m_children.end(), pTransform->GetID());
	if (itor != m_children.end())
		return;

	m_children.push_back(pTransform->GetID());
}

void Transform::RemoveChild(ITransform* pTransform)
{
	std::vector<YY_OBJECTID>::iterator itor = std::find(m_children.begin(), m_children.end(), pTransform->GetID());
	if (itor == m_children.end())
		return;

	m_children.erase(itor);
}

bool Transform::IsChildExist(ITransform* pTransform)
{
	std::vector<YY_OBJECTID>::iterator itor = std::find(m_children.begin(), m_children.end(), pTransform->GetID());
	if (itor == m_children.end())
		return false;

	return true;

}

NS_YY_END