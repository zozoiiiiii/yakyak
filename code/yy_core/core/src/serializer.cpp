#include "serializer.h"


NS_YY_BEGIN


bool Serializer::SetPropVal(void* pClassInstance, const MetaField* prop, const YY::Var& var)
{
    // check type.
    int nType = prop->var_type;
    if(nType != var.GetType())
        return false;

    std::string strTmp;
    switch(nType)
    {
    case YVT_BOOL:
        {
            bool tmp = var.GetBool();
            prop->Set(pClassInstance, &tmp);
        }
        break;
    case YVT_SINT32:
        {
            int tmp = var.GetInt();
            prop->Set(pClassInstance, &tmp);
        }
        break;
    case YVT_SINT64:
        {
            sint64 tmp = var.GetInt64();
            prop->Set(pClassInstance, &tmp);
        }
        break;
    case YVT_FLOAT:
        {
            float tmp = var.GetFloat();
            prop->Set(pClassInstance, &tmp);
        }
        break;
    case YVT_DOUBLE:
        {
            double tmp = var.GetDouble();
            prop->Set(pClassInstance, &tmp);
        }
        break;
    case YVT_STRING:
        {
//             if(prop->name == "string")
//             {
                std::string strTmp = var.GetStr();
                prop->Set(pClassInstance, &strTmp);
//             }
//             else
//             {
//                 const char* tmp = var.GetStr();
//                 prop->Set(pClassInstance, &tmp);
//             }
        }
        break;
        //      case YVT_WSTRING:
        //          return &m_str;
        //      case YVT_POINTER:
        //          return &m_pointer;
    default:
        //throw_assert(false, "not support.");
        return false;
    }

    return true;
}

void Serializer::GetPropVal(void* pClassInstance, const MetaField* prop, YY::Var& var)
{
    int nType = prop->var_type;
    throw_assert(nType > YVT_UNKNOWN && nType <= YVT_MAX, "type error.");

    void* pVal = prop->Get(pClassInstance);

    switch(nType)
    {
    case YVT_BOOL:
        var = *(bool*)pVal;
        break;
    case YVT_SINT32:
        var = *(int*)pVal;
        break;
    case YVT_SINT64:
        var = *(sint64*)pVal;
        break;
    case YVT_FLOAT:
        var = *(float*)pVal;
        break;
    case YVT_DOUBLE:
        var = *(double*)pVal;
        break;
    case YVT_STRING:
        {
            var = *(std::string*)pVal;
//             if(prop->name == "string")
//                 var = *(std::string*)pVal;
//             else
//                 var = (char*)pVal;
        }
        break;
     case YVT_WSTRING:
         {
             throw_assert(false, "not support here.");
         }
         break;
        //         var.SetWstr((wchar_t*)pVal);
        //         break;
    default:
        throw_assert(false,"not support");
    }
}

// 
// int Serializer::GetType(const MetaField* prop)
// {
//     std::string sz_vartype = prop->var_type;
// 
//     int vartype = YVT_UNKNOWN;
//     if (sz_vartype == "bool")
//     {
//         vartype = YVT_BOOL;
//     }
//     else if (sz_vartype =="int")
//     {
//         vartype = YVT_SINT32;
//     }
//     else if (sz_vartype == "sint64")
//     {
//         vartype = YVT_SINT64;
//     }
//     else if (sz_vartype == "float")
//     {
//         vartype = YVT_FLOAT;
//     }
//     else if (sz_vartype == "double")
//     {
//         vartype = YVT_DOUBLE;
//     }
//     else if (sz_vartype == "string")
//     {
//         vartype = YVT_STRING;
//     }
//     else if (sz_vartype == "char")
//     {
//         // use string instead of char
//         //if(prop->nTypeFlag == YY::VF_POINTER || prop->field == YY::VF_CONST_POINTER)
//        //     vartype = YVT_STRING;
//     }
//     else if (sz_vartype == "wstring")
//     {
//         vartype = YVT_WSTRING;
//     }
//     else
//     {
//         //MetaClass* pCreator = FindCreator(sz_vartype);
//        // if(!pCreator)
//        //     throw_assert(false, "error.");
// 
//         vartype = YVT_CLASS;
//     }
// 
//     return vartype;
// }


