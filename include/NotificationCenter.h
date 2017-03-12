//
// Created by ice on 17/3/9.
//

#ifndef ICEKNIFELIB_NOTIFICATIONCENTER_H
#define ICEKNIFELIB_NOTIFICATIONCENTER_H

#include "ConcurrentMap.h"
#include <string>
#include "ST_Notification.h"
using namespace std;

template <typename TNotification>
class NotificationCenter {
public:
    //添加观察者
    static void AddObserver(string strKey, TNotification inPara) {
        m_mapTable.Insert(strKey, inPara);
    }

    //移除观察者
    static void RemoveObserver(string strKey) {
        m_mapTable.Remove(strKey);
    }

    // 发送消息 参数 pData: 如果观察者不存在则在这里就删除  如果存在  则在响应完之后删除
    template <typename TMsgData>
    static void PostMessage(string strObserverKey, TMsgData pData){
        //直接创建线程
        if (m_mapTable.IsExist(strObserverKey)){
            ST_Notification *pStNoti = new ST_Notification();
            pStNoti->m_strRespObserverKey = strObserverKey;
            pStNoti->m_pData = pData;

            pthread_t ptServer = 0;
            if (pthread_create(&ptServer, NULL, &NotificationCenter::MessageThread, pStNoti) != 0) {
                CPubFunc::PrintString("创建消息发送线程失败");
            }
        } else {
            if (pData != NULL) {
                delete pData;
            }
        }
    }
    //消息派发线程
    static void *MessageThread(void *pPara){
        if (pPara != NULL) {
            ST_Notification *stValue = (ST_Notification*)pPara;
            if (stValue->m_strRespObserverKey.empty() == false){
                auto atObserver = m_mapTable.GetValue(stValue->m_strRespObserverKey);
                atObserver->OnResponseNomalNotification(*stValue);
            }
            delete stValue;
        }
        return NULL;
    }


private:
    // 存储通知表
    static ConcurrentMap<string, TNotification> m_mapTable;

};
template <typename TNotification> ConcurrentMap<string, TNotification> NotificationCenter<TNotification>::m_mapTable;

#endif //ICEKNIFELIB_NOTIFICATIONCENTER_H
