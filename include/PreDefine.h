//
// Created by ice on 17/3/1.
//

#ifndef ICEKNIFELIB_PREDEFINE_H
#define ICEKNIFELIB_PREDEFINE_H

#include "json.h"


// 路径的分隔符定义
#ifdef __WIN32
#define SEPARATOR "\\"
#define __OS_WINDOWS__
#else
#define SEPARATOR "/"

#include "LVcDefine.h"
#endif

typedef Json::Value     _JsonValue;
typedef Json::Reader    _JsonReader;
typedef Json::FastWriter     _JsonFastWriter;



#endif //ICEKNIFELIB_PREDEFINE_H
