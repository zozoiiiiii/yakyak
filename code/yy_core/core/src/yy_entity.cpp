#include "core/inc/yy_entity.h"
#include <algorithm>

NS_YY_BEGIN

Entity* Entity::ParseFromFile(IObjectMgr* pObjMgr, const std::string& file, YY_OBJECTID parent, YY_OBJECTID prev, YY_OBJECTID next)
{
	const std::string& xmlFile = file;
	FILE* pFile = fopen(xmlFile.c_str(), "r");
	if (nullptr == pFile)
		return nullptr;

	fseek(pFile, 0, SEEK_END);
	int nSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	YY::AutoMem<char, 512> buf(nSize);
	fread(buf.GetBuf(), nSize, 1, pFile);
	fclose(pFile);

	return parseFrom(pObjMgr, buf.GetBuf(), parent, prev, next);
}

Entity* Entity::parseFrom(IObjectMgr* pObjMgr, const std::string& data, YY_OBJECTID parent, YY_OBJECTID prev, YY_OBJECTID next)
{
	YY::AutoMem<char, 512> strBuf(data.length());
	strcpy(strBuf.GetBuf(), data.c_str());

	xml_document doc;
	try
	{
		doc.parse<0>(strBuf.GetBuf());
	}
	catch (std::exception & e)
	{
		return nullptr;
	}

	// only one root node.
	xml_node * node = doc.first_node();
	if (node->next_sibling() != nullptr)
		return nullptr;

	BaseObject* pObject = parseFromNode(pObjMgr, node);
	if (pObject->IsInstanceOf("Entity"))
		return (Entity*)pObject;

	pObjMgr->Destroy(pObject->GetID());
	return nullptr;


// 
// 	// parse from the string
// 	xml_attribute* pAttribute = node->first_attribute("class");
// 	if (nullptr == pAttribute)
// 		return nullptr;
// 
// 	std::string className = pAttribute->value();
// 	BaseObject* pObject = pObjMgr->BeginCreate(className);
// 	if (!pObject)
// 		return nullptr;
// 
// 	if (!pObject->IsInstanceOf("Entity"))
// 	{
// 		pObjMgr->Destroy(pObject->GetID());
// 		return nullptr;
// 	}
// 
// 	Entity* pEntity = (Entity*)pObject;
// 	bool bLoad = pEntity->parseFromNode(node);
// 	if (!bLoad)
// 	{
// 		// log error.
// 	}
// 
// 	//pObjMgr->EndCreate(pObject, args);
// 
// 	return pEntity;
}


bool Entity::parseProperties(const xml_node* pNode, BaseObject* pObject)
{
	bool result = true;

	MetaClass* pMetaClass = pObject->GetMetaClass();
	xml_attribute* pAttribute = pNode->first_attribute();
	while (pAttribute)
	{		
		std::string key = pAttribute->name();
		std::string value = pAttribute->value();

		std::vector<std::string> splits;
		YY::Split(splits, key, ".");
		if (splits.size() >= 2)
		{
			if (splits.size() != 2)
			{
				result = false;
				pAttribute = pAttribute->next_attribute();
				continue;
			}

			std::string className = splits[0];
			std::string fieldName = splits[1];
			void* pInstance = nullptr;
			pObject->GetFieldVal(className.c_str(), pInstance);
			MetaField* pMetaField = GetReflectionMgr()->FindMetaFieldFromAll(className, fieldName);
			if (nullptr == pMetaField)
			{
				result = false;
				pAttribute = pAttribute->next_attribute();
				continue;
			}

			YY::Var var;
			var.SetType(pMetaField->var_type);
			var.ParseFrom(value);
			GetReflectionMgr()->SetBaseFieldVal(pInstance, pMetaField, var);
			
		}
		else
		{
			// skip this property
			if (key == "class")
			{
				pAttribute = pAttribute->next_attribute();
				continue;
			}

			MetaField* pMetaField = GetReflectionMgr()->FindMetaFieldFromAll(pMetaClass->name, key);
			if (nullptr == pMetaField)
			{
				result = false;
				pAttribute = pAttribute->next_attribute();
				continue;
			}

			YY::Var var;
			var.SetType(pMetaField->var_type);
			var.ParseFrom(value);
			GetMgr()->GetReflectionMgr()->SetBaseFieldVal(pObject, pMetaField, var);
		}

		pAttribute = pAttribute->next_attribute();
	}
	return result;
}

bool Entity::parseChildren(const xml_node* pNode)
{
	if (nullptr == pNode)
		return false;

	xml_node* pChildrenNode = pNode->first_node("children");
	if (nullptr == pChildrenNode)
		return true;

	bool bResult = true;
	xml_node* pChildNode = pChildrenNode->first_node();
	while (pChildNode)
	{
		Entity* pChildEntity = parseFromNode(GetMgr(), pChildNode);
		if(!pChildEntity)
		{
			bResult = false;
			pChildNode = pChildNode->next_sibling();
			continue;
		}

		AddChild(pChildEntity);
		pChildNode = pChildNode->next_sibling();
	}

	return bResult;
}

