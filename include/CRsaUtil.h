
#pragma once
#include <string>

using namespace std;

class CRsaUtil
{
public:
	static string	CompleteRSAPrivateKey(string strPublicKey);
	static string	CompleteRSAPublicKey(string strPublicKey);
	//RSA  ˽Կ����
	static	bool	RSA_Private_Encode(string &strKey, string& strData, int iStepLen, string &strOut);
	static	string	RSA_Private_Decode(string &strKey, string& strData);
	//
	//��Կ
	static bool		RSA_Public_Encode(string &strKey, string& strData, int iStepLen, string &strOut);
	static bool		RSA_Public_Decode(string &strKey, string& strData, int iStepLen, string &strOut);
	//ecb ģʽ
	//static bool		RSA_Public_Encode_ECB(string &strKey, string& strData, int iStepLen, string &strOut);
protected:
private:
};