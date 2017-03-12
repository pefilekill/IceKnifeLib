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

    //插入
    void Insert(TVALUE tvPara) {
        pthread_mutex_lock(&m_mtxVector);
        auto atValue = tvPara;
        m_vecData.push_back(atValue);
        pthread_mutex_unlock(&m_mtxVector);
    }

    // 获取大小
    int GetSize() {
        return m_vecData.size();
    }

    //获取指定index的值
    TVALUE GetIndexAt(int iIndex) {
        pthread_mutex_lock(&m_mtxVector);
        auto atRet = m_vecData[iIndex];
        pthread_mutex_unlock(&m_mtxVector);
        return atRet;
    }


private:
    //
    pthread_mutex_t m_mtxVector;
    //值
    vector<TVALUE> m_vecData;
};


#endif //LOADER_CONCURRENTLIST_H
