//
// Created by ice on 2017/3/20.
//

#include "CBaseTypeUtil.h"

int CBaseTypeUtil::m_IntValue = 0;
int* CBaseTypeUtil::m_pIntValue = NULL;
byte  CBaseTypeUtil::m_byteValue = '0';
byte*  CBaseTypeUtil::m_pByteValue = NULL;




//////////////////
string CBaseTypeUtil::m_strIntName = typeid(CBaseTypeUtil::m_IntValue).name();
string CBaseTypeUtil::m_strPIntName = typeid(CBaseTypeUtil::m_pIntValue).name();
string CBaseTypeUtil::m_strByteName = typeid(CBaseTypeUtil::m_byteValue).name();
string CBaseTypeUtil::m_strPByteName = typeid(CBaseTypeUtil::m_pByteValue).name();