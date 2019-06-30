#include "core/inc/yy_entity.h"
#include <algorithm>

NS_YY_BEGIN

bool Entity::parseFromObject(const rapidjson::Value* pObject)
{
	if (!BaseObject::parseFromObject(pObject))
		return false;


	if (!parseFromComponents(pObject))
		return false;

	if (!parseFromChildren(pObject))
		return false;

	return true;
}


bool Entity::parseFromComponents(const rapidjson::Value* pObject)
{
	return true;
}

bool Entity::parseFromChildren(const rapidjson::Value* pObject)
{
	Value::ConstMemberIterator childrenItr = pObject->FindMember("children");
	if (childrenItr == pObject->MemberEnd())
		return true;


	bool bRet = true;
	IReflectionMgr* pReflectionMgr = GetReflectionMgr();
	const Value* children = &childrenItr->value;
	for (Value::ConstMemberIterator itr = children->MemberBegin(); itr != children->MemberEnd(); ++itr)
	{
		std::string key = itr->name.GetString();
		const rapidjson::Value* pChildValue = &itr->value;

		// new entity
		BaseObject* pChildObject = GetMgr()->Create(key);
		if (!pChildObject)
		{
			bRet = false;
			continue;
		}

		if (!pChildObject->IsInstanceOf("Entity"))
		{
			GetMgr()->Destroy(pChildObject->GetID());
			bRet = false;
			continue;
		}

		Entity* pChildEntity = (Entity*)pChildObject;
		if (!pChildEntity->parseFromObject(pChildValue))
		{
			bRet = false;
			continue;
		}

		AddChild(pChildEntity);
	}

	return bRet;
}



bool Entity::serializeObjectTo(rapidjson::Document* doc, rapidjson::Value* pObject)
{
	if (!BaseObject::serializeObjectTo(doc, pObject))
		return false;

	if (!serializeComponents(doc, pObject))
		return false;

	if (!serializeChildren(doc, pObject))
		return false;

	return true;
}


bool Entity::serializeComponents(rapidjson::Document* doc, rapidjson::Value* pObject)
{
	return true;
}

bool Entity::serializeChildren(rapidjson::Document* doc, rapidjson::Value* pObject)
{
	int nChildCnt = GetChildCount();
	if (nChildCnt <= 0)
		return true;

	bool bResult = true;
	rapidjson::Value children;
	children.SetObject();

	for (int i=0; i<m_children.size(); i++)
	{
		YY_OBJECTID id = m_children[i];
		BaseObject* pChildObject = GetMgr()->Find(id);
		if (!pChildObject)
		{
			bResult = false;
			continue;
		}


		if (!pChildObject->IsInstanceOf("Entity"))
		{
			bResult = false;
			continue;
		}

		Entity* pChildEntity = (Entity*)pChildObject;

		rapidjson::Value subValue;
		subValue.SetObject();
		if(!pChildEntity->serializeObjectTo(doc, &subValue))
		{
			bResult = false;
			continue;
		}

		rapidjson::Value subKey(pChildEntity->GetMetaClass()->name.c_str(), doc->GetAllocator());
		children.AddMember(subKey, subValue, doc->GetAllocator());
	}

	pObject->AddMember("children", children, doc->GetAllocator());
	return bResult;
}



void Entity::OnCreate()
{
	BaseObject::OnCreate();

// 	YY_OBJECTID parent = YY_INVALID_OBJECTID;
// 	YY_OBJECTID prev = YY_INVALID_OBJECTID;
// 	YY_OBJECTID next = YY_INVALID_OBJECTID;
// 	auto itor = args.find(PROP_ENTITY_PARENT);
// 	if (itor != args.end())
// 		parent = itor->second.GetInt64();
// 
// 	itor = args.find(PROP_ENTITY_PREV);
// 	if (itor != args.end())
// 		prev = itor->second.GetInt64();
// 
// 	itor = args.find(PROP_ENTITY_NEXT);
// 	if (itor != args.end())
// 		next = itor->second.GetInt64();
// 
// 	BaseObject* pParent = GetMgr()->Find(parent);
// 	BaseObject* pPrev = GetMgr()->Find(prev);
// 	BaseObject* pNext = GetMgr()->Find(next);
// 	if (nullptr != pParent)
// 	{
// 		if (!pParent->IsInstanceOf("Entity"))
// 			return;
// 
// 		Entity* pParentEntity = (Entity*)pParent;
// 		if (pPrev)
// 		{
// 			if (!pPrev->IsInstanceOf("Entity"))
// 				return;
// 
// 			pParentEntity->InsertAfter(this, (Entity*)pPrev);
// 		}
// 		else if (pNext)
// 		{
// 			if (!pNext->IsInstanceOf("Entity"))
// 				return;
// 
// 			pParentEntity->InsertBefore(this, (Entity*)pNext);
// 		}
// 		else
// 				pParentEntity->AddChild(this);
// 	}
}

