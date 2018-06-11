/************************************************************************/
/*
@author:    junliang
@brief:     注册消息和消息处理函数，invoke指定消息和函数, 只限于本类
@time:      20140110
*/
/************************************************************************/
#ifndef _REG_HPP_
#define _REG_HPP_

#include "core/inc/yy_exception.h"
#include <map>

#define MAXREGERRLEN 100

NS_YY_BEGIN


template<class Key, class Param1, class Param2, class Retn = void, class Compare = std::less<Key>>
class Dispatch2
{
public:
    typedef fastdelegate::FastDelegate3<Param1, Param2, Retn> Fun;

    void Bind(const Key& key, Fun bindItem)
    {
        std::map<Key, Fun>::iterator itor = m_mpBindContainer.find(key);
        throw_assert(itor == m_mpBindContainer.end(),"存在相同key");

        if (NULL != bindItem)
            m_mpBindContainer[key] = bindItem;
    }

    Retn Invoke(const Key& key, Param1 arg1, Param2 arg2)
    {
        std::map<Key, Fun>::iterator it = m_mpBindContainer.find(key);
        throw_assert(m_mpBindContainer.end() != it, "not support key:"<<key);
        return it->second(arg1, arg2, arg3);
    }

private:
    std::map<Key, Fun> m_mpBindContainer;
};

NS_YY_END
#endif