bool Entity::parseComponents(const xml_node* pNode)
{
	if (nullptr == pNode)
		return false;

	xml_node* pComponentsNode = pNode->first_node("components");
	if (nullptr == pComponentsNode)
		return true;

	bool bResult = true;
	xml_node* pComponentNode = pComponentsNode->first_node();
	while (pComponentNode)
	{
		xml_attribute* pAttribute = pComponentNode->first_attribute("class");
		if (nullptr == pAttribute)
			return false;

		std::string className = pAttribute->value();
		YY::Component* pComponent = AddComponent(className);
		if (!pComponent)
		{
			bResult = false;
			pComponentNode = pComponentNode->next_sibling();
			continue;
		}

		if (!parseProperties(pComponentNode, pComponent))
		{
			bResult = false;
			pComponentNode = pComponentNode->next_sibling();
			continue;
		}

		pComponentNode = pComponentNode->next_sibling();
	}

	return bResult;
}

Entity* Entity::parseFromNode(IObjectMgr* pObjMgr, const xml_node* pNode)
{
	if (nullptr == pNode)
		return nullptr;

	xml_attribute* pAttribute = pNode->first_attribute("class");
	if (nullptr == pAttribute)
	 	return nullptr;
	 
	std::string className = pAttribute->value();
	BaseObject* pObject = pObjMgr->Create(className);
	if (!pObject)
	 	return nullptr;

	if (!pObject->IsInstanceOf("Entity"))
	{
		pObjMgr->Destroy(pObject->GetID());
		return nullptr;
	}

	Entity* pEntity = (Entity*)pObject;
	bool bResult = true;
	if (!pEntity->parseProperties(pNode, pEntity))
		bResult = false;

	if (!pEntity->parseChildren(pNode))
		bResult = false;

	if (!pEntity->parseComponents(pNode))
		bResult = false;

	return pEntity;
}




std::string Entity::SerializeTo()
{
	throw_assert(GetMetaClass() && GetMgr(), "null check.");

	xml_document doc;
	xml_node * decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);


	// object node
	xml_node* pRootNode = doc.allocate_node(rapidxml::node_element, doc.allocate_string("node"));
	serializeToNode(&doc, pRootNode);
	doc.append_node(pRootNode);

	std::string xmlText;
	rapidxml::print(std::back_inserter(xmlText), doc, 0);
	return xmlText;
}

void Entity::SerializeToFile(const std::string& file)
{
	FILE* pFile = fopen(file.c_str(), "w");
	throw_assert(NULL != pFile, "create file:" << file << "errno:%d" << errno);
	std::string str = SerializeTo();
	fwrite(str.c_str(), str.length(), 1, pFile);
	fclose(pFile);
}





bool Entity::serializeComponents(xml_document* pDoc, xml_node* pObjectNode)
{
	if (m_components.empty())
		return true;

	bool bResult = true;
	xml_node* pComponentsNode = pDoc->allocate_node(rapidxml::node_element, pDoc->allocate_string("components"));
	pObjectNode->append_node(pComponentsNode);
	auto itor = m_components.begin();
	for (; itor != m_components.end(); itor++)
	{
		xml_node* pTmpNode = pDoc->allocate_node(rapidxml::node_element, pDoc->allocate_string("node"));
		Component* pComponent = itor->second;
		if (!serializeProperties(pComponent, pDoc, pTmpNode))
		{
			bResult = false;
			continue;
		}

		pComponentsNode->append_node(pTmpNode);
	}

	return bResult;
}


bool Entity::serializeChildren(xml_document* pDoc, xml_node* pObjectNode)
{
	int nChildCnt = GetChildCount();
	if (nChildCnt <= 0)
		return true;

	bool bResult = true;

	// children node
	xml_node* pChildrenNode = pDoc->allocate_node(rapidxml::node_element, pDoc->allocate_string("children"));
	pObjectNode->append_node(pChildrenNode);

	for (int i = 0; i < nChildCnt; i++)
	{
		YY_OBJECTID id = GetChildByIndex(i);
		BaseObject* pChildObject = GetMgr()->Find(id);
		if (nullptr == pChildObject)
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
		xml_node* pTmpNode = pDoc->allocate_node(rapidxml::node_element, pDoc->allocate_string("node"));
		//xml_attribute* pAttribute = pDoc->allocate_attribute(pDoc->allocate_string("class"), pDoc->allocate_string(pChildEntity->GetMetaClass()->name.c_str()));
		//pTmpNode->append_attribute(pAttribute);

		pChildEntity->serializeToNode(pDoc, pTmpNode);
		pChildrenNode->append_node(pTmpNode);
	}

	return bResult;
}

