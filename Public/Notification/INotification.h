//
// Created by wangwenzhe on 17/3/9.
//

#ifndef ICEKNIFELIB_INOTIFICATION_H
#define ICEKNIFELIB_INOTIFICATION_H

#include <string>
#include "PreDefine.h"

using namespace std;

class INotification {
public:
    //获取对象的消息key函数  对于单例的对象可定义为静态的以方便获取   对于非单例的不要定义为静态函数
    virtual string GetObserverKey() = 0;
    //响应消息函数  异步，不可执行ui线程内的操作 ncMsg 代表的是数据
    virtual void OnResponseNomalNotification(int iAction, PVOID ncMsg) = 0;
};


#endif //ICEKNIFELIB_INOTIFICATION_H
