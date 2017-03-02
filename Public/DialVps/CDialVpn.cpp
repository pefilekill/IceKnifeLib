
#ifdef __OS_WINDOWS__
#include <stdafx.h>
#include <atlstr.h>
#include <WinSock2.h>
#include <Ras.h>
#include <Raserror.h>
#endif



#include <string>

#include "CDialVpn.h"
#include "CPubFuncUtil.h"

#pragma comment(lib, "Rasapi32.lib")

#ifdef __OS_WINDOWS__
CSimpleArray<HRASCONN> g_arrayHconn;
#endif

//vps拨号名  拨号地址 
bool CDialVpn::createConn(string lpszPkbPath, string lpszDeviceName, string lpszDialupName, string lpszIPAddress, VpnStrategy emStrategy, string lpszPreKey)
{
    bool bCreate = false;
#ifdef __OS_WINDOWS__

	RASENTRY	rasEntry;
	ZeroMemory(&rasEntry, sizeof(rasEntry));
	
	DWORD dwEntryInfoSize = 0; 
	RasGetEntryProperties(NULL, NULL, NULL, &dwEntryInfoSize, NULL,NULL); 
	rasEntry.dwSize				= dwEntryInfoSize;//sizeof (RASENTRY);
	//rasEntry.dwfOptions			= RASEO_RemoteDefaultGateway | RASEO_SpecificNameServers;
	//rasEntry.dwfOptions         = RASEO_RequireMsEncryptedPw | RASEO_RequireDataEncryption;
	//entry.dwfOptions = entry.dwfOptions | RASEO_UseCountryAndAreaCodes;
	//entry.dwfOptions = entry.dwfOptions | RASEO_SpecificIpAddr;
	//entry.dwfOptions = entry.dwfOptions | RASEO_SpecificNameServers;
	//entry.dwfOptions = entry.dwfOptions | RASEO_IpHeaderCompression;
	rasEntry.dwfOptions = rasEntry.dwfOptions | RASEO_RemoteDefaultGateway;
	//entry.dwfOptions = entry.dwfOptions | RASEO_DisableLcpExtensions; //禁用LCP扩展
	// entry.dwfOptions = entry.dwfOptions | RASEO_TerminalBeforeDial;
	// entry.dwfOptions = entry.dwfOptions | RASEO_TerminalAfterDial;
	rasEntry.dwfOptions = rasEntry.dwfOptions | RASEO_ModemLights;//连接后在通知区域显示图标
	rasEntry.dwfOptions = rasEntry.dwfOptions | RASEO_SwCompression; //启用软件压缩
	//entry.dwfOptions = entry.dwfOptions | RASEO_RequireEncryptedPw;
	//entry.dwfOptions = entry.dwfOptions | RASEO_RequireMsEncryptedPw;
	//entry.dwfOptions = entry.dwfOptions | RASEO_RequireDataEncryption;
	//entry.dwfOptions = entry.dwfOptions | RASEO_NetworkLogon;
	//entry.dwfOptions = entry.dwfOptions | RASEO_UseLogonCredentials;//登陆域
	rasEntry.dwfOptions = rasEntry.dwfOptions | RASEO_PromoteAlternates;
	rasEntry.dwfOptions = rasEntry.dwfOptions | RASEO_ShowDialingProgress; //显示连接进度
	rasEntry.dwfOptions2 = rasEntry.dwfOptions2 | RASEO2_DontNegotiateMultilink; //不为单路连接协商多路链接
	rasEntry.dwfOptions2 = rasEntry.dwfOptions2 | RASEO2_DisableNbtOverIP;//禁用tcp/ip上的NetBEUI
	//rasEntry.dwfOptions2 = rasEntry.dwfOptions2 | RASEO2_Internet;
	rasEntry.dwfOptions2 = rasEntry.dwfOptions2 | RASEO2_SecureClientForMSNet;//ms网络客户端
	rasEntry.dwfOptions2 = rasEntry.dwfOptions2 | RASEO2_SecureFileAndPrint;//文件和打印
	rasEntry.dwfOptions2 = rasEntry.dwfOptions2 | RASEO2_ReconnectIfDropped;//断线重拨
	rasEntry.dwfOptions2 = rasEntry.dwfOptions2 | RASEO2_UsePreSharedKey;

	rasEntry.dwAlternateOffset	= 0;
	rasEntry.dwCountryID		= 86;
	rasEntry.dwCountryCode		= 86;
	//rasEntry.dwDialExtraPercent = 75; //带宽比例  忽略即可
	//rasEntry.dwDialExtraSampleSeconds = 120;
	rasEntry.dwDialMode         = RASEDM_DialAll;
	rasEntry.dwfNetProtocols = 4;
	
	rasEntry.dwFramingProtocol = RASFP_Ppp;
	//rasEntry.dwHangUpExtraPercent = 10;
	//rasEntry.dwHangUpExtraSampleSeconds = 120;
	rasEntry.dwRedialCount = 0;
	//	rasEntry.dwRedialPause = 60;
	rasEntry.dwType = RASET_Vpn;
	
	rasEntry.dwFrameSize		= 0;
	rasEntry.dwfNetProtocols	= RASNP_Ip;		// TCP/IP
	rasEntry.dwChannels			= 0;
	rasEntry.dwReserved1		= 0;
	rasEntry.dwReserved2		= 0;	

	// Strings
// 	wcscpy_s (rasEntry.szAreaCode,RAS_MaxAreaCode + 1, L" ");
// 	wcscpy_s (rasEntry.szScript,MAX_PATH, L"");
// 	wcscpy_s (rasEntry.szAutodialDll, MAX_PATH, L"");
// 	wcscpy_s (rasEntry.szAutodialFunc, MAX_PATH , L"");
// 	wcscpy_s (rasEntry.szX25PadType, RAS_MaxPadType + 1, L"");
// 	wcscpy_s (rasEntry.szX25Address, RAS_MaxX25Address + 1, L"");
// 	wcscpy_s (rasEntry.szX25Facilities, RAS_MaxFacilities + 1, L"");
// 	wcscpy_s (rasEntry.szX25UserData, RAS_MaxUserData + 1,L"");
	wcscpy_s (rasEntry.szDeviceName, RAS_MaxDeviceName + 1, lpszDeviceName);//L"WAN 微型端口 (L2TP)"
	wcscpy_s (rasEntry.szDeviceType, RAS_MaxDeviceType + 1, RASDT_Vpn);	
	wcscpy_s (rasEntry.szLocalPhoneNumber, RAS_MaxPhoneNumber + 1, lpszIPAddress);// "60.190.168.108");//服务器地址或域名
	rasEntry.dwVpnStrategy = emStrategy;     //vpn类型
	rasEntry.dwEncryptionType = ET_Require; //数据加密类型    

	//Create Entry
	DWORD dwError = ::RasSetEntryProperties (lpszPkbPath, lpszDialupName, &rasEntry, rasEntry.dwSize, NULL, NULL);
	if (dwError == ERROR_SUCCESS) 
	{
		//MessageBox(NULL, L"set suc", L"ds", MB_OK);
		RASCREDENTIALS  rasCts ;
		ZeroMemory(&rasCts, sizeof(RASCREDENTIALS));
		rasCts.dwSize = sizeof(RASCREDENTIALS);
		rasCts.dwMask = RASCM_PreSharedKey; //拨号预共享密钥
		wcscpy_s(rasCts.szPassword, PWLEN + 1, lpszPreKey);		
		dwError = RasSetCredentials(lpszPkbPath, lpszDialupName, &rasCts, 0);
		if (dwError == ERROR_SUCCESS)
			return true;
	}

	if (bCreate == FALSE)
	{
		CString strErr = L"";
		strErr.Format(L"错误:%d",dwError);
		MessageBox(NULL, strErr, L"ds", MB_OK);
	}

#endif

	return bCreate;
}

