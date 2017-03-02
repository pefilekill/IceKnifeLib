
#include "CAesUtil.h"
#include "openssl/aes.h"

string CAesUtil::Encrypt(string strKey, string strData, string strIv)
{
	string strResult = "";

	if (strKey.length() == 0 || strData.length() == 0)
		return strResult;

	AES_KEY aes;
	if(AES_set_encrypt_key((unsigned char*)strKey.c_str(), 128, &aes) < 0)
		return strResult;
	
	char *szOut = new char[AES_BLOCK_SIZE];
	int iDataLen = strData.length();
	int iEnLen = 0;
	
	int iAllocLen = (strData.length()/AES_BLOCK_SIZE + 1)*16;//(strData.length()%AES_BLOCK_SIZE ==0) ? strData.length() : (strData.length()/AES_BLOCK_SIZE + 1)*16;
	unsigned char* szOriData = new unsigned char[iAllocLen];
	memset(szOriData, iAllocLen - iDataLen, iAllocLen);  //PKCS5Padding填充
	memcpy(szOriData, strData.c_str(), strData.length());;//memcpy_s(szOriData, iAllocLen, strData.c_str(), strData.length());

	while (iEnLen < iDataLen)
	{
		memset(szOut, 0, AES_BLOCK_SIZE);
		AES_encrypt(szOriData + iEnLen, (unsigned char*)szOut, &aes); //其实就是ECB模式
		iEnLen += AES_BLOCK_SIZE;
		strResult.append(szOut, AES_BLOCK_SIZE);// += szOut;
	}
	delete[] szOut;
	delete[] szOriData;
	return strResult;
}
//
string CAesUtil::Decrypt(string strKey, string strData, string strIv)
{
	string strResult = "";

	if (strKey.length() == 0 || strData.length() == 0)
		return strResult;

	AES_KEY aes;
	if(AES_set_decrypt_key((unsigned char*)strKey.c_str(), 128, &aes) < 0)
		return strResult;

	char *szOut = new char[AES_BLOCK_SIZE];
	int iDataLen = strData.length();
	int iEnLen = 0;

	//int iAllocLen = (strData.length()%AES_BLOCK_SIZE ==0) ? strData.length() : (strData.length()/AES_BLOCK_SIZE + 1)*16;
	//unsigned char* szOriData = new unsigned char[iAllocLen];
	//memset(szOriData, iAllocLen - iDataLen, iAllocLen);  //PKCS5Padding填充
	//memcpy_s(szOriData, iAllocLen, strData.c_str(), strData.length());

	string strDecode = "";
	while (iEnLen < iDataLen)
	{
		memset(szOut, 0, AES_BLOCK_SIZE);;//::ZeroMemory(szOut, AES_BLOCK_SIZE);
		AES_decrypt((unsigned char*)(strData.c_str() + iEnLen), (unsigned char*)szOut, &aes); //其实就是ECB模式
		iEnLen += AES_BLOCK_SIZE;
		strDecode.append(szOut, AES_BLOCK_SIZE);// += szOut;
	}
	delete[] szOut;
	int iSubLen = strDecode[strDecode.length() - 1];
	strResult = strDecode.substr(0, strDecode.length() - iSubLen);

//	delete[] szOriData;

	return strResult;
}


