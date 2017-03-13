#include "encode/md5/md5.h"
#include "Base64.h"
#include "SHA1.h"
#include "CPubFuncUtil.h"
#include <time.h>
#include <sys/timeb.h>
#include <sys/socket.h>
#include <zconf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <uuid/uuid.h>
#include <pthread.h>
#include <vector>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex>
#include <iostream>
#include <codecvt>
//#include "CHttp/CHttpMutiUtil.h"

#define  IDE_ATAPI_IDENTIFY				0xA1			//  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY				0xEC			//  Returns ID sector for ATA.
#define  IOCTL_GET_DRIVE_INFO			0x0007c088
#define  IOCTL_GET_VERSION				0x00074080

#pragma warning(disable:4293)

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib, "wininet.lib")


pthread_mutex_t CPubFunc::m_mtxLog;

CPubFunc g_pfMtx;

#ifdef __OS_WINDOWS__
//
CString CPubFunc::GetCommAppDataPath()
{
	TCHAR szPath[MAX_PATH] = {0};
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_APPDATA, TRUE);
	// 获取exe名字
	CString strExeDir = GetExeFolder();
	CString strExePath = GetExePath();
	strExePath.Replace(strExeDir, L"");
	if (strExePath.Find(L".") > 0)
		strExePath.Truncate(strExePath.Find(L"."));

	CString strRet = szPath;
	strRet += L"\\" + strExePath + L"\\";
	CreateDirectory(strRet, NULL);
	return strRet;
}

