
#pragma once
#include <string>

using namespace std;
class CAesUtil
{
public:
	//加密  
	static string		Encrypt(string strKey, string strData, string strIv);
	//解密
	static string		Decrypt(string strKey, string strData, string strIv);

protected:
private:
};

