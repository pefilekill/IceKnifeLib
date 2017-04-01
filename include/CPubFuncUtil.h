
//公共功能，皮肤、配置信息保存
//2014.5.12 add convert str 2 unicode encoding
#pragma once
#include <string>
#include <list>
#include <vector>
#include <Windows.h>
#include <atlstr.h>

using namespace std;






class CPubFunc {
public:

	//获取程序目录
	static std::string GetExeFolder();

	//获取程序完整路径
	static std::string GetExePath();

	static void PrintString(std::string strPara);

	//字符串替换
	static std::string Replace(std::string strOriData, std::string strKey, std::string strValue);

	//替换一次
	static std::string ReplaceOnce(std::string strOriData, std::string strKey, std::string strValue);

	//获取全部文件 dir路径默认不是 ／ 结尾
	static std::list<std::string> GetAllFiles(std::string strDir);

	//获取文件md5  大写
	static std::string GetFileMD5(std::string strFilePath);

	//获取缓冲区md5
	static std::string GetBufferMD5(char *szDstBuf, int dwBufLen);

	//获取临时目录
	static std::string GetOurTempPath();

	//嵌套创建文件夹
	static bool EnumCreateFolder(CString strDir);

	//
	static DWORD GetFileSize(std::string strFilePath);

	//获取sha1
	std::string GetFileSha1(std::string strFilePath);

	//获取文件的text内容
	static std::string ReadFileText(std::string strFlePath);

	static std::list<std::string> SplitByNewLine(std::string strRsc);

	//
	static std::string GetUUID();

	//生成指定长度的随机字符串
	static std::string GetRandString(int iLength);

	//生成随机数  参数为最大值
	static int GetRandValue(int iMaxValue = 0);

	//little endian 16
	static std::string Int2LittleEndianHex(int iPara);

	//
	static std::string Int2String(int iPara);

	//
	static std::string Int642String(int64_t iPara);


	//
	static int String2Int(std::string strPara);

	static int64_t String2Int64(std::string strPara);

	static std::string str2HexStr(std::string strSrc);

	static std::string HexStr2Str(std::string strhex);

	//将hex直接转换为内存
	static std::string HexStr2StrMem(std::string strHex);

	//将ip字符串转换为字节
	static std::string ConvertIpstr2IpByte(std::string strIp);

	//
	static bool IsPortOpend(std::string strIp, int iPort);

	static void DeleteAllFiles(std::string strDir);

	static bool WriteString2File(std::string strWite, std::string strFilePath);


	//等待
	static void NewSleep(DWORD i64WaitTime);

	//获取时间的值
	static int64_t currentTimeMillis();

	//获取格式化的时间 2014-10-11 09:24:14
	static std::string GetFormatDate();

	//
	static void AppendBigendian(std::string &strSrc, DWORD dwValue);

	//算术右移就是有符号位  逻辑右移就是直接右移

	//循环右移
	static DWORD __ROR4__(DWORD x, int n);

	static DWORD ARM_NEGS(DWORD dwPara1);

	static DWORD ARM_SBCS(DWORD dwPara1, DWORD dwPara2, int iCValue);

	static int Arm_CMP_C_Value(DWORD dwPara1, DWORD dwPara2);

	static void Arm_STR(byte *szBuf, DWORD dwInput);

	static long ARM_ASRS(long dwPara1, DWORD dwOffset);

	static DWORD ARM_LSRS(DWORD dwPara1, DWORD dwOffset);

	static DWORD ARM_lrand48();

	static DWORD ARM_BICS(long dwPara1, DWORD dwPara2);

	static DWORD ARM_idivmod(long dwPara1, DWORD dwPara2);

	static long ARM_lsls(long dwPara1, DWORD dwPara2);

	static long ARM_EORS(long dwPara1, DWORD dwPara2);

	static long ARM_ORRS(long dwPara1, DWORD dwPara2);


	// 去掉协议头  加上 /
	static std::string FormatUrl(std::string strUrl);

	//
	static std::string GetHostFromeUrl(std::string strUrl);

	//
	static bool ConnectServer(std::string strHost, int iPort, SOCKET & iOutSock);

	// 设置超时时间  单位秒
	static void SetSocketSendTimeOut(SOCKET sockPara, int iTimeOut);

	//
	static std::string Convert2Ip(std::string strPara);

	//
	static bool IsSocketClosed(int iSock);
	// 关闭socket
	static bool CloseSocket(SOCKET &sock);


	//判断是否是ip
	static bool IsIPAddress(std::string strPara);

	//判断ip格式是否是v4
	static bool IsIPV4(std::string strIp);

	//判断ip格式是否是v6
	static bool IsIPV6(std::string strIp);

	//根据域名获取IP stirng
	static std::string GetHostStringByName(std::string strDomainName);

	//
	static std::string GetUriFromUrl(std::string strUrl);

	//获取头部
	static std::string GetHeader(std::string strFullData);

	//清理头部
	static void ClearResultHeader(std::string &strSrcData);

	//清理chunk数据
	static int ClearChunkedJunkData(std::string &strSrcData);

	//判断是否gzip
	static bool IsContentEncodeGzip(std::string strHeader);

	//
	static int64_t String2Long(std::string strPara);
	//
	static std::string MakeLower(std::string strSrc);
	//
	static std::string MakeUpper(std::string strSrc);
	//
	static std::string GetRandNumber(int iNumLen);
	//参数  真正的utf8字符串
	static std::string Utf82Unicode(std::string strUtf8);
	//参数 真正的unicode字符串
	static std::string Unicode2Utf8(std::string strUnicode);
	
protected:
private:

};

class CEvent
{
public:
	CEvent();
	~CEvent();
	// 等待  默认无限等待
	void Wait(DWORD dwWait = INFINITE);
	// 通知
	void Post();

private:
	HANDLE m_hEvent;
};
