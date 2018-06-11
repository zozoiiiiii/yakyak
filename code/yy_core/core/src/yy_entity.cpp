#include "../inc/yy_entity.h"
#include "../inc/yy_entitymgr.h"
NS_YY_BEGIN
void Entity::AddChild(YY_ENTITYID id)
{
    if (IsChildExist(id))
        return;

    m_children.push_back(id);
}

void Entity::RemoveChild(YY_ENTITYID id)
{
    int nSize = m_children.size();
    for (int i = 0; i < nSize; i++)
    {
        if (id == m_children[i])
        {
            m_children.erase(m_children.begin() + i);
            return;
        }
    }
}

bool Entity::IsChildExist(YY_ENTITYID id)
{
    int nSize = m_children.size();
    for (int i = 0; i < nSize; i++)
    {
        if (id == m_children[i])
            return true;
    }

    return false;
}


YY::VarList Entity::GetChildList()
{
    YY::VarList args;

    int nSize = m_children.size();
    for (int i = 0; i < nSize; i++)
    {
        args << m_children[i];
    }

    return args;
}

void Entity::SetParent(YY_ENTITYID id)
{
    throw_assert(!IsChildExist(id), "child entity could not become parent entity.")
    m_parent = id;
}

YY_ENTITYID Entity::GetParent()
{
    return m_parent;
}

bool Entity::SerializeTo(YY::VarList& args)
{
    ISerializer* pSerialize = GetMgr()->GetReflectionMgr()->GetSerializer();
    return pSerialize->SerializeTo(this, m_pMetaClass->name, args);
}

bool Entity::ParseFrom(const YY::VarList& args, int& read_index)
{
    ISerializer* pSerialize = GetMgr()->GetReflectionMgr()->GetSerializer();
    return pSerialize->ParseFrom(this, m_pMetaClass->name, args, read_index);
}

void Entity::Invoke(void* result, const char* name, void* parameters[])
{
    throw_assert(NULL!=m_pMetaClass, "null check.");
    std::map<std::string, MetaMethod*>::iterator itor = m_pMetaClass->methods.find(name);
    throw_assert(itor != m_pMetaClass->methods.end(), "not exist method:"<<name);

    MetaMethod* pMethodCreator = m_pMetaClass->methods[name];
    pMethodCreator->Invoke(result, this, parameters);
}


void Entity::Invoke(void* result, const char* name)
{
    void* parameters[1];
    Invoke(result, name, parameters);
}

void Entity::Invoke(void* result, const char* name, void* p1)
{
    void* parameters[1];
    parameters[0]=p1;
    Invoke(result, name, parameters);
}

void Entity::Invoke(void* result, const char* name, void* p1, void* p2)
{
    void* parameters[2];
    parameters[0]=p1;
    parameters[1]=p2;
    Invoke(result, name, parameters);
}

void Entity::Invoke(void* result, const char* name, void* p1, void* p2, void* p3)
{
    void* parameters[3];
    parameters[0]=p1;
    parameters[1]=p2;
    parameters[2]=p3;
    Invoke(result, name, parameters);
}


void Entity::Invoke(void* result, const char* name, void* p1, void* p2, void* p3, void* p4)
{
    void* parameters[4];
    parameters[0]=p1;
    parameters[1]=p2;
    parameters[2]=p3;
    parameters[3]=p4;
    Invoke(result, name, parameters);
}

void Entity::Invoke(void* result, const char* name, void* p1, void* p2, void* p3, void* p4, void* p5)
{
	void* parameters[5];
	parameters[0]=p1;
	parameters[1]=p2;
	parameters[2]=p3;
	parameters[3]=p4;
	parameters[4]=p5;
	Invoke(result, name, parameters);
}
// 
// int Entity::GetPropInt(const std::string& name)
// {
//     int tmp;
//     MetaField* pMetaField = m_pMetaClass->fields[name];
//     pMetaField->field->Get(this, &tmp);
//     return tmp;
// }
// 
// void Entity::SetPropInt(const std::string& name, int val)
// {
//     MetaField* pMetaField = m_pMetaClass->fields[name];
//     pMetaField->Set(this, &val);
// }
// 
// float Entity::GetPropFloat(const std::string& name)
// {
//     float tmp;
//     MetaField* pMetaField = m_pMetaClass->fields[name];
//     pMetaField->field->Get(this, &tmp);
//     return tmp;
// }
// 
// void Entity::SetPropFloat(const std::string& name, float val)
// {
//     MetaField* pMetaField = m_pMetaClass->fields[name];
//     pMetaField->Set(this, &val);
// }
// 
// std::string Entity::GetPropString(const std::string& name)
// {
//     std::string tmp;
//     MetaField* pMetaField = m_pMetaClass->fields[name];
//     pMetaField->field->Get(this, &tmp);
//     return tmp;
// }
// 
// void Entity::SetPropString(const std::string& name, const std::string& val)
// {
//     MetaField* pMetaField = m_pMetaClass->fields[name];
//     pMetaField->Set(this, &val);
// }


///////////////custom prop

bool Entity::IsCustomPropExist(const std::string& name)
{
    std::map<std::string, YY::Var>::iterator itor = m_custom_props.find(name);
    if(itor==m_custom_props.end())
    {
        return false;
    }

    return true;
}

void Entity::SetCustomProp(const std::string& name, const YY::Var& val)
{
    m_custom_props[name] = val;
}

int Entity::GetCustomPropInt(const std::string& name)
{
    if(!IsCustomPropExist(name))
        return 0;

    return m_custom_props[name].GetInt();
}


float Entity::GetCustomPropFloat(const std::string& name)
{
    if(!IsCustomPropExist(name))
        return 0;

    return m_custom_props[name].GetFloat();
}


std::string Entity::GetCustomPropString(const std::string& name)
{
    if(!IsCustomPropExist(name))
        return 0;

    return m_custom_props[name].GetStr();
}

NS_YY_END


