//
// Created by ice on 16/7/13.
//

#include "CHttp.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include "CPubFuncUtil.h"
#include <map>
#include "CUtil.h"

CHttp::CHttp() {
    m_iSock = 0;
}


bool CHttp::CheckUrl(string strUrl) {
    //仅检测http://
    transform(strUrl.begin(), strUrl.end(), strUrl.begin(), ::tolower);
    if (strUrl.find("http://") == -1) {
        return false;
    }
    return true;
}

//非\r\n结尾
string CHttp::MakeHeaderContent(string strHost, string strExtraHeader, bool bKeepLive, bool bPost, bool bGzip) {
    map<string, string> mapNomal; //常规的必备头部
    //
    mapNomal.insert(make_pair("Host:", strHost));
    mapNomal.insert(make_pair("Connection:", bKeepLive ? ("keep-alive") : ("close")));
    mapNomal.insert(make_pair("Accept:", "*/*"));
    mapNomal.insert(make_pair("Accept-Encoding:", "gzip, deflate"));
    mapNomal.insert(make_pair("User-Agent:", "bricks Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36"));
    mapNomal.insert(make_pair("Cache-Control:", "no-cache"));
    mapNomal.insert(make_pair("Accept-Language:", "zh-CN,zh;q=0.8,en;q=0.6"));
    if (bGzip) {
        mapNomal.insert(make_pair("Content-Type:","application/x-www-form-urlencoded; charset=UTF-8"));
        mapNomal.insert(make_pair("Content-Encoding:","gzip"));
    } else {
        if (bPost) {
            mapNomal.insert(make_pair("Content-Type:","application/x-www-form-urlencoded; charset=UTF-8"));
        } else{
            mapNomal.insert(make_pair("Content-Type:", "text/plain;charset=UTF-8"));
        }
    }

    string strColon = ":";
    list<string> listExtra = CPubFunc::SplitByNewLine(strExtraHeader);
    for (list<string>::iterator itLoop = listExtra.begin(); itLoop != listExtra.end() ; ++itLoop) {
        string strLineOne = *itLoop;
        int iColonPos = strLineOne.find(strColon);
        if (iColonPos > 0) {
            string strKey = strLineOne.substr(0, iColonPos + 1);
            string strValue = strLineOne.substr(iColonPos +1, strLineOne.length() - iColonPos - 1);
            if (!strKey.empty() && !strValue.empty()) {
                if (mapNomal.find(strKey) == mapNomal.end()) {
                    mapNomal.insert(make_pair(strKey, strValue));
                } else {
                    mapNomal[strKey] = strValue;
                }
            }
        }
    }

    string strFullContent = "";
    for (map<string, string>::iterator itLoop = mapNomal.begin(); itLoop != mapNomal.end() ; ++itLoop) {
        if (strFullContent.empty()) {
            strFullContent = itLoop->first + " " + itLoop->second;
        } else {
            strFullContent += "\r\n" + itLoop->first + " "+ itLoop->second;
        }
    }

    return strFullContent;
}



