/************************************************************************/
/*
@author:  junliang
@brief:   
    debug:assert
    release: throw + stack trace

@time:    20130401
*/
/************************************************************************/
#pragma once
#include "reflection/yy_type.h"
#include <stdio.h>
#include <assert.h>
#include <string>
#include <exception>
#include <sstream>
#include <iostream>

NS_YY_BEGIN
class ExceptionStreamFormatter
{
public:
    operator std::string() const {
        return stream.str();
    }

    template<typename T>
    ExceptionStreamFormatter& operator << (const T& value) {
        stream << value;
        return *this;
    }

private:
    std::ostringstream stream;
};

class AssertFailureException : public std::exception
{
public:

    AssertFailureException(const std::string& expression, const std::string& file, int line, const std::string& msg="");
    virtual ~AssertFailureException() throw() {}

    const char* What() const{return m_what.c_str();}

private:
    std::string m_what;
};
NS_YY_END

#define throw_assert(_expr, _msg)    \
if (!(_expr)){ throw YY::AssertFailureException(#_expr, __FILE__, __LINE__, (YY::ExceptionStreamFormatter() << _msg)); }