bool Entity::serializeProperties(BaseObject* pObject, xml_document* pDoc, xml_node* pObjectNode)
{
	if (NULL == pDoc || NULL == pObject || nullptr == pObjectNode)
		return false;

	bool bResult = true;
	IReflectionMgr* pReflectionMgr = GetReflectionMgr();
	MetaClass* pMetaClass = pObject->GetMetaClass();

	// class name
	xml_attribute* pAttributeKey = pDoc->allocate_attribute("class", pDoc->allocate_string(pMetaClass->name.c_str()));
	pObjectNode->append_attribute(pAttributeKey);

	std::vector<MetaField*> fields;
	pReflectionMgr->GetAllMetaField(pMetaClass->name, fields);
	for (int i=0; i<fields.size(); i++)
	{
		MetaField* pMetaField = fields[i];
		std::string key = pMetaField->name;

		if (pMetaField->var_type == YVT_CLASS)
		{
			// property is object
			std::string className = pMetaField->type_name;
			void* pInstance = pMetaField->Get(this);

			MetaClass* pChildInstanceMetaClass = pReflectionMgr->FindMetaClass(className);
			throw_assert(NULL != pChildInstanceMetaClass, "null check.");
			auto itor = pChildInstanceMetaClass->fields.begin();
			for (; itor != pChildInstanceMetaClass->fields.end(); itor++)
			{
				MetaField* pField = itor->second;
				if (pField->var_type == YVT_CLASS)	// not support
				{
					bResult = false;
					continue;
				}

				YY::Var value = pReflectionMgr->GetBaseFieldVal(pInstance, pField);
				std::string strFieldVal = value.SerializeTo(); //getStr(pMetaField->var_type, pFieldVal);
				std::string name = className + "." + pField->name;
				xml_attribute* pAttributeKey = pDoc->allocate_attribute(pDoc->allocate_string(name.c_str()), pDoc->allocate_string(strFieldVal.c_str()));
				pObjectNode->append_attribute(pAttributeKey);
			}
		}
		else
		{
			// normal property node
			YY::Var value = pReflectionMgr->GetBaseFieldVal(pObject, pMetaField);
			std::string strFieldVal = value.SerializeTo(); //getStr(pMetaField->var_type, pFieldVal);
			xml_attribute* pAttributeKey = pDoc->allocate_attribute(pDoc->allocate_string(key.c_str()), pDoc->allocate_string(strFieldVal.c_str()));
			pObjectNode->append_attribute(pAttributeKey);
		}
	}

	return bResult;
}



bool Entity::serializeToNode(xml_document* pDoc, xml_node* pObjectNode)
{
	if (NULL == pDoc || NULL == pObjectNode)
		return false;

	bool bResult = true;
	if (!serializeProperties(this, pDoc, pObjectNode))
		bResult = false;

	if (!serializeComponents(pDoc, pObjectNode))
		bResult = false;

	if (!serializeChildren(pDoc, pObjectNode))
		bResult = false;
	return bResult;
}










void Entity::OnCreate(const VariantMap& args)
{
	BaseObject::OnCreate(args);

	YY_OBJECTID parent = YY_INVALID_OBJECTID;
	YY_OBJECTID prev = YY_INVALID_OBJECTID;
	YY_OBJECTID next = YY_INVALID_OBJECTID;
	auto itor = args.find(PROP_ENTITY_PARENT);
	if (itor != args.end())
		parent = itor->second.GetInt64();

	itor = args.find(PROP_ENTITY_PREV);
	if (itor != args.end())
		prev = itor->second.GetInt64();

	itor = args.find(PROP_ENTITY_NEXT);
	if (itor != args.end())
		next = itor->second.GetInt64();

	BaseObject* pParent = GetMgr()->Find(parent);
	BaseObject* pPrev = GetMgr()->Find(prev);
	BaseObject* pNext = GetMgr()->Find(next);
	if (nullptr != pParent)
	{
		if (!pParent->IsInstanceOf("Entity"))
			return;

		Entity* pParentEntity = (Entity*)pParent;
		if (pPrev)
		{
			if (!pPrev->IsInstanceOf("Entity"))
				return;

			pParentEntity->InsertAfter(this, (Entity*)pPrev);
		}
		else if (pNext)
		{
			if (!pNext->IsInstanceOf("Entity"))
				return;

			pParentEntity->InsertBefore(this, (Entity*)pNext);
		}
		else
				pParentEntity->AddChild(this);
	}
}

void Entity::OnDestroyed()
{
	ClearChildren();
	GetMgr()->Destroy(GetID());
}


Entity* Entity::Duplicate()
{
	std::string str = SerializeTo();
	return Entity::parseFrom(GetMgr(), str);
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


