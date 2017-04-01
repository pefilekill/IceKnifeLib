#include "CPubFuncUtil.h"
#include "encode/md5/md5.h"
#include "Base64.h"
#include "SHA1.h"

#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <zconf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <vector>
#include <regex>
#include <iostream>
#include <codecvt>
#include "enumfile.h"

//#include "CHttp/CHttpMutiUtil.h"

#define  IDE_ATAPI_IDENTIFY                0xA1            //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY                0xEC            //  Returns ID sector for ATA.
#define  IOCTL_GET_DRIVE_INFO            0x0007c088
#define  IOCTL_GET_VERSION                0x00074080

#pragma warning(disable:4293)

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib, "wininet.lib")



//获取临时目录
string	CPubFunc::GetOurTempPath()
{
    char	szPath[MAX_PATH] = {0};
    GetTempPathA(MAX_PATH, szPath);
	char szLong[MAX_PATH] = {0};
    GetLongPathNameA(szPath, szLong, MAX_PATH);
    string strTemp = szLong;
    strTemp+="ice\\";
    CreateDirectoryA(strTemp.c_str(), NULL);
    return strTemp;
}

//获取程序目录
string CPubFunc::GetExeFolder() {
    string strExeDir = GetExePath();
    if (strExeDir.empty() == false) {
        strExeDir = strExeDir.substr(0, strExeDir.rfind("\\"));
    }
    return strExeDir;
}

//获取程序完整路径
string CPubFunc::GetExePath() {
    string strExePath = "";
    char exeFullPath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	char szLong[MAX_PATH] = {0};
    GetLongPathNameA(exeFullPath, szLong, MAX_PATH);
	strExePath = szLong;
    return strExePath;
}


//获取文件md5
string CPubFunc::GetFileMD5(string strFilePath) {
	BYTE lpMD5[256];
	memset(lpMD5, 0, 256);
	BOOL bResult = FALSE;
	HANDLE hFile = CreateFileA(strFilePath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		INVALID_HANDLE_VALUE
	);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		const DWORD dwBuffLen = 1024;
		DWORD dwReadBytes;
		BYTE* lpBuff = new BYTE[dwBuffLen];

		struct MD5Context ctx;
		MD5Init(&ctx);

		while (ReadFile(hFile, lpBuff, dwBuffLen, &dwReadBytes, NULL) && dwReadBytes != 0)
		{
			MD5Update(&ctx, (PBYTE)lpBuff, dwReadBytes);
		}

		MD5Final(lpMD5, &ctx);
		CloseHandle(hFile);

		delete[] lpBuff;
		lpBuff = NULL;

		bResult = TRUE;
	}

	string strMd5Ori = "";
	strMd5Ori.append((char *)lpMD5, 16);

	string strMd5Ret = str2HexStr(strMd5Ori);
	return strMd5Ret;
}

//获取缓冲区md5
string CPubFunc::GetBufferMD5(char *szDstBuf, int iBufLen) {
    unsigned char lpMD5[32];
    memset(lpMD5, 0, 32);

    struct MD5Context ctx;
    MD5Init(&ctx);

    MD5Update(&ctx, (const unsigned char *) szDstBuf /*lpBuff*/, iBufLen);

    MD5Final(lpMD5, &ctx);

    string strMd5Ori = "";
    strMd5Ori.append((char *) lpMD5, 16);

    string strMd5Ret = str2HexStr(strMd5Ori);
    return strMd5Ret;
}


bool CPubFunc::EnumCreateFolder(CString strDir)
{
	strDir.Replace(L"/", L"\\");
	int n = strDir.Find(L"\\");
	while (TRUE)
	{
		CString strFolder;
		if (n == -1)
		{
			break;
		}
		strFolder = strDir.Left(n);
		if (!PathFileExists(strFolder))
		{
			if (!CreateDirectory(strFolder, NULL) && strFolder.GetLength()>2)
			{
				return FALSE;
			}
		}
		n = strDir.Find(L"\\", n + 1);
	}
	return TRUE;
}

