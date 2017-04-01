/*
 * Created by ice on 2017/3/20.
 *
 * 判断是否是基本类型  和释放基本类型
 *
 *
 *
*/

#ifndef ICEKNIFELIB_CBASETYPEUTIL_H
#define ICEKNIFELIB_CBASETYPEUTIL_H

#include <string>
#include "LVcDefine.h"

using namespace std;

class CBaseTypeUtil{
public:
    // 原始类型
    static int m_IntValue;
    static int *m_pIntValue;
    static string m_strIntName, m_strPIntName;

    static byte m_byteValue;
    static byte* m_pByteValue;
    static string m_strByteName, m_strPByteName;

    static bool m_boolValue;
    static bool* m_pBoolValue;
    static string m_strBoolName, m_strPBoolName;

    static short m_shortValue;
    static short *m_pShortValue;
    static string m_strShortName, m_strPShortName;

    static INT64 m_Int64Value;
    static INT64 * m_pInt64Value;
    static string m_strInt64Name, m_strPInt64Name;

    static float m_floatValue;
    static float * m_pfloatValue;
    static string m_strfloatName, m_strPfloatName;

    static double m_doubleValue;
    static double * m_pdoubleValue;
    static string m_strdoubleName, m_strPdoubleName;

    static string m_stringValue;
    static string * m_pstringValue;
    static string m_strstringName, m_strPstringName;



    template <typename TVALUE>
    static bool IsBaseType(TVALUE tvPara){
        string strParaName = typeid(tvPara).name();
        return (m_strIntName == strParaName);
    }

    template <typename TVALUE> //
    static bool FreeBaseType(TVALUE tvPara){
        string strParaName = typeid(tvPara).name();
        if (strParaName == m_strIntName ){   //非指针时直接返回true   不用释放
            return true;
        }
        if (strParaName == m_strPIntName){
            int *pInt = (int*)tvPara;
            delete pInt;
        }
    }
    // 获取变量的类型名字
    template <typename TVALUE> //
    static string GetTypeName(TVALUE tvPara) {
        string strParaName = typeid(tvPara).name();
        return strParaName;
    }
    //----------------------------------------------
    template <typename TVALUE>   //是否是int类型
    static bool IsInt(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strIntName;
    }
    static bool IsInt(string strParaName){
        return strParaName == m_strIntName;
    }
    template <typename TVALUE>   //是否是int 指针
    static bool IsPInt(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strPIntName;
    }
    static bool IsPInt(string strParaName){
        return strParaName == m_strPIntName;
    }
    //----------------------------------------------
    template <typename TVALUE>   //是否是byte类型
    static bool IsByte(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strByteName;
    }
    static bool IsByte(string strParaName){
        return strParaName == m_strByteName;
    }
    template <typename TVALUE>   //是否是byte 指针
    static bool IsPByte(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strPByteName;
    }
    static bool IsPByte(string strParaName){
        return strParaName == m_strPByteName;
    }
//----------------------------------------------
    template <typename TVALUE>   //是否是bool类型
    static bool IsBool(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strBoolName;
    }
    static bool IsBool(string strParaName){
        return strParaName == m_strBoolName;
    }
    template <typename TVALUE>   //是否是byte 指针
    static bool IsPBool(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strPBoolName;
    }
    static bool IsPBool(string strParaName){
        return strParaName == m_strPBoolName;
    }
    //----------------------------------------------
    template <typename TVALUE>   //是否是short类型
    static bool IsShort(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strShortName;
    }
    static bool IsShort(string strParaName){
        return strParaName == m_strShortName;
    }
    template <typename TVALUE>   //是否是short指针
    static bool IsPShort(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strPShortName;
    }
    static bool IsPShort(string strParaName){
        return strParaName == m_strPShortName;
    }
    //----------------------------------------------
    template <typename TVALUE>   //是否是int64类型
    static bool IsInt64(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strInt64Name;
    }
    static bool IsInt64(string strParaName){
        return strParaName == m_strInt64Name;
    }
    template <typename TVALUE>   //是否是int64指针
    static bool IsPInt64(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strPInt64Name;
    }
    static bool IsPInt64(string strParaName){
        return strParaName == m_strPInt64Name;
    }
    //----------------------------------------------
    template <typename TVALUE>   //是否是 float 类型
    static bool IsFloat(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strfloatName;
    }
    static bool IsFloat(string strParaName){
        return strParaName == m_strfloatName;
    }
    template <typename TVALUE>   //是否是 float 指针
    static bool IsPFloat(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strPfloatName;
    }
    static bool IsPFloat(string strParaName){
        return strParaName == m_strPfloatName;
    }
    //----------------------------------------------
    template <typename TVALUE>   //是否是 double 类型
    static bool IsDouble(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strdoubleName;
    }
    static bool IsDouble(string strParaName){
        return strParaName == m_strfloatName;
    }
    template <typename TVALUE>   //是否是 double 指针
    static bool IsPDouble(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strPdoubleName;
    }
    static bool IsPDouble(string strParaName){
        return strParaName == m_strPdoubleName;
    }
    //----------------------------------------------
    template <typename TVALUE>   //是否是 string 类型
    static bool IsString(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strstringName;
    }
    static bool IsString(string strParaName){
        return strParaName == m_strstringName;
    }
    template <typename TVALUE>   //是否是 string 指针
    static bool IsPString(TVALUE tvPara){
        return GetTypeName(tvPara) == m_strPstringName;
    }
    static bool IsPString(string strParaName){
        return strParaName == m_strPstringName;
    }

};



#endif //ICEKNIFELIB_CBASETYPEUTIL_H
