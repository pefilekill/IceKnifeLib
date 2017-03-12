//
// Created by wangwenzhe on 17/3/9.
//

#ifndef LOADER_CONCURRENTMAP_H
#define LOADER_CONCURRENTMAP_H

#include <vector>
#include <string>
#include <pthread.h>
#include <map>
#include "CPubFuncUtil.h"

using namespace std;

template<typename TKEY, typename TVALUE>
class ConcurrentMap {
public:
    ConcurrentMap() {
        pthread_mutex_init(&m_mtxMap, NULL);
    }

    ~ConcurrentMap() {
        pthread_mutex_destroy(&m_mtxMap);
    }
    //是否包含
    bool IsExist(TKEY keyCheck){
        bool bExist = false;
        pthread_mutex_lock(&m_mtxMap);
        bExist = (find(m_vecKey.begin(), m_vecKey.end(), keyCheck) != m_vecKey.end());
        pthread_mutex_unlock(&m_mtxMap);
        return bExist;
    }

    //插入
    void Insert(TKEY keyPara, TVALUE valuePara) {
        pthread_mutex_lock(&m_mtxMap);
        auto atKey = keyPara;
        auto atValue = valuePara;
        m_vecKey.push_back(atKey);
        m_vecValue.push_back(atValue);
        pthread_mutex_unlock(&m_mtxMap);
    }

    //读取
    TVALUE GetValue(TKEY keyPara) {
        pthread_mutex_lock(&m_mtxMap);
        auto atSearch = find(m_vecKey.begin(), m_vecKey.end(), keyPara);
        int iSub = atSearch - m_vecKey.begin(); //这里可能会有波浪线  不影响
        auto atValue = m_vecValue[iSub];
        pthread_mutex_unlock(&m_mtxMap);
        return atValue;
    }

    // 获取大小
    int GetSize() {
        return m_vecKey.size();
    }

    //获取指定index的值
    TVALUE GetIndexAt(int iIndex) {
        pthread_mutex_lock(&m_mtxMap);
        auto atRet = m_vecValue[iIndex];
        pthread_mutex_unlock(&m_mtxMap);
        return atRet;
    }
    // 移除
    void Remove(TKEY keyPara){
        pthread_mutex_lock(&m_mtxMap);
        auto atSearchKey = find(m_vecKey.begin(), m_vecKey.end(), keyPara);
        auto atSearchValue = atSearchKey - m_vecKey.begin() + m_vecValue.begin(); //这里可能会有波浪线  不影响
        m_vecKey.erase(atSearchKey);
        m_vecValue.erase(atSearchValue);
        pthread_mutex_unlock(&m_mtxMap);
        return ;
    }

private:
    //
    pthread_mutex_t m_mtxMap;
    //值
    vector<TKEY> m_vecKey;
    vector<TVALUE> m_vecValue;
};


#endif //LOADER_CONCURRENTMAP_H