//开始vpn
bool CDialVpn::Dial(string lpszPkbPath,string szEntryName, string szUserName, string szUserPwd, string szDomain)
{
#ifdef __OS_WINDOWS__
    RASDIALPARAMS rasDialPara = {0};
	rasDialPara.dwSize = sizeof(RASDIALPARAMS);
	wcscpy_s(rasDialPara.szEntryName, RAS_MaxEntryName + 1, szEntryName);
	wcscpy_s(rasDialPara.szUserName, UNLEN + 1, szUserName);
	wcscpy_s(rasDialPara.szPassword, PWLEN + 1, szUserPwd);
	//wcscpy_s(rasDialPara.szDomain, DNLEN + 1, szDomain);

	HRASCONN hConn = NULL;
	DWORD dwError = RasDial(NULL, lpszPkbPath, &rasDialPara, NULL, NULL, &hConn);
	if (dwError != ERROR_SUCCESS)
	{
		RasHangUp(hConn);
		HangUpAll();
		if (dwError == ERROR_INVALID_SIZE)
			rasDialPara.dwSize = offsetof(RASDIALPARAMSW, dwIfIndex);

		dwError = RasDial(NULL, lpszPkbPath, &rasDialPara, NULL, NULL, &hConn);
	}

	if (dwError == ERROR_SUCCESS)
	{
		g_arrayHconn.Add(hConn);
		return true;
	}
#endif
	return false;
}
//挂断所有
void CDialVpn::HangUpAll()
{
#ifdef __OS_WINDOWS__
    DWORD dwCb = 0;
	DWORD dwConnections = 0;
	LPRASCONN lpRasConn = NULL;
	for (int iExist = 0; iExist < g_arrayHconn.GetSize(); iExist++)
	{
		RasHangUp(g_arrayHconn[iExist]);
	}
	if (g_arrayHconn.GetSize() > 0)
	{
		CPubFunc::NewSleep(5000);
	}



	DWORD dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);
	if (dwRet == ERROR_BUFFER_TOO_SMALL){
		lpRasConn = new RASCONN[dwConnections];
		lpRasConn[0].dwSize = sizeof(RASCONN);
		dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);
		if (ERROR_SUCCESS == dwRet){
			for (DWORD i = 0; i < dwConnections; i++){
				DWORD dwHang = RasHangUp(lpRasConn[i].hrasconn);
				CPubFunc::NewSleep(5000);
			}
		}
		//Deallocate memory for the connection buffer
		//HeapFree(GetProcessHeap(), 0, lpRasConn);
		delete[] lpRasConn;
		lpRasConn = NULL;
	}
#endif


}
