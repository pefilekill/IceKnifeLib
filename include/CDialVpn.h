
//实现系统的vpn拨号

#pragma once

#include <string>
#include "PreDefine.h"

using namespace std;

class CDialVpn
{
public:
	enum VpnStrategy
	{
		Strategy_Default = 0,
		Strategy_PptpOnly = 1,
		Strategy_L2tpOnly = 3,
	};



	// dwStrategy = VS_L2tpOnly VS_PptpOnly
	bool  createConn(string lpszPkbPath, string lpszDeviceName, string lpszDialupName, string lpszIPAddress,  VpnStrategy emStrategy, string lpszPreKey );
	//开始vpn
	bool	Dial(string lpszPkbPath,string szEntryName, string szUserName, string szUserPwd, string szDomain);
	//挂断所有
	void	HangUpAll();

protected:
private:


};