//获取最大盘符
CString CPubFunc::GetMaxDriver()
{
    DWORD64 MaxSize = 0;
    CString MaxPar = L"";

    CString parID = L"";
    TCHAR a[256] = {0};
    DWORD len = GetLogicalDriveStrings(sizeof(a)/sizeof(TCHAR), a);
    for(TCHAR *s = a; *s; s += _tcslen(s) + 1)
    {
        if(GetDriveType(s) != 3)
            continue;

        parID = s;
        parID.TrimRight(L"\\");

        DWORD64 qwFreeBytesToCaller = 0;
        DWORD64 qwTotalBytes = 0;
        DWORD64 qwFreeBytes = 0;

        if(!GetDiskFreeSpaceEx( parID,
                                (PULARGE_INTEGER)&qwFreeBytesToCaller,
                                (PULARGE_INTEGER)&qwTotalBytes,
                                (PULARGE_INTEGER)&qwFreeBytes))
        {
            return L"";
        }

        if(qwFreeBytes > MaxSize)
        {
            MaxSize = qwFreeBytes;
            MaxPar = parID;
        }
    }
    return MaxPar;
}
//获取临时目录
CString	CPubFunc::GetOurTempPath()
{
    TCHAR	szPath[MAX_PATH] = {0};
    GetTempPath(MAX_PATH,szPath);
    TCHAR *szLong=new TCHAR[MAX_PATH];
    GetLongPathName(szPath,szLong,MAX_PATH);
    CString strTemp=szLong;
    strTemp+="ice\\";
    CreateDirectory(strTemp,NULL);
	delete[] szLong;
    return strTemp;
}
//取系统位数 x86 x64
BOOL CPubFunc::IsWin64()
{
    BOOL bRet = FALSE;
    static HMODULE hKernel32 = NULL;
    if( !hKernel32 )
        hKernel32 = ::LoadLibrary(TEXT("Kernel32.DLL"));
    if( !hKernel32 )
        return FALSE;

    typedef BOOL (WINAPI *FunctionIsWow64Process)(HANDLE hProcess,PBOOL Wow64Process);
    FunctionIsWow64Process pfnIsWow64Process = NULL;
    pfnIsWow64Process = (FunctionIsWow64Process)GetProcAddress(hKernel32, "IsWow64Process");

    if (NULL == pfnIsWow64Process)
        return FALSE;

    HANDLE hCurrentProcess = GetCurrentProcess();
    pfnIsWow64Process(hCurrentProcess, &bRet);
    return bRet;
}
//获取mac地址
CString	CPubFunc::GetMacAddr()
{
    CString strRstMacAddr = L"";
    char szTemp[MAX_PATH] = {0};

    PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    ZeroMemory(pAdapterInfo, ulOutBufLen);

    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
        ZeroMemory(pAdapterInfo, ulOutBufLen);
    }
    if(GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;

        while(pAdapter)
        {
            if (pAdapter->AddressLength > 0)
            {
                CStringA strDescript = pAdapter->Description;
                ZeroMemory(szTemp, MAX_PATH);
                sprintf_s(szTemp, MAX_PATH,
                          "%02X-%02X-%02X-%02X-%02X-%02X",
                          pAdapter->Address[0],
                          pAdapter->Address[1],
                          pAdapter->Address[2],
                          pAdapter->Address[3],
                          pAdapter->Address[4],
                          pAdapter->Address[5]);

                CStringA strMac = szTemp;
                strRstMacAddr = (CString)(CA2W(strMac,CP_ACP));
                break;
            }
            pAdapter = pAdapter->Next;
        }
    }
    if (pAdapterInfo) free(pAdapterInfo);
    return strRstMacAddr;
}
//获取计算机名
CString	CPubFunc::GetPcName()
{
    TCHAR nameTmp[512] = {0};
    DWORD sizeNum = 512;
    if(!GetComputerName(nameTmp, &sizeNum))
        return L"";
    else
        return nameTmp;
}
//cpuid
CString CPubFunc::GetCpuid()
{
	int CPUInfo[4] = {-1};
	__cpuid(CPUInfo, 1);

	CString strCpuId = L"";
	strCpuId.Format(L"%08X-%08X-%08X-%08X",CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
	return strCpuId;
}
//返回的是十六进制 第一硬盘
string CPubFunc::GetHdSerialNumber()
{
	string strHdInfo = "";
	HANDLE hDevice = CreateFileW(L"\\\\.\\PhysicalDrive0", 0,
								 FILE_SHARE_READ | FILE_SHARE_WRITE,
								 NULL, OPEN_EXISTING, 0,NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
		return strHdInfo;

	STORAGE_PROPERTY_QUERY   Query = {StorageDeviceProperty,PropertyStandardQuery, 0};
	char szDescript[1024] = {0};
	DWORD dwOutBufLen = 0;
	if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &Query, sizeof(STORAGE_PROPERTY_QUERY),
		&szDescript, 1024, &dwOutBufLen, (LPOVERLAPPED)NULL))
	{
		STORAGE_DEVICE_DESCRIPTOR *desp = (STORAGE_DEVICE_DESCRIPTOR*)szDescript;
		if (desp->SerialNumberOffset > 0)
		{
			char *szSerial = (char*)szDescript + desp->SerialNumberOffset;
			strHdInfo.append(szSerial, strlen(szSerial));
		}
	}
	return strHdInfo;
}
//换行分割
CSimpleArray<CString> CPubFunc::SplitByNewLine(CString strRsc)
{
    CSimpleArray<CString>  cRstArray;
    cRstArray.RemoveAll();

    WCHAR *szRsc=strRsc.GetBuffer();
    strRsc.ReleaseBuffer();

    CString strTemp=L"";
    WCHAR* szDelim=L"\r\n", *pszRst = NULL, *next_token = NULL;
    pszRst= wcstok_s(szRsc,szDelim,&next_token);
    strTemp=pszRst;
    strTemp.TrimLeft();
    strTemp.TrimRight();
    if (strTemp!="")
        cRstArray.Add(strTemp);
    while (pszRst)
    {
        pszRst=wcstok_s(NULL,szDelim,&next_token);
        if (pszRst)
        {
            strTemp=pszRst;
            strTemp.TrimLeft();
            strTemp.TrimRight();
            if (strTemp!="")
                cRstArray.Add(strTemp);
        }
        else
            break;
    }
    return cRstArray;
}
//
CSimpleArray<CString>	CPubFunc::SpliteByKey(CString strData, CString strKey)
{
	string strOriData = CStringA(strData);
	string strOriKey = CStringA(strKey);

	CSimpleArray<string>  arrayTemp =SpliteByKey(strOriData, strOriKey);

	CSimpleArray<CString> arrayRet;
	for (int iLoop = 0; iLoop < arrayTemp.GetSize(); iLoop ++)
	{
		string strOne = arrayTemp[iLoop];
		arrayRet.Add(CPubFunc::String2CString(strOne));
	}

	return arrayRet;
}
//
int  CPubFunc::CreateRun(CString strSubKey, CString strExePath)
{
	//添加以下代码
	HKEY   hKey;
	//找到系统的启动项
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	//打开启动项Key
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	if(lRet== ERROR_SUCCESS)
	{
		//添加注册
//		RegSetValue(hKey, strSubKey, REG_SZ, strExePath, MAX_PATH);
		string strExePathA = CStringA(strExePath);

		RegSetValueExA(hKey, CStringA(strSubKey), 0, REG_SZ, (const BYTE *)strExePathA.c_str(), strExePathA.length());
		RegCloseKey(hKey);
	}
	return 0;
}
//取消开机启动
int CPubFunc::DeleteRun(CString strSubKey)
{
    CString strExePath = GetExePath();
    TCHAR   szExePath[MAX_PATH] = {0};
    wcscpy_s(szExePath, MAX_PATH, strExePath);

    //添加以下代码
    HKEY   hKey;
    //找到系统的启动项
    LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    //打开启动项Key
    long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
    if(lRet== ERROR_SUCCESS)
    {
        //删除注册
        RegDeleteValue(hKey, strSubKey);
        RegCloseKey(hKey);
    }
    return 0;
}
//
CString CPubFunc::String2CString(string strSrc)
{
	return CString(CStringA(strSrc.c_str()));
}
//
string CPubFunc::CString2String(CString strSrc)
{
	string strRet = CStringA(strSrc);
	return strRet;
}
//
CString CPubFunc::ConvertStr2Utf8Hex(CString strInput)
{
	string strUTF = CW2A(strInput, CP_UTF8);

	string strHexRet = CPubFunc::str2HexStr(strUTF);

	CString strRet_MemMode = CString(CStringA(strHexRet.c_str()));
	return strRet_MemMode;
}
//驱系统的版本
enumSysName CPubFunc::GetSystemName()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
	if(::GetVersionExW((OSVERSIONINFO *)&os))
	{
		enumSysName vname;
		switch(os.dwMajorVersion)
		{
		case 4:
			switch(os.dwMinorVersion)
			{
			case 0:
				if(os.dwPlatformId==VER_PLATFORM_WIN32_NT)
					vname=NAME_WIN_NT4 ;//"Microsoft Windows NT 4.0";
				else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
					vname=NAME_WIN_95 ;//"Microsoft Windows 95";
				break;
			case 10:
				vname=NAME_WIN_98 ;//"Microsoft Windows 98";
				break;
			case 90:
				vname= NAME_WIN_ME;//"Microsoft Windows Me";
				break;
			}
			break;
		case 5:
			switch(os.dwMinorVersion)
			{
			case 0:
				vname=NAME_WIN_2000 ;//"Microsoft Windows 2000";
				break;
			case 1:
				vname=NAME_XP ;//"Microsoft Windows XP";
				break;
			case 2:
				if(os.wProductType==VER_NT_WORKSTATION &&
					info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)

					vname=NAME_XP ;//"Microsoft Windows XP Professional x64 Edition";
				else if(GetSystemMetrics(SM_SERVERR2)==0)
					vname=NAME_WIN_SERVER_2003 ;//"Microsoft Windows Server 2003";
				else if(GetSystemMetrics(SM_SERVERR2)!=0)
					vname=NAME_WIN_SERVER_2003_R2 ;//"Microsoft Windows Server 2003 R2";
				break;
			}
			break;
		case 6:
			switch(os.dwMinorVersion)
			{
			case 0:
				if(os.wProductType==VER_NT_WORKSTATION)
					vname= NAME_VISTA;//"Microsoft Windows Vista";
				else
					vname= NAME_WIN_SERVER_2008;//"Microsoft Windows Server 2008";
				break;
			case 1:
				if(os.wProductType==VER_NT_WORKSTATION)
					vname=NAME_WIN7 ;//"Microsoft Windows 7";
				else
					vname=NAME_WIN_SERVER_2008_R2 ;// "Microsoft Windows Server 2008 R2";
				break;
			case 2:
				if (os.wProductType==VER_NT_WORKSTATION)
					vname=NAME_WIN8;
				else
					vname=NAME_WIN_SERVER_2012;
			}
			break;
		default:
			vname=NAME_WIN_UNKOWN ;//"未知操作系统";
		}
		return vname;
	}
	else
		return NAME_WIN_UNKOWN;
}
//根据程序名获取进程的PID
bool CPubFunc::GetProcessIDWithName(CString strParaName, bool bCaseSensitive, CSimpleArray<DWORD> &arrayOut)
{
	//获取系统快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return false;

	//创建单个进程快照结构体，初始化大小
	PROCESSENTRY32 pe32 = {0};
	pe32.dwSize = sizeof(PROCESSENTRY32);  //务必提前初始化，否则默认的大小不一定满足要求
	//枚举系统快照链表中的第一个进程项目
	BOOL bProcess = Process32First(hProcessSnap, &pe32);
	while (bProcess)
	{
		CString strExeName = pe32.szExeFile;
		if ((bCaseSensitive ? strExeName.Compare(strParaName) : strExeName.CompareNoCase(strParaName)) == 0)
			arrayOut.Add(pe32.th32ProcessID);
		//继续枚举下一个进程
		bProcess = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);
	return true;
}
//提权
bool CPubFunc::AdjustPurview()
{
	TOKEN_PRIVILEGES TokenPrivileges;
	bool bRet;
	HANDLE hToken;

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &TokenPrivileges.Privileges[0].Luid);
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES| TOKEN_QUERY, &hToken);

	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	bRet = AdjustTokenPrivileges(hToken, false, &TokenPrivileges, 0, NULL, NULL) ? true: false;

	CloseHandle(hToken);
	return bRet ;
}
//
void CPubFunc::EnableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if ( ! OpenProcessToken( GetCurrentProcess(),TOKEN_ALL_ACCESS, &hToken ) )
	{
		printf("提权失败。\r\n");
		return;
	}

	if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		printf("提权失败。\r\n");
		return;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof (tkp), NULL, NULL ) )
	{
		printf("提权失败。\r\n");
		//CloseHandle( hToken );
	}
	else
		printf("提权成功！\r\n");
	CloseHandle( hToken );
	return ;
}