void Entity::OnDestroyed()
{
	ClearChildren();
	GetMgr()->Destroy(GetID());
}


Entity* Entity::Duplicate()
{
	//std::string str = serializeTo();
	//return Entity::parseFrom(GetMgr(), str);
	return nullptr;
}


void Entity::InsertAfter(Entity* pEntity, Entity* after)
{
	auto itor = std::find(m_children.begin(), m_children.end(), pEntity->GetID());
	if (itor == m_children.end())
		return;

	m_children.insert(itor, after->GetID());
}

void Entity::InsertBefore(Entity* pEntity, Entity* before)
{
	auto itor = std::find(m_children.begin(), m_children.end(), pEntity->GetID());
	if (itor == m_children.end())
		return;

	//if(itor==m_children.begin())
		//m_children.insert()

	//m_children.insert(itor, before->GetID());
}

void Entity::SetParent(Entity* pEntity)
{
	if(pEntity)
		m_parent = pEntity->GetID();
}

Entity* Entity::FindParent()
{
	BaseObject* pObject = GetMgr()->Find(m_parent);
	if (NULL == pObject)
		return NULL;

	if (!pObject->IsInstanceOf("Entity"))
		return NULL;

	return (Entity*)pObject;
}


void Entity::ClearChildren()
{
	InvokeEvent("ClearChildren");

	int nCnt = GetChildCount();
	for (int i = 0; i < nCnt; i++)
	{
		YY_OBJECTID id = GetChildByIndex(i);
		BaseObject* pObject = GetMgr()->Find(id);
		if (nullptr == pObject)
			continue;

		if (!pObject->IsInstanceOf("Entity"))
		{
			GetMgr()->Destroy(pObject->GetID());
			continue;
		}

		Entity* pChildEntity = (Entity*)pObject;
		pChildEntity->ClearChildren();
		GetMgr()->Destroy(pObject->GetID());
	}

	m_children.clear();
}

VarList Entity::GetChildren()
{
	VarList args;
	for (int i = 0; i < m_children.size(); i++)
	{
		args << m_children[i];
	}
	return args;
}


int Entity::GetChildCount()
{
	return m_children.size();
}

YY_OBJECTID Entity::GetChildByIndex(int index)
{
	if (index < 0 || index >= GetChildCount())
		return YY_INVALID_OBJECTID;

	return m_children[index];
}


Entity* Entity::FindChild(const std::string& name)
{
	int nCnt = GetChildCount();
	for (int i = 0; i < nCnt; i++)
	{
		YY_OBJECTID id = GetChildByIndex(i);
		BaseObject* pObject = GetMgr()->Find(id);
		if (NULL == pObject)
			continue;

		if (!pObject->IsInstanceOf("Entity"))
			continue;

		Entity* pEntity = (Entity*)pObject;
		if (pEntity->GetName() == name)
			return pEntity;
	}

	return nullptr;
}

void Entity::AddChild(Entity* pEntity)
{
	auto itor = std::find(m_children.begin(), m_children.end(), pEntity->GetID());
	if (itor != m_children.end())
		return;

	m_children.push_back(pEntity->GetID());
	pEntity->SetParent(this);
	int nIndex = m_children.size() - 1;

	InvokeEvent("AddChild", YY::VarList()<<pEntity->GetID()<<nIndex);
}