bool Serializer::ParseFrom(void* pClassInstance, const std::string& name, const YY::VarList& args, int& read_index)
{
    std::vector<std::string> serializedClasses;
    read_index=0;
    return InnerParseFrom(pClassInstance, name, args, read_index, serializedClasses);
}

bool Serializer::InnerParseFrom(void* pClassInstance, const std::string& name, const YY::VarList& args, 
                                   int& nIndex, std::vector<std::string>& serializedClasses)
{
    throw_assert(NULL!=pClassInstance, "null check.");

    // check is this class exist.
    int nNum = (int)serializedClasses.size();
    for(int i=0; i<nNum; i++)
    {
        if(name == serializedClasses[i])
            return false;
    }
    serializedClasses.push_back(name);

    // parse from prop data
    MetaClass* pCreator = m_pReflectionMgr->FindCreator(name);
    if(NULL == pCreator)
        return false;

    // check prop size
    int nSize = (int)pCreator->fields.size();
    int nTmpSize = args.GetInt(nIndex);
    nIndex++;

    if(nSize != nTmpSize)
        return false;

    for(int i=0; i<nSize; i++)
    {
        const char* pPropName = args.GetStr(nIndex);
        nIndex++;
        std::map<std::string, MetaField*>::iterator itor = pCreator->fields.find(pPropName);
        if(itor==pCreator->fields.end())
        {
            return false;
        }

        const MetaField* pMetaField = itor->second;
        int nType = pMetaField->var_type;

        // base type, store in Var
        if(nType!=YVT_UNKNOWN)
        {
            YY::Var val = args.GetVar(nIndex);
            nIndex++;

            bool tmp_result = SetPropVal(pClassInstance, pMetaField, val);
            if(!tmp_result)
                return false;
        }
        else // classes.
        {
            void* pPropClass = pMetaField->Get(pClassInstance);
            bool tmp_ret = InnerParseFrom(pPropClass, pMetaField->name, args, nIndex, serializedClasses);
            nIndex++;

            if(!tmp_ret)
                return false;
        }
    }

    return true;
}

bool Serializer::SerializeTo(void* pClassInstance, const std::string& name, YY::VarList& args)
{
    std::vector<std::string> serializedClasses;
    return InnerSerializeTo(pClassInstance, name, args, serializedClasses);
}

bool Serializer::InnerSerializeTo(void* pClassInstance, const std::string& name, YY::VarList& args , std::vector<std::string>& serializedClasses)
{
     throw_assert(NULL!=pClassInstance, "null check.");
 
     // check is this class exist.
     int nNum = (int)serializedClasses.size();
     for(int i=0; i<nNum; i++)
     {
         if(name == serializedClasses[i])
             return false;
     }
     serializedClasses.push_back(name);
 
     MetaClass* pCreator = m_pReflectionMgr->FindCreator(name);
     if(NULL == pCreator)
         return false;

     int nSize = (int)pCreator->fields.size();
     args<<nSize;
     std::map<std::string, MetaField*>::iterator itor = pCreator->fields.begin();
     for(; itor!=pCreator->fields.end(); itor++)
     {
         const MetaField* prop = itor->second;
         args<<prop->name;
 
         void* pVal = prop->Get(pClassInstance);

         int nType = prop->var_type;
 
         // base type, store in Var
         if(nType!=YVT_UNKNOWN)
         {
             YY::Var propVar;
             GetPropVal(pClassInstance, prop, propVar);
             args<<propVar;
         }
         else // classes.
         {
             void* pPropClass = (void*)(pVal);
             if(!InnerSerializeTo(pPropClass, prop->name, args, serializedClasses))
                 return false;
         }
     }

    return true;
}


NS_YY_END