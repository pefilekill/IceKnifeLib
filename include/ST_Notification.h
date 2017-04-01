//
// Created by ice on 17/3/10.
//

#ifndef ICEKNIFELIB_ST_NOTIFICATION_H
#define ICEKNIFELIB_ST_NOTIFICATION_H

#include "PreDefine.h"
#include <string>
using namespace std;

struct ST_Notification {
    string m_strRespObserverKey; //响应的观察者key
    PVOID m_pData; //  数据的指针


};



#endif //ICEKNIFELIB_ST_NOTIFICATION_H
