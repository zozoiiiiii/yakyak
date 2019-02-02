/************************************************************************/
/*
@author:  junliang
@brief:   
			1. hierarchy feature.
			2. render feature, position is in Item(2d), VisObj(3d)
			3. component manager

			base object   : create/destroy, meta
			    |
			  entity      : parent/children, component, serialize

			ui: entity+ item transform component + item render component
			3d: entity+ transform component + render component


			// xml file format





@time:    2018/08/14
*/
/************************************************************************/
#pragma once

#include "core/inc/yy_object.h"
#include "core/inc/yy_objectmgr.h"
#include "core/inc/yy_event_mgr.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
typedef rapidxml::xml_document<char> xml_document;
typedef rapidxml::xml_node<char> xml_node;
typedef rapidxml::xml_attribute<char> xml_attribute;

NS_YY_BEGIN

// entity properties
#define PROP_BASEOBJECT_DATA "Prop_BaseObject_Data"	// serialized data
#define PROP_BASEOBJECT_FILE "Prop_BaseObject_File"	// serilized file
#define PROP_ENTITY_PARENT "Prop_Entity_Parent"
#define PROP_ENTITY_PREV "Prop_Entity_Prev"
#define PROP_ENTITY_NEXT "Prop_Entity_Next"

class Component;
class Entity : public BaseObject
{
	YY_BEGIN(Entity, BaseObject);
	YY_FIELD(&Entity::m_name, "name", "");
	YY_END
public:
	static Entity* parseFrom(IObjectMgr* pObjMgr, const std::string& data, YY_OBJECTID parent = YY_INVALID_OBJECTID, YY_OBJECTID prev = YY_INVALID_OBJECTID, YY_OBJECTID next = YY_INVALID_OBJECTID);
	static Entity* ParseFromFile(IObjectMgr* pObjMgr, const std::string& file, YY_OBJECTID parent = YY_INVALID_OBJECTID, YY_OBJECTID prev = YY_INVALID_OBJECTID, YY_OBJECTID next = YY_INVALID_OBJECTID);
	virtual std::string SerializeTo();
	virtual void SerializeToFile(const std::string& file);

	Entity() {}
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroyed();

	Entity* Duplicate();
	void SetName(const std::string& name) { m_name = name; }
	std::string GetName() { return m_name; }

	// parent/children
	void SetParent(Entity* pEntity);
	Entity* FindParent();
	void ClearChildren();
	VarList GetChildren();
	int GetChildCount();
	YY_OBJECTID GetChildByIndex(int index);
	bool IsChildExist(Entity* pEntity);
	void AddChild(Entity* pEntity);
	Entity* FindChild(const std::string& name);
	void RemoveChild(Entity* pEntity);
	void InsertAfter(Entity* pEntity, Entity* after);
	void InsertBefore(Entity* pEntity, Entity* before);
	//virtual void OnAddBatch(IBatchGroup* pBatchGroup);
	//virtual void OnRender(RenderContext* pCxt) {}
	//virtual void OnRenderBefore(RenderContext* pCxt);
	//virtual void OnRenderAfter(RenderContext* pCxt);
	//IRender* GetRender();

	// component
	std::vector<Component*> GetAllComponents();
	Component* FindComponent(const std::string& strClassName);
	Component* AddComponent(const std::string& strClassName);
	void RemoveComponent(YY_OBJECTID id);
	virtual void OnComponentAdded(Component* pComponent) {}
	virtual void OnComponentRemoved(Component* pComponent) {}


	void InvokeEvent(const std::string& event, const YY::VarList& args = YY::VarList());
	virtual void OnEvent(const std::string& event, const YY::VarList& args = YY::VarList()) {}


	// whether the first time to render.
	//virtual bool FirstRender() { return m_bFirstRender; }

private:
	static Entity* parseFromNode(IObjectMgr* pObjMgr, const xml_node* pNode);
	bool parseProperties(const xml_node* pNode, BaseObject* pObject);
	bool parseChildren(const xml_node* pNode);
	bool parseComponents(const xml_node* pNode);

	bool serializeToNode(xml_document* pDoc, xml_node* pObjectNode);
	bool serializeComponents(xml_document* pDoc, xml_node* pObjectNode);
	bool serializeChildren(xml_document* pDoc, xml_node* pObjectNode);
	bool serializeProperties(BaseObject* pObject, xml_document* pDoc, xml_node* pObjectNode);
private:
	//bool m_bFirstRender;
	std::string m_name;
	//IRender* m_pRender;
	YY_OBJECTID m_parent;
	std::vector<YY_OBJECTID> m_children;
	std::map<std::string, Component*> m_components;
};













class Component : public BaseObject
{
	YY_BEGIN(Component, BaseObject);
	YY_END
public:
	Entity * GetOwner() { return m_pOwner; }
	virtual void OnAdded() {}
	virtual void OnRemoved() {}
//	virtual void OnRenderBefore(RenderContext* pCxt) {}
//	virtual void OnRenderAfter(RenderContext* pCxt) {}
	virtual void OnEvent(const std::string& event, const YY::VarList& args = YY::VarList()) {};
private:
	void SetOwner(Entity* pOwner) { m_pOwner = pOwner; }
private:
	Entity * m_pOwner;
	friend class Entity;
};


class System : public BaseObject
{
public:
	virtual void onComponentAdded(YY_OBJECTID id) {}
	virtual void onComponentRemoved(YY_OBJECTID id) {}
	virtual void onComponentUpdated(YY_OBJECTID id) {}
};

NS_YY_END


