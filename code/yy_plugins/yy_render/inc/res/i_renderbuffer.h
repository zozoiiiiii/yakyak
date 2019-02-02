/************************************************************************/
/* 
@author:  junliang
@brief:   render buffer
@time:    2017/10/21
*/
/************************************************************************/
#pragma once

#include "i_res.h"
#include "yy_core.h"

class IRenderBuffer : public IRes
{
	YY_VIRTUAL_BEGIN(IRenderBuffer, IRes);
	YY_END
public:
    void SetWidth(int w){m_nWidth=w;}
    int GetWidth(){return m_nWidth;}
    void SetHeight(int h){m_nHeight=h;}
    int GetHeight(){return m_nHeight;}
    void SetFormat(int nFormat){m_nFormat = nFormat;}
    int GetFormat(){return m_nFormat;}
    void SetSizeType(int nSizeType){m_nSizeType=nSizeType;}
    int GetSizeType(){return m_nSizeType;}
    uint32 GetID(){return m_nID;}
    void SetID(uint32 id){m_nID = id;}

    virtual void Generate() = 0;
private:
    int m_nWidth;
    int m_nHeight;
    int m_nSizeType;    // TEX_TYPE_DEVICE
    int m_nFormat;
    uint32 m_nID;
};