#endif   //windows代码结束



//获取程序目录
string	CPubFunc::GetExeFolder()
{
    string strExeDir = GetExePath();
    if (strExeDir.empty() == false)
    {
        strExeDir = strExeDir.substr(0, strExeDir.rfind(SEPARATOR));
    }
    return strExeDir;
}
//获取程序完整路径
string	CPubFunc::GetExePath()
{
    string strExePath = "";
#ifdef __OS_WINDOWS__
    TCHAR exeFullPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, exeFullPath, MAX_PATH);
    TCHAR szLong[MAX_PATH] = {0};
    GetLongPathName(exeFullPath, szLong, MAX_PATH);
    CString strExeDir = L"";
    strExeDir = szLong;
    strExeDir = strExeDir.Left(strExeDir.ReverseFind('\\')+1);
#else
    char szParentDir[_MAX_PATH] = {0};
    if (getcwd(szParentDir, _MAX_PATH) != NULL)
    {
        string strParentDir = szParentDir;
        char szPs[_MAX_PATH] = {0};
        string strPid = Int2String(getpid());
        sprintf(szPs, "ps aux | grep %s", strPid.c_str());
        FILE *fp = popen(szPs,"r");
        if (fp != NULL)
        {
            char szBuf[10240] = {0};
            while (NULL != fgets(szBuf, 10240 -1, fp))
            {
                if (strstr(szBuf, szParentDir) != NULL && strstr(szBuf, strPid.c_str()) != NULL)
                {
                    string strFull = szBuf;
                    int iStart = strFull.find(strParentDir);
                    int iPathLen = strlen(szBuf) - iStart ;
                    strExePath = strFull.substr(iStart , iPathLen);
                    strExePath = Replace(strExePath, "\n", "");
                    strExePath = Replace(strExePath, "\r", "");
                    break;
                }
                memset(szBuf, 0, 10240);
            }
            pclose(fp);
        }
    }