void Entity::RemoveChild(Entity* pEntity)
{
	std::vector<YY_OBJECTID>::iterator itor = std::find(m_children.begin(), m_children.end(), pEntity->GetID());
	if (itor == m_children.end())
		return;

	int nIndex = itor - m_children.begin();
	InvokeEvent("RemoveChild", YY::VarList() << pEntity->GetID()<<nIndex);

	pEntity->SetParent(nullptr);
	m_children.erase(itor);
}

bool Entity::IsChildExist(Entity* pEntity)
{
	std::vector<YY_OBJECTID>::iterator itor = std::find(m_children.begin(), m_children.end(), pEntity->GetID());
	if (itor == m_children.end())
		return false;

	return true;

}
// 
// 
// void Entity::OnAddBatch(IBatchGroup* pBatchGroup)
// {
// 	pBatchGroup->AddNormalBatch(this);
// 
// 	int nChildNum = GetChildCount();
// 	for (int i = 0; i < nChildNum; i++)
// 	{
// 		YY_OBJECTID id = GetChildByIndex(i);
// 		BaseObject* pObject = GetMgr()->Find(id);
// 		if (nullptr == pObject)
// 			continue;
// 
// 		if (!pObject->IsInstanceOf("Entity"))
// 			continue;
// 
// 		Entity* pEntity = (Entity*)pObject;
// 		pEntity->OnAddBatch(pBatchGroup);
// 	}
// }
// 
// 
// IRender* Entity::GetRender()
// {
// 	if (nullptr == m_pRender)
// 		m_pRender = IRender::Instance(GetMgr());
// 
// 	return m_pRender;
// }


void Entity::InvokeEvent(const std::string& event, const YY::VarList& args)
{
	// entity handle this event
	OnEvent(event, args);

	// component handle this event
	std::map<std::string, Component*>::iterator itor = m_components.begin();
	for (; itor != m_components.end(); itor++)
	{
		Component* pComponent = itor->second;
		pComponent->OnEvent(event, args);
	}
}
// 
// void Entity::OnRenderBefore(RenderContext* pCxt)
// {
// 	auto itor = m_components.begin();
// 	for (; itor != m_components.end(); itor++)
// 	{
// 		Component* pComponent = itor->second;
// 		pComponent->OnRenderBefore(pCxt);
// 	}
// }
// 
// void Entity::OnRenderAfter(RenderContext* pCxt)
// {
// 	auto itor = m_components.begin();
// 	for (; itor != m_components.end(); itor++)
// 	{
// 		Component* pComponent = itor->second;
// 		pComponent->OnRenderAfter(pCxt);
// 	}
// 
// 	m_bFirstRender = false;
// }

std::vector<Component*> Entity::GetAllComponents()
{
	std::vector<Component*> components;
	auto itor = m_components.begin();
	for (; itor != m_components.end(); itor++)
	{
		Component* pComponent = itor->second;
		components.push_back(pComponent);
	}
	return components;
}

Component* Entity::FindComponent(const std::string& strClassName)
{
	auto itor = m_components.find(strClassName);
	if (itor == m_components.end())
		return NULL;

	return itor->second;
}


Component* Entity::AddComponent(const std::string& strClassName)
{
	// same component already exist.
	if (NULL != FindComponent(strClassName))
		return NULL;

	BaseObject* pObject = GetMgr()->Create(strClassName);
	if (!pObject->IsInstanceOf("Component"))
		return NULL;

	Component* pComponent = (Component*)pObject;
	m_components[strClassName] = pComponent;
	pComponent->SetOwner(this);
	InvokeEvent("addComponent");
	return pComponent;
}

void Entity::RemoveComponent(YY_OBJECTID id)
{
	BaseObject* pComponent = GetMgr()->Find(id);
	if (NULL == pComponent)
		return;

	std::string strClassName = pComponent->GetMetaClass()->name;
	auto itor = m_components.find(strClassName);
	if (itor != m_components.end())
	{
		Component* pComponent = itor->second;
		InvokeEvent("removeComponent");
		m_components.erase(itor);
	}
}
NS_YY_END