//
bool CHttp::RequestGet(string strUrl, string strExtraHeader,string &strOut, bool bKeepLive) {
    if (!CheckUrl(strUrl))
        return false;

    string strHost = CPubFunc::GetHostFromeUrl(strUrl);
    string strPort = "80";
    if (strHost.find(":") != -1) {
        int iColonPos = strHost.find(":");
        strPort = strHost.substr(iColonPos + 1, strHost.length() - iColonPos - 1);
        strHost = strHost.substr(0, iColonPos);
    }
    if (!CPubFunc::ConnectServer(strHost, atoi(strPort.c_str()), m_iSock))
        return false;
    //处理header
    string strFullHeader = "GET " + CPubFunc::GetUriFromUrl(strUrl);//strUrlPath;
    strFullHeader += " HTTP/1.1\r\n";
    strFullHeader += MakeHeaderContent(strHost, strExtraHeader, bKeepLive, false, false) + "\r\n";
//    strFullHeader += "Host: " + strHost + "\r\n";
//    strFullHeader += bKeepLive ? ("Connection: keep-alive\r\n") : ("Connection: close\r\n");
//    strFullHeader += "Content-Type: text/plain;charset=UTF-8\r\n";
//    strFullHeader += "Accept: */*\r\n";
//    strFullHeader += "Accept-Encoding: gzip, deflate\r\n";
//    strFullHeader += "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36\r\n";
//    strFullHeader += "Cache-Control: no-cache\r\n";
//    strFullHeader += "Accept-Language: zh-CN,zh;q=0.8,en;q=0.6\r\n";

    strFullHeader += "\r\n";
    //CPublic::PrintString(CPublic::str2HexStr(strFullHeader).c_str());
    if (!Send(strFullHeader, strOut))
        return false;
    //取头部
    string strRespHeader = CPubFunc::GetHeader(strOut);
    if (strRespHeader.empty()) {
        return false;
    }
    //CPublic::PrintString("完整respHex:\n" + CPublic::str2HexStr(strOut));
    string strTempOut = strOut;
    CPubFunc::ClearResultHeader(strTempOut);

    if (CPubFunc::IsContentEncodeGzip(strRespHeader)) {
        //CPublic::PrintString("自动解压:hex:" + CPublic::str2HexStr(strTempOut));
        string strUnzip = "";
        if (CUtil::UncompressGzip(strTempOut, strUnzip)) {
            string strChunkKey = "Transfer-Encoding: chunked\r\n";
            if (strRespHeader.find(strChunkKey) != -1)
                strRespHeader.replace(strRespHeader.find(strChunkKey), strChunkKey.length(), "");
            string strGzipKey = "Content-Encoding: gzip\r\n";
            if (strRespHeader.find(strGzipKey) != -1)
                strRespHeader.replace(strRespHeader.find(strGzipKey), strGzipKey.length(), "");


            strOut = strRespHeader + strUnzip;
            //CPublic::PrintString("unzip:" + strUnzip);
            //CPublic::PrintString("NewFullBodyHex:" + CPublic::str2HexStr(strOut));
        }
    } else {
        // CPublic::PrintString("无需解压");
    }
    return true;
}
//
int CHttp::CalcContentLength(string strHeader) {
    int iLen = 0;
    if (strHeader.find(_HTTP_HEADER_END) != -1 && strHeader.find(_HTTP_HEADER_Content_Length) != -1) {
        int iPos1 = strHeader.find(_HTTP_HEADER_Content_Length);
        if (iPos1 != -1) {
            int iKeyContentLengthSize = strlen(_HTTP_HEADER_Content_Length);
            int iPos2 = strHeader.find("\r\n", iPos1 + iKeyContentLengthSize);
            if (iPos2 > iPos1) {
                string strConLen = strHeader.substr(iPos1 + iKeyContentLengthSize, iPos2 - iPos1 - iKeyContentLengthSize);
                strConLen = CPubFunc::Replace(strConLen, " ", "");
                iLen = CPubFunc::String2Long(strConLen);
            }
        }
    }
    return iLen;
}
//
bool CHttp::Send(string strData, string &strRecv) {
    //CPublic::PrintString("跟踪：发送的原始数据Hex：");
    //CPublic::PrintString(CPublic::str2HexStr(strData));
    const int iMaxRecvSize = 1024;

    bool bRet = false;
    if (send(m_iSock, strData.c_str(), strData.length(), 0) <= 0) {
        bRet = false;
    } else {
        char szBuf[iMaxRecvSize] = {0};
        int iRecvLen = 0;

        fd_set fdset;
        struct timeval delayval;
        int lRetVal;

        long lStartTime = time(NULL);
        int iFullContengLength = 0 , iRecvedDataLen = 0;
        string strTempHeader = "";

        while (true) {
            if (time(NULL) -lStartTime > 60) { // 超时
                break;
            }
            delayval.tv_sec = 1;
            delayval.tv_usec = 0;

            FD_ZERO(&fdset);
            FD_SET(m_iSock, &fdset);

            lRetVal = select(m_iSock + 1, &fdset, NULL, NULL, &delayval); // 尤其要注意第一个参数
            if (lRetVal == 0) {
                continue;
            }
            else if (lRetVal < 0) {
                break;
            }
            else if (FD_ISSET(m_iSock, &fdset) != 0) {
                iRecvLen = 0;
                memset(&szBuf, 0, iMaxRecvSize);
                iRecvLen = recv(m_iSock, szBuf, iMaxRecvSize - 1, 0);
                if (iRecvLen > 0) {
                    strRecv.append(szBuf, iRecvLen);

                    iRecvedDataLen += iRecvLen;
                    //判断长度
                    if ( iFullContengLength == 0) {
                        strTempHeader.append(szBuf, iRecvLen);
                        if (strTempHeader.find(_HTTP_HEADER_END) != -1) {
                            strTempHeader = strTempHeader.substr(0, strTempHeader.find(_HTTP_HEADER_END) + strlen(_HTTP_HEADER_END));
                            iFullContengLength = CalcContentLength(strTempHeader);
                        }
                    }
                    //  数据接收完整的判断    防止某些服务端sb,Close状态下仍socket不断开的情况
                    if (iFullContengLength <= iRecvedDataLen - strTempHeader.length() && iFullContengLength != 0) {
                        break;
                    }

                } else {
                    if (IsSocketClosed(m_iSock)) {
                        break;
                    }
                }
            }
        }
        bRet = true;
    }
    close(m_iSock);
    return bRet;
}

