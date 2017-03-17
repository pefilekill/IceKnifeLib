/*
* Created by ice on 17/3/16.
 *
 * 存储任意类型的list  对于用户自定义的类型，需要继承实现释放部分
 * 必须要有默认的构造函数
 * 不支持链表
*/

#ifndef ICEKNIFELIB_CANYLIST_H
#define ICEKNIFELIB_CANYLIST_H

#include "PreDefine.h"
#include <string>
#include "ConcurrentMap.h"
#include "ConcurrentVector.h"

using namespace std;

class CAnyList {
public:
    CAnyList() {
        m_iIndex = 0;
    }

    //  自动释放可识别的指针
    ~CAnyList() {
        Free();


    }

    template<typename TVAUE>
    void Add(TVAUE tvPara) {
        if (IsBaseType(tvPara)) {
            TVAUE *pTemp = new TVAUE();
            *pTemp = tvPara;
            string strParaType = typeid(tvPara).name();
            m_mapTypeTable.Insert(m_iIndex, strParaType);
            m_mapData.Insert(m_iIndex, pTemp);
            m_iIndex++;
        } else {
            string strParaType = typeid(tvPara).name();
            CPubFunc::PrintString("不支持的类型:" + strParaType);
        }
    }

    // 释放已知类型
    void Free() {
        for (int iLoop = 0; iLoop < m_iIndex; ++iLoop) {
            if (m_mapData.ContainsKey(iLoop) && m_mapTypeTable.ContainsKey(iLoop)) {
                string strType = m_mapTypeTable.GetValue(iLoop);
                PVOID pValue = m_mapData.GetValue(iLoop);
                if (DeleteVars(strType, pValue)) {
                    m_mapTypeTable.Remove(iLoop);
                    m_mapData.Remove(iLoop);
                }
            }
        }

    }

    // 是否是基础类型  int 之类
    template<typename TVAUE>
    bool IsBaseType(TVAUE tvPara) {
        string strParaType = typeid(tvPara).name();

        static int tp_IntValue = 0;
        static string tp_stringValue = "";

        string strIntType = typeid(tp_IntValue).name();
        string strStringType = typeid(tp_stringValue).name();
        if (strParaType == strIntType || strParaType == strStringType) {
            return true;
        }
        return false;
    }

    // 删除指针
    bool DeleteVars(string strParaType, PVOID pPara) {
        static int tp_IntValue = 0;
        static string tp_stringValue = "";

        string strIntType = typeid(tp_IntValue).name();
        string strStringType = typeid(tp_stringValue).name();

        bool bDel = true;
        if (strIntType == strParaType) {
            int *pInt = (int *) pPara;
            delete pInt;
        } else if (strStringType == strParaType) {
            string *pStr = (string *) pPara;
            delete pStr;
        } else {
            bDel = false;
        }
        return bDel;
    }

private:
    int m_iIndex;
    // 纪录类型的map   key:index value: type
    ConcurrentMap<int, string> m_mapTypeTable;
    // 存储数据
    ConcurrentMap<int, PVOID> m_mapData;
};


#endif //ICEKNIFELIB_CANYLIST_H
