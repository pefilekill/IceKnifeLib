//
// Created by ice on 2017/3/20.
//

#include "CBaseTypeUtil.h"

int CBaseTypeUtil::m_IntValue = 0;
int* CBaseTypeUtil::m_pIntValue = NULL;
string CBaseTypeUtil::m_strIntName = typeid(CBaseTypeUtil::m_IntValue).name();
string CBaseTypeUtil::m_strPIntName = typeid(CBaseTypeUtil::m_pIntValue).name();

byte  CBaseTypeUtil::m_byteValue = '0';
byte*  CBaseTypeUtil::m_pByteValue = NULL;
string CBaseTypeUtil::m_strByteName = typeid(CBaseTypeUtil::m_byteValue).name();
string CBaseTypeUtil::m_strPByteName = typeid(CBaseTypeUtil::m_pByteValue).name();

bool  CBaseTypeUtil::m_boolValue = false;
bool*  CBaseTypeUtil::m_pBoolValue = NULL;
string CBaseTypeUtil::m_strBoolName = typeid(CBaseTypeUtil::m_boolValue).name();
string CBaseTypeUtil::m_strPBoolName = typeid(CBaseTypeUtil::m_pBoolValue).name();

short  CBaseTypeUtil::m_shortValue = 0;
short*  CBaseTypeUtil::m_pShortValue = NULL;
string CBaseTypeUtil::m_strShortName = typeid(CBaseTypeUtil::m_shortValue).name();
string CBaseTypeUtil::m_strPShortName = typeid(CBaseTypeUtil::m_pShortValue).name();

INT64  CBaseTypeUtil::m_Int64Value = 0;
INT64*  CBaseTypeUtil::m_pInt64Value = NULL;
string CBaseTypeUtil::m_strInt64Name = typeid(CBaseTypeUtil::m_Int64Value).name();
string CBaseTypeUtil::m_strPInt64Name = typeid(CBaseTypeUtil::m_pInt64Value).name();

float  CBaseTypeUtil::m_floatValue = 0;
float*  CBaseTypeUtil::m_pfloatValue = NULL;
string CBaseTypeUtil::m_strfloatName = typeid(CBaseTypeUtil::m_floatValue).name();
string CBaseTypeUtil::m_strPfloatName = typeid(CBaseTypeUtil::m_pfloatValue).name();

double  CBaseTypeUtil::m_doubleValue = 0;
double*  CBaseTypeUtil::m_pdoubleValue = NULL;
string CBaseTypeUtil::m_strdoubleName = typeid(CBaseTypeUtil::m_doubleValue).name();
string CBaseTypeUtil::m_strPdoubleName = typeid(CBaseTypeUtil::m_pdoubleValue).name();

string  CBaseTypeUtil::m_stringValue = 0;
string*  CBaseTypeUtil::m_pstringValue = NULL;
string CBaseTypeUtil::m_strstringName = typeid(CBaseTypeUtil::m_stringValue).name();
string CBaseTypeUtil::m_strPstringName = typeid(CBaseTypeUtil::m_pstringValue).name();




