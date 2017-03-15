//
// Created by ice on 17/3/10.
//

#ifndef LOADER_CONCURRENTLIST_H
#define LOADER_CONCURRENTLIST_H

#include <string>
#include <vector>
#include <pthread.h>

using namespace std;

template <typename TVALUE>
class ConcurrentVector {
public:
    ConcurrentVector() {
        pthread_mutex_init(&m_mtxVector, NULL);
    }

    ~ConcurrentVector() {
        pthread_mutex_destroy(&m_mtxVector);
    }
    // 重载  只读
    TVALUE operator[] (int iIndex){
        TVALUE tvRet;
        GetValueAt(iIndex, tvRet);  //加锁依赖GetValueAt中的保证
        return tvRet;
    }

    //插入
    void Insert(TVALUE tvPara) {
        pthread_mutex_lock(&m_mtxVector);
        auto atValue = tvPara;
        m_vecData.push_back(atValue);
        pthread_mutex_unlock(&m_mtxVector);
    }

    // 获取大小
    int GetSize() {
        int iSize = 0;
        pthread_mutex_lock(&m_mtxVector);
        iSize = m_vecData.size();
        pthread_mutex_unlock(&m_mtxVector);
        return iSize;
    }

    //获取指定index的值
    bool GetValueAt(int iIndex, TVALUE &tvOut) {
        bool bRet = false;
        pthread_mutex_lock(&m_mtxVector);
        if (m_vecData.size() > iIndex){
            tvOut = m_vecData[iIndex];
            bRet = true;
        }
        pthread_mutex_unlock(&m_mtxVector);
        return bRet;
    }
    // 设置值
    void SetValueAt(int iIndex, TVALUE tvPara){
        pthread_mutex_lock(&m_mtxVector);
        if (m_vecData.size() > iIndex){
             m_vecData[iIndex] = tvPara;
        }
        pthread_mutex_unlock(&m_mtxVector);
    }

    //获取首个
    bool GetFirstValue(TVALUE &tvOut){
        return GetValueAt(0, tvOut);
    }
    //删除指定的值
    void RemoveAt(int iIndex){
        pthread_mutex_lock(&m_mtxVector);
        if (m_vecData.size() > iIndex){
            m_vecData.erase(m_vecData.begin() + iIndex);
        }
        pthread_mutex_unlock(&m_mtxVector);
    }
    //删除全部
    void RemoveAll(){
        pthread_mutex_lock(&m_mtxVector);
        m_vecData.clear();
        pthread_mutex_unlock(&m_mtxVector);
    }

private:
    //
    pthread_mutex_t m_mtxVector;
    //值
    vector<TVALUE> m_vecData;
};


#endif //LOADER_CONCURRENTLIST_H
