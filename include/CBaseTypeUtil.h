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

using namespace std;

class CBaseTypeUtil{
public:
    // 原始类型
    static int m_IntValue;
    static int *m_pIntValue;
    // 原始类型名字
    static string m_strIntName, m_strPIntName;


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


};



#endif //ICEKNIFELIB_CBASETYPEUTIL_H
