//
// Created by ice on 2017/3/20.
//

#include "CBaseTypeUtil.h"

int CBaseTypeUtil::m_IntValue = 0;
int* CBaseTypeUtil::m_pIntValue = NULL;




//////////////////
string CBaseTypeUtil::m_strIntName = typeid(CBaseTypeUtil::m_IntValue).name();
string CBaseTypeUtil::m_strPIntName = typeid(CBaseTypeUtil::m_pIntValue).name();