#endif
    return strExePath;
}


//获取文件md5
string CPubFunc::GetFileMD5(string strFilePath)
{
    BYTE lpMD5[256];
    memset(lpMD5,0,256);
    HANDLE hFile = open(strFilePath.c_str(), O_RDONLY);
    if ( hFile != INVALID_HANDLE_VALUE )
    {
        const DWORD dwBuffLen = 1024;
        DWORD dwReadBytes;
        BYTE* lpBuff = new BYTE[dwBuffLen];
        memset(lpBuff, 0, dwBuffLen);

        struct MD5Context ctx;
        MD5Init(&ctx);

        while(read(hFile,lpBuff,dwBuffLen - 1) > 0 )
        {
            MD5Update(&ctx, (PBYTE)lpBuff, dwReadBytes);
            memset(lpBuff, 0, dwBuffLen);
        }
        MD5Final(lpMD5, &ctx);
        close(hFile);

        delete[] lpBuff;
        lpBuff = NULL;
    }

    string strMd5Ori = "";
    strMd5Ori.append((char*)lpMD5, 16);

    string strMd5Ret = str2HexStr(strMd5Ori);
    return strMd5Ret;
}
//获取缓冲区md5
string CPubFunc::GetBufferMD5(char* szDstBuf, int iBufLen)
{
    unsigned char lpMD5[32];
    memset(lpMD5,0,32);

    struct MD5Context ctx;
    MD5Init(&ctx);

    MD5Update(&ctx, (const unsigned char*)szDstBuf /*lpBuff*/, iBufLen);

    MD5Final(lpMD5, &ctx);

    string strMd5Ori = "";
    strMd5Ori.append((char*)lpMD5, 16);

    string strMd5Ret = str2HexStr(strMd5Ori);
    return strMd5Ret;
}