//
bool  CHttp::IsSocketClosed(int iSock) {
    //
    int iErr = 0;
    socklen_t len = sizeof(iErr);
    if (getsockopt(iSock, SOL_SOCKET, SO_ERROR, &iErr, &len) < 0) {
        return true;
    }
    //
    sockaddr_in addrMy;
    memset(&addrMy, 0, sizeof(addrMy));
    socklen_t leng = sizeof(addrMy);

    if (getsockname(iSock, (sockaddr *) &addrMy, &leng) != 0) {
        return true;
    }
    //
    char szPeek[2] = {0};
    if (recv(iSock, szPeek, 1, MSG_PEEK) == 0) {
        return true;
    }
    return false;
}

//--------------
bool CHttp::RequestPost(string strUrl, string strExtraHeader, string strData, string &strOut, bool bKeepLive,
                        bool bGzip) {
    if (!CheckUrl(strUrl))
        return false;

    string strHost = CPubFunc::GetHostFromeUrl(strUrl);
    string strPort = "80";
    if (strHost.find(":") != -1) {
        int iColonPos = strHost.find(":");
        strPort = strHost.substr(iColonPos + 1, strHost.length() - iColonPos - 1);
        strHost = strHost.substr(0, iColonPos);
    }
    if (!CPubFunc::ConnectServer(strHost, atoi(strPort.c_str()), m_iSock))
        return false;
    string strSendData = ""; //
    //处理header
    string strFullHeader = "POST " + CPubFunc::GetUriFromUrl(strUrl);//strUrlPath;
    strFullHeader += " HTTP/1.1\r\n";
    strFullHeader += MakeHeaderContent(strHost, strExtraHeader, bKeepLive, true, bGzip);
    if (bGzip) {
        CUtil::CompressGzip(strData, strSendData);
    } else {
        strSendData = strData;
    }
    strFullHeader += "\r\nContent-Length: " + CPubFunc::Int2String(strSendData.length()) + "\r\n";
    strFullHeader += "\r\n";

    string strFullBody = strFullHeader + strSendData;
    //CPublic::PrintString("跟踪FullBody:" + CPublic::str2HexStr(strFullBody));
    if (!Send(strFullBody, strOut))
        return false;
    //取头部
    string strRespHeader = CPubFunc::GetHeader(strOut);
    if (strRespHeader.empty()) {
        return false;
    }
    //CPublic::PrintString("完整respHex:\n" + CPublic::str2HexStr(strOut));
    string strTempOut = strOut;
    CPubFunc::ClearResultHeader(strTempOut);
    //CPublic::PrintString("清理之后respHex:\n" + CPublic::str2HexStr(strTempOut));

    if (CPubFunc::IsContentEncodeGzip(strRespHeader)) {
        //CPublic::PrintString("自动解压:hex:" + CPublic::str2HexStr(strTempOut));
        string strUnzip = "";
        if (CUtil::UncompressGzip(strTempOut, strUnzip)) {
            string strChunkKey = "Transfer-Encoding: chunked\r\n";
            if (strRespHeader.find(strChunkKey) != -1)
                strRespHeader.replace(strRespHeader.find(strChunkKey), strChunkKey.length(), "");
            string strGzipKey = "Content-Encoding: gzip\r\n";
            if (strRespHeader.find(strGzipKey) != -1)
                strRespHeader.replace(strRespHeader.find(strGzipKey), strGzipKey.length(), "");


            strOut = strRespHeader + strUnzip;
            //CPublic::PrintString("unzip:" + strUnzip);
            //CPublic::PrintString("NewFullBodyHex:" + CPublic::str2HexStr(strOut));
        }
    } else {
        // CPublic::PrintString("无需解压");
        strOut = strRespHeader + strTempOut;
    }
    return true;


}

