//
// Created by ice on 16/7/13.
// http请求
//

#ifndef IMSDK_CHTTP_H
#define IMSDK_CHTTP_H
#include <WinSock2.h>
#include <string>

using namespace std;

#define _HTTP_METHOD_GET "GET"
#define _HTTP_METHOD_POST "POST"
#define _HTTP_METHOD_CONNECT "CONNECT"
#define _HTTP_METHOD_OPTIONS "OPTIONS"
#define _HTTP_HEADER_END  "\r\n\r\n"
#define _HTTP_HEADER_Content_Length  "Content-Length:"
#define _HTTP_HEADER_Upgrade   "Upgrade:"


class CHttp {
public:
    CHttp();
    //检测url   必须http://开头
    static bool CheckUrl(string strUrl);

    //判断socket是否有效
    static bool  IsSocketClosed(int iSock);



    //拼装头部  非\r\n\r\n结尾  //非\r\n结尾
    static string MakeHeaderContent(string strHost, string strExtraHeader, bool bKeepLive, bool bPost, bool bGzip);
    // 计算头部声明的数据长度
    int CalcContentLength(string strHeader);

    //Get   cookie refer暂时不支持  自动解码gzip
    bool RequestGet(string strUrl, string strExtraHeader, string &strOut, bool bKeepLive);


    //Post
    bool RequestPost(string strUrl, string strExtraHeader, string strData, string &strOut, bool bKeepLive, bool bGzip);


private:
    //连接服务器
    bool ConnectServer(string strHost, int iPortint,int &iOutSock);
    //发送
    bool Send(string strData, string &strRecv);

    //socket
    SOCKET m_iSock;

};


#endif //IMSDK_CHTTP_H
