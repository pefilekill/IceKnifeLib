
//公共功能，皮肤、配置信息保存
//2014.5.12 add convert str 2 unicode encoding
#pragma once

#include <string>
#include "PreDefine.h"
#include <sys/types.h>
#include <list>
#include <vector>

using namespace std;


enum enumSysName {
    NAME_XP = 1,   //1
    NAME_VISTA,  //2
    NAME_WIN7,   //3
    NAME_WIN8,   //后面开始定义基本用不到的
    NAME_WIN_SERVER_2012,
    NAME_WIN_NT4,
    NAME_WIN_95,
    NAME_WIN_98,
    NAME_WIN_ME,
    NAME_WIN_2000,
    NAME_WIN_SERVER_2003,
    NAME_WIN_SERVER_2003_R2,
    NAME_WIN_SERVER_2008,
    NAME_WIN_SERVER_2008_R2,
    NAME_WIN_UNKOWN,
};


class CPubFunc {
public:
    CPubFunc() {};

    //设置随机数种子
    static void DoSrand();


    //获取程序目录
    static string GetExeFolder();

    //获取程序完整路径
    static string GetExePath();

    static void PrintString(string strPara);

    //字符串替换
    static string Replace(string strOriData, string strKey, string strValue);

    //替换一次
    static string ReplaceOnce(string strOriData, string strKey, string strValue);

    //获取全部文件 dir路径默认不是 ／ 结尾
    static list<string> GetAllFiles(string strDir);

#ifdef __OS_WINDOWS__
    //获取CSIDL_COMMON_APPDATA目录
    static CString				GetCommAppDataPath();
    //获取最大盘符
    CString				GetMaxDriver();
    //获取存储目录
    CString				GetCacheDir();
//设置存储目录
    BOOL				SetCacheDir(CString strDirPath);
    //读取开机启动
    bool				GetBootStart();
    //设置开机启动
    BOOL				SetBootStart(bool bBootStart);
    //取系统位数 x86 x64
    static BOOL				IsWin64();
    //驱系统的版本
    static enumSysName			GetSystemName();
    //硬盘序列号
    static string			GetHdSerialNumber();
    //mac
    static CString					GetMacAddr();
    //pc name
    static CString					GetPcName();
    //cpuid
    static CString					GetCpuid();
    //换行分割
    static CSimpleArray<CString>	SplitByNewLine(CString strRsc);

    static CSimpleArray<CString>		SpliteByKey(CString strData, CString strKey);

    //开机启动
    static int				CreateRun(CString strSubKey, CString strExePath);
    //取消开机启动
    static int				DeleteRun(CString strSubKey);
//
    static string			CString2String(CString strSrc);

    static CString			ConvertStr2Utf8Hex(CString strInput);
//根据程序名获取进程的PID
    static bool				GetProcessIDWithName(CString strParaName, bool bCaseSensitive, CSimpleArray<DWORD> &arrayOut);
    //提权
    static bool				AdjustPurview();
    //
    static void				EnableDebugPriv() ;





#endif

    //获取文件md5  大写
    static string GetFileMD5(string strFilePath);

    //获取缓冲区md5
    static string GetBufferMD5(char *szDstBuf, int dwBufLen);

    //获取临时目录
    static string GetOurTempPath();

    //嵌套创建文件夹
    static BOOL EnumCreateFolder(string strDir);

    //
    static DWORD GetFileSize(string strFilePath);

    //获取sha1
    string GetFileSha1(string strFilePath);

    //获取文件的text内容
    static string ReadFileText(string strFlePath);

    static list<string> SplitByNewLine(string strRsc);

    //
    static string GetUUID();

    //生成指定长度的随机字符串
    static string GetRandString(int iLength);

    //生成随机数  参数为最大值
    static int GetRandValue(int iMaxValue = 0);

    //little endian 16
    static string Int2LittleEndianHex(int iPara);

    //
    static string Int2String(int iPara);

    //
    static string Int642String(INT64 iPara);


    //
    static int String2Int(string strPara);

    static INT64 String2Int64(string strPara);

    static string str2HexStr(string strSrc);

    static string HexStr2Str(string strhex);

    //将hex直接转换为内存
    static string HexStr2StrMem(string strHex);

    //将ip字符串转换为字节
    static string ConvertIpstr2IpByte(string strIp);

    //
    static bool IsPortOpend(string strIp, int iPort);

    static void DeleteAllFiles(string strDir);

    static bool WriteString2File(string strWite, string strFilePath);


    //等待
    static void NewSleep(int64_t i64WaitTime);

    //优雅关闭socket
    static void GracefulCloseSocket(int sockPara);

    //获取时间的值
    static INT64 currentTimeMillis();

    //获取格式化的时间 2014-10-11 09:24:14
    static string GetFormatDate();

    //
    static void AppendBigendian(string &strSrc, DWORD dwValue);

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
    static string FormatUrl(string strUrl);

    //
    static string GetHostFromeUrl(string strUrl);

    //
    static bool ConnectServer(string strHost, int iPort, int &iOutSock);

    //
    static string Convert2Ip(string strPara);

    //
    static bool IsSocketClosed(int iSock);

    //判断是否是ip
    static bool IsIPAddress(string strPara);

    //判断ip格式是否是v4
    static bool IsIPV4(string strIp);

    //判断ip格式是否是v6
    static bool IsIPV6(string strIp);

    //根据域名获取IP stirng
    static string GetHostStringByName(string strDomainName);

    //
    static string GetUriFromUrl(string strUrl);

    //获取头部
    static string GetHeader(string strFullData);

    //清理头部
    static void ClearResultHeader(string &strSrcData);

    //清理chunk数据
    static int ClearChunkedJunkData(string &strSrcData);

    //判断是否gzip
    static bool IsContentEncodeGzip(string strHeader);

    //
    static int64_t String2Long(string strPara);
    //
    static string MakeLower(string strSrc);
    //
    static string MakeUpper(string strSrc);
    //
    static string GetRandNumber(int iNumLen);

protected:
private:
    static bool m_bSrand;

};