//获取时间的值
INT64 CPubFunc::currentTimeMillis() {
    INT64 i64Time = 0;
    _timeb timebuffer;
    _ftime_s( &timebuffer );
    i64Time = timebuffer.time;
    return i64Time;
}

//获取格式化的时间 2014-10-11 09:24:14
string CPubFunc::GetFormatDate() {
    string strTime = "";
#ifdef __OS_WINDOWS__
    SYSTEMTIME st = {0};
    GetLocalTime(&st);
    char szFmt[1024] = {0};
    sscanf(szFmt, "%d-%02d-%02d %02d:%02d:%02d",
                   st.wYear,
                   st.wMonth,
                   st.wDay,
                   st.wHour,
                   st.wMinute,
                   st.wSecond);
    strTime = szFmt;
#else
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char szFmt[1024] = {0};
    sprintf(szFmt, "%d-%02d-%02d %02d:%02d:%02d", (timeinfo->tm_year),
            timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    strTime = szFmt;
#endif
    return strTime;
}


//
DWORD CPubFunc::GetFileSize(string strFilePath) {
    unsigned long filesize = -1;
    struct stat statbuff;
    if (stat(strFilePath.c_str(), &statbuff) < 0) {
        return filesize;
    } else {
        filesize = statbuff.st_size;
    }
    return filesize;
//    DWORD dwRet = 0;
//	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hFile != INVALID_HANDLE_VALUE)
//	{
//		dwRet = ::GetFileSize(hFile, NULL);
//		CloseHandle(hFile);
//	}
//	return dwRet;
}

//
string CPubFunc::ReadFileText(string strFilePath) {
    string strContext = "";
    if (PathFileExistsA(strFilePath.c_str())) { //PathFileIsExist(strFilePath)
        //只读打开文件
		HANDLE hFile = CreateFileA(strFilePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD dwBufSize = GetFileSize(strFilePath);
            char *szBuf = new char[dwBufSize];
            memset(szBuf, 0, dwBufSize);
			DWORD dwRead = 0;
			ReadFile(hFile, szBuf, dwBufSize, &dwRead, NULL);
            //read返回0表示文件读取完毕
                    strContext.append(szBuf, dwRead);
            delete[] szBuf;
            //别忘记关闭
            CloseHandle(hFile);
        } 
    }
    return strContext;
}

//
void CPubFunc::PrintString(string strPara) {
	OutputDebugStringA(strPara.c_str());
}

//
string CPubFunc::Replace(string strOriData, string strKey, string strValue) {
    if (strOriData.find(strKey) == -1 || strKey == strValue || strKey.empty())
        return strOriData;

    string strRet = strOriData;
    int iPos = strRet.find(strKey);
    while (iPos != -1) {
        strRet = strRet.replace(iPos, strKey.length(), strValue);
        iPos = strRet.find(strKey, iPos + strValue.length());
    }
    return strRet;
}

//获取文件的sha1
string CPubFunc::GetFileSha1(string strFilePath) {
    CSHA1 sha;
	TCHAR *szFilePath = CA2W(strFilePath.c_str());
    bool bGetHash = sha.HashFile(szFilePath);
    sha.Final();

    if (bGetHash) {
        wstring wsHash = L"";
        sha.ReportHashStl(wsHash, CSHA1::REPORT_HEX_SHORT);
		string strRet = CW2A(wsHash.c_str());
        return strRet;
    }
    return "";
}

//换行分割
list<string> CPubFunc::SplitByNewLine(string strSrc) {
    list<string> listAll;
    string strTemp = Replace(strSrc, "\r\n", "\n");
    int iStartPos = 0;
    int iEndPos = strTemp.find("\n", 0);
    while (iEndPos != -1) {
        string strOne = strTemp.substr(iStartPos, iEndPos - iStartPos);
        listAll.push_back(strOne);
        iStartPos = iEndPos + 1;
        iEndPos = strTemp.find("\n", iStartPos);
    }
    return listAll;
}

//
string CPubFunc::GetUUID() {
    CString strUuid = "";
    UUID uuId;
    if (UuidCreate(&uuId) == RPC_S_OK )
    {
        RPC_WSTR pUuidString;
        if (UuidToString(&uuId, &pUuidString) == RPC_S_OK )
        {
            strUuid.Format(_T("%s"), pUuidString);
            RpcStringFree(&pUuidString);
        }
    }
	string strRst = CStringA(strUuid);
    return strRst;
}

//
int CPubFunc::GetRandValue(int iMaxValue) {
    static int iEnsure = rand();
    srand(time(NULL) + iEnsure);
    iEnsure = rand();
    return iEnsure;
}

//
string CPubFunc::GetRandString(int iLength) {
    string strRand = "";
    while (strRand.length() < iLength) {
        strRand += GetUUID();
    }
    strRand = strRand.substr(0, iLength);
    return strRand;
}


string CPubFunc::str2HexStr(string strSrc) {
    const char *szTable = "0123456789ABCDEF";
    string strRet = "";

//	string strTempSrc = CStringA(strSrc);
    int bit;

    for (int i = 0; i < (int) strSrc.length(); i++) {
        bit = (strSrc[i] & 0x0f0) >> 4;
        strRet.append(szTable + bit, 1);
        bit = strSrc[i] & 0x0f;
        strRet.append(szTable + bit, 1);
    }
    return strRet;
}

string CPubFunc::HexStr2Str(string strhex) {
    if (strhex.length() == 0) {
        return "";
    }
    int l = strhex.length();
    if (l % 2 == 1) {
        return "";
    }
    //byte[] b = new byte[l / 2];
    string strRet = "";
    for (int i = 0; i != l / 2; i++) {
        string strTempHex2 = strhex.substr(i * 2, 2);
        char szTemp = (char) strtol(strTempHex2.c_str(), NULL, 16);//atoi(strTempHex2.c_str());
        strRet.append(&szTemp, 1);
        //b[i] = (byte) Integer.parseInt(strhex.substring(i * 2, i * 2 + 2), 16);
    }
    return strRet;
}

string CPubFunc::HexStr2StrMem(string strHex) {
    string strRet = "";
    char *szTemp = new char[2];
    memset(szTemp, 0, 2);
    for (int iLoop = 0; iLoop < (int) strHex.length() / 2; iLoop++) {
        string strOne = strHex.substr(iLoop * 2, 2);
        int iTest = strtol(strOne.c_str(), NULL, 16);
        szTemp[0] = iTest;
        strRet.append(szTemp, 1);
    }
    delete[] szTemp;
    return strRet;
}


//
void CPubFunc::NewSleep(DWORD dwWaitTime) {
	HANDLE hEvent = CreateEventA(NULL, TRUE, FALSE, GetRandString(32).c_str());
	if (hEvent == NULL)
		Sleep(dwWaitTime);
	else
	{
		WaitForSingleObject(hEvent, dwWaitTime);
		CloseHandle(hEvent);
	}
    return;
}

//路径默认不是 ／ 结尾
list<string> CPubFunc::GetAllFiles(string strDirPara) {
    list<string> vecFileList;

	CString strDir = CStringA(strDirPara.c_str());
	strDir.Replace(L"/", L"\\");
	if (strDir.Right(1) == L"\\")
		strDir.Truncate(strDir.GetLength() - 1);

	CEnumFile enumer(strDir, L"*.*");
	for (int iIndex = 0; iIndex < enumer.GetFileCount(); iIndex++)
	{
		CString strFilePath = enumer.GetFileFullPath(iIndex);
		if (PathFileExists(strFilePath))
		{
			vecFileList.push_back(string(CStringA(strFilePath)));
		}
	}

	for (int iIndex = 0; iIndex < enumer.GetDirCount(); iIndex++)
	{
		CString strChildDirPath = enumer.GetDirFullPath(iIndex);
		list<string> listTemp = GetAllFiles(string(CStringA(strChildDirPath)));
		vecFileList.splice(vecFileList.end(), listTemp);
	}
    return vecFileList;
}

void CPubFunc::DeleteAllFiles(string strDir) {
    list<string> listAllFile = GetAllFiles(strDir);
    if (listAllFile.size() != 0) {
        for (list<string>::iterator itLoop = listAllFile.begin(); itLoop != listAllFile.end(); itLoop++) {
            string strOne = *itLoop;
            remove(strOne.c_str());
        }
    }

    return;
}

//
bool CPubFunc::WriteString2File(string strWite, string strFilePath) {
	if (strWite.length() == 0)
	{
		return false;
	}

	HANDLE hFile = CreateFileA(strFilePath.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	SetFilePointer(hFile, 0, 0, FILE_END);
	//	strWite += L"\r\n\r\n";
	DWORD dwWrite = 0;
	bool bRet = TRUE;
	if (!WriteFile(hFile, strWite.c_str(), strWite.length(), &dwWrite, NULL))
	{
		bRet = false;
	}
	CloseHandle(hFile);
	return bRet;
}

//
void CPubFunc::AppendBigendian(string &strSrc, DWORD dwValue) {
    char *szValue = (char *) &dwValue;
    strSrc.append(szValue + 3, 1);
    strSrc.append(szValue + 2, 1);
    strSrc.append(szValue + 1, 1);
    strSrc.append(szValue + 0, 1);
    return;
}

string CPubFunc::Int2LittleEndianHex(int iPara) {
    char szBuf[32] = {0};
    sprintf(szBuf, "%08x", iPara);
    string strRet = HexStr2StrMem(string(szBuf));
    return strRet;
}

//
string CPubFunc::Int2String(int iPara) {
    char szBuf[32] = {0};
    sprintf(szBuf, "%d", iPara);
    string strRet = szBuf;
    return strRet;
}

string CPubFunc::Int642String(INT64 iPara) {
    char szBuf[32] = {0};
    sprintf(szBuf, "%lld", iPara);
    string strRet = szBuf;
    return strRet;
}

//
int CPubFunc::String2Int(string strPara) {
    if (strPara.length() == 0)
        strPara = "0";

    int iRet = atoi(strPara.c_str());
    return iRet;
}

//
INT64 CPubFunc::String2Int64(string strPara) {
    if (strPara.length() == 0)
        strPara = "0";

    INT64 iRet = atoll(strPara.c_str());
    return iRet;
}


DWORD CPubFunc::__ROR4__(DWORD x, int n) {
//	DWORD dwRet = (x >> n) | ((x << sizeof(DWORD)*8) - n);
    DWORD dwRet = (x >> n) | ((x << 8) - n);
    return dwRet;
}

DWORD CPubFunc::ARM_NEGS(DWORD dwPara1) {
    DWORD dwRet = 0;
    //if (dwPara1 >= 0)
    {
        dwRet = ~dwPara1 + 1;
    }
    //else
    //	dwRet = ~dwPara1;
    return dwRet;
}

DWORD CPubFunc::ARM_SBCS(DWORD dwPara1, DWORD dwPara2, int iCValue) {
    DWORD dwResult = 0;
    //int iR1 = 0 - !((dwPara1>=dwPara2) ? 1 : 0);
    int iR1 = !iCValue;//~((dwPara1>=dwPara2) ? 1 : 0);
    dwResult = dwPara1 - dwPara2 - iR1;
    return dwResult;
}

//
int CPubFunc::Arm_CMP_C_Value(DWORD dwPara1, DWORD dwPara2) {
    if (dwPara1 >= dwPara2) {
        return 1;
    }
    return 0;
}

//
void CPubFunc::Arm_STR(byte *szBuf, DWORD dwInput) {
    memcpy(szBuf, &dwInput, 4);
}

//算术右移要处理符号位
long CPubFunc::ARM_ASRS(long dwPara1, DWORD dwOffset) {
    long dwRet = dwPara1 >> dwOffset;
    return dwRet;
}

DWORD CPubFunc::ARM_LSRS(DWORD dwPara1, DWORD dwOffset) {
    DWORD dwRet = dwPara1 >> dwOffset;
    return dwRet;
}

DWORD CPubFunc::ARM_lrand48() {
    DWORD dwRet = rand() * rand();  //十六进制从0x1 - 0xFF
    return dwRet;
}

//
DWORD CPubFunc::ARM_BICS(long dwPara1, DWORD dwPara2) {
    return dwPara1 & !dwPara2;
}

//
DWORD CPubFunc::ARM_idivmod(long dwPara1, DWORD dwPara2) {
    DWORD dwValue = dwPara1 % dwPara2;
    return dwValue;
}

long CPubFunc::ARM_lsls(long dwPara1, DWORD dwOffset) {
    long dwRet = dwPara1 << dwOffset;
    return dwRet;
}

long CPubFunc::ARM_EORS(long dwPara1, DWORD dwOffset) {
    long dwRet = dwPara1 ^dwOffset;
    return dwRet;
}

long CPubFunc::ARM_ORRS(long dwPara1, DWORD dwOffset) {
    long dwRet = dwPara1 | dwOffset;
    return dwRet;
}

//
string CPubFunc::FormatUrl(string strUrl) {
    strUrl = ReplaceOnce(strUrl, "http://", "");
    strUrl = ReplaceOnce(strUrl, "https://", "");
    if (strUrl.find("/") == -1) {
        strUrl += "/";
    }
    return strUrl;
}

//
string CPubFunc::GetHostFromeUrl(string strUrl) {
    string strFmtUrl = FormatUrl(strUrl);
    string strHost = strFmtUrl.substr(0, strFmtUrl.find("/"));
    return strHost;
}

//替换一次
string CPubFunc::ReplaceOnce(string strOriData, string strKey, string strValue) {
    if (strOriData.find(strKey) == -1 || strKey == strValue || strKey.empty())
        return strOriData;

    string strRet = strOriData;
    int iPos = strRet.find(strKey);
    if (iPos != -1) {
        strRet = strRet.replace(iPos, strKey.length(), strValue);
        iPos = strRet.find(strKey, iPos + strValue.length());
    }
    return strRet;
}

//
//
bool CPubFunc::ConnectServer(string strHost, int iPort, SOCKET & iOutSock) {
    string strIp = Convert2Ip(strHost);
    if (IsIPV4(strIp)) {
        iOutSock = socket(PF_INET, SOCK_STREAM, 0);
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(strIp.c_str());
        serv_addr.sin_port = htons(iPort);
        //SetSocketSendTimeOut(iOutSock, 2);
        if (connect(iOutSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            PrintString("连接失败-----0");
            return false;
        }
    } else if (IsIPV6(strIp)) {
		AF_INET
        iOutSock = socket(PF_INET6, SOCK_STREAM, 0);
        in6_addr addrV6;
        inet_pton(AF_INET6, strIp.c_str(), &addrV6);

        struct sockaddr_in6 serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin6_family = AF_INET6;
        serv_addr.sin6_addr = addrV6;
        serv_addr.sin6_port = htons(iPort);
        //设置超时时间
        //SetSocketSendTimeOut(iOutSock, 2);
        if (connect(iOutSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            PrintString("连接失败-----1");
            return false;
        }
    } else {
        PrintString("域名解析失败,ip不合法：" + strIp);
        return false;
    }

    return true;
}

//
void CPubFunc::SetSocketSendTimeOut(SOCKET sockPara, int iTimeOut) {
    iTimeOut *= 1000;
    if (-1 == setsockopt(sockPara, SOL_SOCKET, SO_SNDTIMEO, (char *) &iTimeOut, sizeof(int))) {
        CPubFunc::PrintString("设置发送超时时间失败:" + CPubFunc::Int2String(errno));
    }
}

//
string CPubFunc::Convert2Ip(string strPara) {
    return IsIPAddress(strPara) ? strPara : GetHostStringByName(strPara);
}

//判断是否是ip
bool CPubFunc::IsIPAddress(string strPara) {
    return IsIPV4(strPara) || IsIPV6(strPara);
}

bool CPubFunc::IsIPV4(string strIp) {
    return inet_addr(strIp.c_str()) != INADDR_NONE;
}

bool CPubFunc::IsIPV6(string strIp) {
    in6_addr addrV6;
    return inet_pton(AF_INET6, strIp.c_str(), &addrV6) > 0;
}

//根据域名获取IP stirng
string CPubFunc::GetHostStringByName(string strDomainName) {
    string strRet = "";
    struct addrinfo *answer, hint, *curr;
    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;
    //hint.ai_flags = AI_DEFAULT;  //IOS务必加   否则有可能出现v4和v6同时出现的情况  Android务必不加，否则有可能出现解析失败
    char ipstr2[128] = {0};
    struct sockaddr_in *sockaddr_ipv4;
    struct sockaddr_in6 *sockaddr_ipv6;

    int ret = getaddrinfo(strDomainName.c_str(), NULL, &hint, &answer);
    PrintString("getaddrinfo:" + strDomainName);
    if (ret != 0) {
        PrintString("getaddrinfo失败:" + Int2String(ret) + "    host:" + strDomainName);
        return "";
    }
//    EAI_SOCKTYPE
    bool bGetSuc = false;
    for (curr = answer; curr != NULL; curr = curr->ai_next) {
        switch (curr->ai_family) {
            case AF_UNSPEC:
                //do something here
                break;
            case AF_INET:
                sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>( curr->ai_addr);
                inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstr2, sizeof(ipstr2));
                strRet.append(ipstr2, strlen(ipstr2));
                bGetSuc = true;
                break;
            case AF_INET6:
                sockaddr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>( curr->ai_addr);
                inet_ntop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstr2, sizeof(ipstr2));
                strRet.append(ipstr2, 128);
                bGetSuc = true;
                break;
        }
        if (bGetSuc)
            break;
    }
    freeaddrinfo(answer);
    return strRet;
}

//
string CPubFunc::GetUriFromUrl(string strUrl) {
    string strFmtUrl = FormatUrl(strUrl);
    int iSlashPos = strFmtUrl.find("/");
    string strUri = strFmtUrl.substr(iSlashPos, strFmtUrl.length() - iSlashPos).c_str();
    return strUri;

}

string CPubFunc::GetHeader(string strFullData) {
    if (strFullData.find("\r\n\r\n") != -1) {
        string strHeader = strFullData.substr(0, strFullData.find("\r\n\r\n") + 4);
        return strHeader;
    }
    return "";
}

void CPubFunc::ClearResultHeader(string &strSrcData) {
    if (strSrcData.find("\r\n\r\n") == -1)
        return;

    ClearChunkedJunkData(strSrcData);

    int iHeaderEndPos = strSrcData.find("\r\n\r\n");

    if (iHeaderEndPos == -1)
        return;

    strSrcData = strSrcData.substr(iHeaderEndPos + 4, strSrcData.length() - iHeaderEndPos - 4);
    return;
}

int CPubFunc::ClearChunkedJunkData(string &strSrcData) {
    string strChunkKey = "Transfer-Encoding: chunked\r\n";

    if (strSrcData.find(strChunkKey) == -1 || strSrcData.find("\r\n\r\n") == -1)
        return -1;

    //抛弃第三方   转为java处理


    regex rxLen("\\x0d\\x0a([0-9A-Fa-f])+\\x0d\\x0a");
    string strFmt = "";

    string strHeader = strSrcData.substr(0, strSrcData.find("\r\n\r\n") + 4);
    string strBody = strSrcData.substr(strSrcData.find("\r\n\r\n") + 2,
                                       strSrcData.length() - strSrcData.find("\r\n\r\n") - 2);

    string strClearBody = regex_replace(strBody, rxLen, strFmt);
    if (strClearBody.substr(strClearBody.length() - 2, 2) ==
        "\r\n") {
        strClearBody = strClearBody.substr(0, strClearBody.length() - 2);
    }

    strSrcData = strHeader + strClearBody;//strHeader + string(szRepLenBuf);
    strSrcData.replace(strSrcData.find(strChunkKey), strChunkKey.length(), "");

    return strSrcData.length();
}

bool CPubFunc::IsContentEncodeGzip(string strHeader) {
    return strHeader.find("Content-Encoding: gzip") != -1;
}

//
int64_t CPubFunc::String2Long(string strPara) {
    //PrintString("应做内容检测");
    int64_t i64 = strtoll(strPara.c_str(), NULL, 10);
    return i64;
}

string CPubFunc::MakeLower(string strSrc) {
    transform(strSrc.begin(), strSrc.end(), strSrc.begin(), ::tolower);
    return strSrc;
}

string CPubFunc::MakeUpper(string strSrc) {
    transform(strSrc.begin(), strSrc.end(), strSrc.begin(), ::toupper);
    return strSrc;
}

//
string CPubFunc::GetRandNumber(int iNumLen) {
    static int iEnsure = rand();
    srand(time(NULL) + iEnsure);
    iEnsure = rand();
    string strRet = "";
    while (strRet.length() < iNumLen) {
        strRet += Int2String(rand());
    }
    strRet = strRet.substr(0, iNumLen);
    return strRet;
}

//参数  真正的utf8字符串
string CPubFunc::Utf82Unicode(string strUtf8) {
    string strUnicode = "";
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;
    u16string ucs2_cvt_str = utf8_ucs2_cvt.from_bytes(strUtf8); // utf-8 to ucs2
    char *szBuf = (char *) ucs2_cvt_str.c_str();
    int iLen = strlen(szBuf);
    strUnicode.append(szBuf, iLen);
    return strUnicode;
}

//参数 真正的unicode字符串
string CPubFunc::Unicode2Utf8(string strUnicode) {
    char16_t *szUnicode = (char16_t *) strUnicode.c_str();
    u16string u16Unicode = szUnicode;
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;
    string strUtf8 = utf8_ucs2_cvt.to_bytes(u16Unicode); // ucs2 -> utf8
    return strUtf8;
}

bool  CPubFunc::IsSocketClosed(int iSock) {
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

bool CPubFunc::CloseSocket(SOCKET &sock) {
    shutdown(sock, SHUT_RDWR);//SD_BOTH
    sock = 0;
    return false;
}

sem_t *CPubFunc::CreateSem(string strSemName, int iPermision) {
    // IOS上必须这种写法
    sem_t *pRet = sem_open(strSemName.c_str(),O_CREAT, iPermision, 1) ;
    return pRet;
}

sem_t *CPubFunc::CreateSem(int iPermision) {
    return CreateSem(GetRandString(32), iPermision);
}
sem_t *CPubFunc::CreateSem(string strSemName) {
    return CreateSem(strSemName, 0644);
}
sem_t *CPubFunc::CreateSem() {
    return CreateSem(GetRandString(32), 0644);
}

void CPubFunc::DeleteSem(sem_t *semPara, string strName) {
    sem_close(semPara);
    sem_unlink(strName.c_str());
}



CEvent::CEvent()
{
	m_hEvent = CreateEvent(NULL, TRUE, FALSE, CPubFunc::GetRandString(32).c_str());
}

CEvent::~CEvent()
{
	CloseHandle(m_hEvent);
}

void CEvent::Wait(DWORD dwWait)
{
	WaitForSingleObject(m_hEvent, dwWait);
}

void CEvent::Post()
{
	SetEvent(m_hEvent);
}