BOOL CPubFunc::EnumCreateFolder(string strDir)
{
#ifdef __OS_WINDOWS__
    strDir.Replace(L"/",L"\\");
    int n=strDir.Find(L"\\");
    while (TRUE)
    {
        CString strFolder;
        if (n==-1)
        {
            break;
        }
        strFolder=strDir.Left(n);
        if (!PathFileExists(strFolder))
        {
            if (!CreateDirectory(strFolder,NULL) && strFolder.GetLength()>2)
            {
                return FALSE;
            }
        }
        n=strDir.Find(L"\\",n+1);
    }
    return TRUE;
#else
    mkdir(strDir.c_str(), 0755);
    return TRUE;
#endif
}

//获取时间的值
INT64 CPubFunc::currentTimeMillis()
{
    INT64  i64Time = 0;
#ifdef __OS_WINDOWS__
    _timeb timebuffer;
	_ftime_s( &timebuffer );
	i64Time = timebuffer.time;
#else
    struct timeval tv;
    gettimeofday(&tv,NULL);

    i64Time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
    return i64Time;
}

//获取格式化的时间 2014-10-11 09:24:14
string CPubFunc::GetFormatDate()
{
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
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char szFmt[1024] = {0};
    sprintf(szFmt,"%d-%02d-%02d %02d:%02d:%02d", (timeinfo->tm_year),
           timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    strTime = szFmt;
#endif
	return strTime;
}



//
DWORD CPubFunc::GetFileSize(string strFilePath)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(strFilePath.c_str(), &statbuff) < 0){
        return filesize;
    }else{
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
    if ( access(strFilePath.c_str(), F_OK) == 0) { //PathFileIsExist(strFilePath)
        //只读打开文件
        int fd = open( strFilePath.c_str(), O_RDONLY);
        if(fd != -1) {
            int iBufSize = 512;
            char *szBuf = new char[iBufSize];
            memset(szBuf, 0, iBufSize);
            //read返回0表示文件读取完毕
            int iRead = 0;
            while (true) {
                iRead = read(fd, szBuf, iBufSize - 1);
                if (iRead > 0) {
                    strContext.append(szBuf, iRead);
                    memset(szBuf, 0, iRead);
                } else {
                    break;
                }
            }
            delete[] szBuf;
            //别忘记关闭
            close(fd);
        } else {
            PrintString("读取文件失败" + Int2String(errno));
            PrintString(strerror(errno));
        }
    }
    return strContext;
}

//
void  CPubFunc::PrintString(string strPara) {
    pthread_mutex_lock(&m_mtxLog);
#ifdef __OS_WINDOWS__

#else
    std::cout << strPara << std::endl;
#endif
    pthread_mutex_unlock(&m_mtxLog);
}
//
string CPubFunc::Replace(string strOriData, string strKey, string strValue) {
    if (strOriData.find(strKey) == -1 || strKey == strValue || strKey.empty() )
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
string CPubFunc::GetFileSha1(string strFilePath)
{
    CSHA1 sha;
    bool bGetHash = sha.HashFile(strFilePath.c_str());
    sha.Final();

    if (bGetHash)
    {
        string wsHash = "";
        sha.ReportHashStl(wsHash, CSHA1::REPORT_HEX_SHORT);
        return wsHash;
    }
    return "";
}

//换行分割
list<string> CPubFunc::SplitByNewLine(string strSrc)
{
	list<string>  listAll;
    string strTemp = Replace(strSrc, "\r\n", "\n");
    int iStartPos = 0;
    int iEndPos = strTemp.find("\n", 0);
    while (iEndPos != -1)
    {
        string strOne = strTemp.substr(iStartPos, iEndPos - iStartPos);
        listAll.push_back(strOne);
        iStartPos = iEndPos + 1;
        iEndPos = strTemp.find("\n", iStartPos);
    }
	return listAll;
}

//
string CPubFunc::GetUUID()
{
	string strUuid = "";
#ifdef __OS_WINDOWS__
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
#else
    uuid_t uu;
    uuid_generate( uu );
    char szOne[4] = {0};
    for(int iLoop = 0; iLoop < 16; iLoop++)
    {
        sprintf(szOne, "%02X", uu[iLoop]);
        strUuid.append(szOne, 2);
    }
#endif
	return strUuid;
}
//
int CPubFunc::GetRandValue(int iMaxValue)
{
    static int iEnsure = rand();
    srand(time(NULL) + iEnsure);
    iEnsure = rand();
	return iEnsure;
}

//
string CPubFunc::GetRandString(int iLength)
{
    string strRand = "";
    while(strRand.length() < iLength)
    {
        strRand += GetUUID();
    }
    strRand = strRand.substr(0, iLength);
	return strRand;
}



string CPubFunc::str2HexStr(string strSrc)
{
    const char  *szTable = "0123456789ABCDEF";
	string strRet = "";

//	string strTempSrc = CStringA(strSrc);
	int bit;

	for (int i = 0; i < (int)strSrc.length(); i++)
	{
		bit = (strSrc[i] & 0x0f0) >> 4;
		strRet.append(szTable + bit,1);
		bit = strSrc[i] & 0x0f;
		strRet.append(szTable + bit,1);
	}
	return strRet;
}

string CPubFunc::HexStr2Str(string strhex)
{
	if (strhex.length() == 0)
	{
		return "";
	}
	int l = strhex.length();
	if (l % 2 == 1)
	{
		return "";
	}
	//byte[] b = new byte[l / 2];
	string strRet = "";
	for (int i = 0; i != l / 2; i++)
	{
		string strTempHex2 = strhex.substr(i * 2,  2);
		char szTemp = (char)strtol(strTempHex2.c_str(), NULL, 16);//atoi(strTempHex2.c_str());
		strRet.append(&szTemp,1);
		//b[i] = (byte) Integer.parseInt(strhex.substring(i * 2, i * 2 + 2), 16);
	}
	return strRet;
}

string CPubFunc::HexStr2StrMem(string strHex)
{
	string strRet = "";
	char *szTemp = new char[2];
	memset(szTemp, 0, 2);
	for (int iLoop = 0; iLoop < (int)strHex.length()/2; iLoop++)
	{
		string strOne = strHex.substr(iLoop *2, 2);
		int iTest = strtol(strOne.c_str(), NULL, 16);
		szTemp[0] = iTest;
		strRet.append(szTemp, 1);
	}
	delete[] szTemp;
	return strRet;
}


//
void CPubFunc::NewSleep(int64_t i64WaitTime)
{
    pthread_mutex_t mtWait;
    pthread_cond_t cdWait;

    if (pthread_mutex_init(&mtWait, NULL)) {
        return;
    }
    if (pthread_cond_init(&cdWait, NULL)) {
        pthread_mutex_destroy(&mtWait);
        return;
    }

    if (pthread_mutex_lock(&mtWait) != 0) {
        pthread_mutex_destroy(&mtWait);
        return;
    }

    struct timespec abstime;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    abstime.tv_sec = tv.tv_sec + i64WaitTime / 1000;
    abstime.tv_nsec = tv.tv_usec * 1000 + (i64WaitTime % 1000) * 1000000;
    if (abstime.tv_nsec >= 1000000000) {
        abstime.tv_nsec -= 1000000000;
        abstime.tv_sec++;
    }

    pthread_cond_timedwait(&cdWait, &mtWait, &abstime);
    pthread_mutex_unlock(&mtWait);
    pthread_mutex_destroy(&mtWait);
    return;
}

//路径默认不是 ／ 结尾
list<string> CPubFunc::GetAllFiles(string strDir) {
    list<string> vecFileList;

    if (strDir.empty())
        return vecFileList;
    if (strDir[strDir.length() - 1 ] != SEPARATOR[0])
        strDir += SEPARATOR;

    DIR *dir = NULL;
    struct dirent *ptr = NULL;

    if ((dir = opendir(strDir.c_str())) == NULL)
    {
        return vecFileList;
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if(strcmp( ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 || ptr->d_type == 10)    ///current dir OR parrent dir OR link
            continue;
        else if(ptr->d_type == 8) {    ///file
            string strOnePath = strDir + "/" + ptr->d_name;
            vecFileList.push_back(strOnePath);
        }
        else if(ptr->d_type == 4) {   ///dir
            string strChildDir = strDir + "/" + ptr->d_name;
            list<string> vecChild = GetAllFiles(strChildDir);
            vecFileList.splice(vecFileList.end(), vecChild);
        }
    }
    closedir(dir);
    return vecFileList;
}

void CPubFunc::DeleteAllFiles(string strDir)
{
    list<string> listAllFile = GetAllFiles(strDir);
    if (listAllFile.size() != 0)
    {
        for (list<string>::iterator itLoop = listAllFile.begin(); itLoop != listAllFile.end() ; itLoop ++) {
            string strOne = *itLoop;
            remove(strOne.c_str());
        }
    }

	return ;
}
//
bool CPubFunc::WriteString2File(string strWite, string strFilePath)
{
    string strContext = "";
    if ( access(strFilePath.c_str(), F_OK) == 0) { //PathFileIsExist(strFilePath)
        //只读打开文件
        int fd = open( strFilePath.c_str(), O_WRONLY | O_CREAT, 0777);
        if(fd != -1) {
            write(fd, strWite.c_str(), strWite.length());
            //别忘记关闭
            close(fd);
            return true;
        } else {
            PrintString("读取文件失败" + Int2String(errno));
            PrintString(strerror(errno));
        }
    }
	return false;
}
//
void CPubFunc::AppendBigendian(string &strSrc, DWORD dwValue)
{
	char *szValue= (char*) &dwValue;
	strSrc.append(szValue + 3, 1);
	strSrc.append(szValue + 2, 1);
	strSrc.append(szValue + 1, 1);
	strSrc.append(szValue + 0, 1);
	return;
}
string CPubFunc::Int2LittleEndianHex(int iPara)
{
    char szBuf[32] = {0};
    sprintf(szBuf, "%08x", iPara);
	string strRet = HexStr2StrMem(string(szBuf));
	return strRet;
}

//
string CPubFunc::Int2String(int iPara)
{
    char szBuf[32] = {0};
    sprintf(szBuf, "%d", iPara);
    string strRet = szBuf;
    return strRet;
}
string CPubFunc::Int642String(INT64 iPara)
{
    char szBuf[32] = {0};
    sprintf(szBuf, "%lld", iPara);
	string strRet = szBuf;
	return strRet;
}

//
int CPubFunc::String2Int(string strPara)
{
	if (strPara.length() == 0)
		strPara = "0";

	int iRet = atoi(strPara.c_str());
	return iRet;
}
//
INT64 CPubFunc::String2Int64(string strPara)
{
	if (strPara.length() == 0)
		strPara = "0";

	INT64 iRet = atoll(strPara.c_str());
	return iRet;
}



DWORD CPubFunc::__ROR4__(DWORD x,  int n)
{
//	DWORD dwRet = (x >> n) | ((x << sizeof(DWORD)*8) - n);
    DWORD dwRet = (x >> n) | ((x << 8) - n);
	return dwRet;
}

DWORD CPubFunc::ARM_NEGS(DWORD dwPara1)
{
	DWORD dwRet = 0;
	//if (dwPara1 >= 0)
	{
		dwRet = ~dwPara1 + 1;
	}
	//else
	//	dwRet = ~dwPara1;
	return dwRet;
}
DWORD CPubFunc::ARM_SBCS(DWORD dwPara1, DWORD dwPara2, int iCValue)
{
	DWORD dwResult = 0;
	//int iR1 = 0 - !((dwPara1>=dwPara2) ? 1 : 0);
	int iR1 = !iCValue;//~((dwPara1>=dwPara2) ? 1 : 0);
	dwResult = dwPara1 - dwPara2 - iR1;
	return  dwResult;
}
//
int CPubFunc::Arm_CMP_C_Value(DWORD dwPara1, DWORD dwPara2)
{
	if (dwPara1 >= dwPara2)
	{
		return 1;
	}
	return 0;
}
//
void CPubFunc::Arm_STR(byte *szBuf, DWORD dwInput)
{
    memcpy(szBuf, &dwInput, 4);
}
//算术右移要处理符号位
long CPubFunc::ARM_ASRS(long dwPara1, DWORD dwOffset)
{
	long dwRet = dwPara1 >> dwOffset;
	return dwRet;
}
DWORD CPubFunc::ARM_LSRS(DWORD dwPara1, DWORD dwOffset)
{
	DWORD dwRet = dwPara1 >> dwOffset;
	return dwRet;
}
DWORD CPubFunc::ARM_lrand48()
{
	DWORD dwRet = rand() * rand();  //十六进制从0x1 - 0xFF
	return dwRet;
}
//
DWORD CPubFunc::ARM_BICS(long dwPara1, DWORD dwPara2)
{
	return dwPara1 & !dwPara2;
}
//
DWORD CPubFunc::ARM_idivmod(long dwPara1, DWORD dwPara2)
{
	DWORD dwValue = dwPara1 % dwPara2;
	return dwValue;
}
long CPubFunc::ARM_lsls(long dwPara1, DWORD dwOffset)
{
	long dwRet = dwPara1 << dwOffset;
	return dwRet;
}
long CPubFunc::ARM_EORS(long dwPara1, DWORD dwOffset)
{
	long dwRet = dwPara1 ^ dwOffset;
	return dwRet;
}
long CPubFunc::ARM_ORRS(long dwPara1, DWORD dwOffset)
{
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
    if (strOriData.find(strKey) == -1 || strKey == strValue || strKey.empty() )
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
bool CPubFunc::ConnectServer(string strHost, int iPort, int &iOutSock) {
    string strIp = Convert2Ip(strHost);
    if (IsIPV4(strIp)) {
        iOutSock = socket(PF_INET, SOCK_STREAM, 0);
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(strIp.c_str());
        serv_addr.sin_port = htons(iPort);
        if (connect(iOutSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            PrintString("连接失败-----0");
            return false;
        }
    } else if (IsIPV6(strIp)) {
        iOutSock = socket(PF_INET6, SOCK_STREAM, 0);
        in6_addr addrV6;
        inet_pton(AF_INET6, strIp.c_str(),&addrV6);

        struct sockaddr_in6 serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin6_family = AF_INET6;
        serv_addr.sin6_addr = addrV6;
        serv_addr.sin6_port = htons(iPort);
        if (connect(iOutSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            PrintString("连接失败-----1");
            return false;
        }
    } else {
        PrintString("域名解析失败,ip不合法："+ strIp);
        return false;
    }

    return true;
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
    return inet_pton(AF_INET6, strIp.c_str(),&addrV6) > 0;
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
    struct sockaddr_in  *sockaddr_ipv4;
    struct sockaddr_in6 *sockaddr_ipv6;

    int ret = getaddrinfo(strDomainName.c_str(), NULL,&hint, &answer);
    PrintString("getaddrinfo:" + strDomainName);
    if (ret != 0) {
        PrintString("getaddrinfo失败:" + Int2String(ret) + "    host:" + strDomainName);
        return "";
    }
//    EAI_SOCKTYPE
    bool bGetSuc = false;
    for (curr = answer; curr != NULL; curr = curr->ai_next) {
        switch (curr->ai_family){
            case AF_UNSPEC:
                //do something here
                break;
            case AF_INET:
                sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>( curr->ai_addr);
                inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstr2,sizeof(ipstr2));
                strRet.append(ipstr2, strlen(ipstr2));
                bGetSuc = true;
                break;
            case AF_INET6:
                sockaddr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>( curr->ai_addr);
                inet_ntop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstr2,sizeof(ipstr2));
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
string CPubFunc::GetRandNumber(int iNumLen)
{
    static int iEnsure = rand();
    srand(time(NULL) + iEnsure);
    iEnsure = rand();
    string strRet = "";
    while(strRet.length() < iNumLen) {
        strRet += Int2String(rand());
    }
    strRet = strRet.substr(0, iNumLen);
    return strRet;
}
//参数  真正的utf8字符串
string CPubFunc::Utf82Unicode(string strUtf8)
{
    string strUnicode = "";
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;
    u16string ucs2_cvt_str = utf8_ucs2_cvt.from_bytes(strUtf8); // utf-8 to ucs2
    char *szBuf = (char*)ucs2_cvt_str.c_str();
    int iLen = strlen(szBuf);
    strUnicode.append(szBuf, iLen);
    return strUnicode;
}
//参数 真正的unicode字符串
string CPubFunc::Unicode2Utf8(string strUnicode)
{
    char16_t *szUnicode = (char16_t*)strUnicode.c_str();
    u16string u16Unicode = szUnicode;
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;
    string strUtf8 = utf8_ucs2_cvt.to_bytes(u16Unicode); // ucs2 -> utf8
    return strUtf8;
}

bool CPubFunc::CloseSocket(SOCKET sock) {
    shutdown(sock, SHUT_RDWR);//SD_BOTH
    return false;
}
