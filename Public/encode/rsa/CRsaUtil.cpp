

#include "openssl/rsa.h"
#include  "openssl/bio.h"
#include "openssl/pem.h"
#include "Base64.h"
#include "CRsaUtil.h"


//
bool CRsaUtil::RSA_Private_Encode(string &strKey, string& strData, int iStepLen, string &strOut)
{
	bool bRet = false;
	char *szRSAEncrypt = NULL;
	BIO *bp = BIO_new(BIO_s_mem());
	RSA *pRsa = NULL;
	if (bp == NULL)
		goto Quit;
	if (BIO_write(bp, (void*)strKey.c_str(), strKey.length()) <= 0)
		goto Quit;

	pRsa = PEM_read_bio_RSAPrivateKey(bp, NULL, NULL, NULL);
	if (pRsa == NULL)
	{
		int iLen = RSA_size(pRsa);
		szRSAEncrypt = new char[iLen +1 ];
		memset(szRSAEncrypt, 0, iLen + 1);

		int iOffset = 0;
		int iDataLen = strData.length();
		int iFlen =  iStepLen;
		while (iOffset < iDataLen)
		{
			memset(szRSAEncrypt, 0, iLen + 1);
			if (iOffset + iStepLen > iDataLen)
				iFlen = iDataLen - iOffset;
			else
				iFlen = iStepLen;
			int iRet = RSA_private_encrypt(iFlen, (unsigned char*)strData.c_str() + iOffset, (unsigned char*)szRSAEncrypt, pRsa, RSA_PKCS1_PADDING);
			if (iRet >=0)
				strOut.append(szRSAEncrypt,iRet);
			iOffset += iStepLen;
		}
		bRet = true;
	}


Quit:
	BIO_free_all(bp);
	RSA_free(pRsa);
	if (szRSAEncrypt != NULL)
		delete[] szRSAEncrypt;

	return bRet;
}

string CRsaUtil::CompleteRSAPublicKey(string strPublicKey)
{
	int nPublicKeyLen = strPublicKey.size();      //strPublicKeyΪbase64����Ĺ�Կ�ַ���

	string strRet = "";
	int iLoop = 0;
	while (true)
	{
		int iStart = iLoop * 64;
		int iEnd = iLoop * 64 + 64;
		if (iEnd > nPublicKeyLen)
		{
			strRet += strPublicKey.substr(iStart, nPublicKeyLen - iStart) + "\n";
			break;
		}
		else
			strRet += strPublicKey.substr(iStart, 64) + "\n";
		iLoop++;
	}

	strRet = "-----BEGIN PUBLIC KEY-----\n" + strRet;
	strRet += "-----END PUBLIC KEY-----\n";
	return strRet;
}
//
string CRsaUtil::CompleteRSAPrivateKey(string strPublicKey)
{
	int nPublicKeyLen = strPublicKey.size();      //strPublicKeyΪbase64����Ĺ�Կ�ַ���

	string strRet = "";
	int iLoop = 0;
	while (true)
	{
		int iStart = iLoop * 64;
		int iEnd = iLoop * 64 + 64;
		if (iEnd > nPublicKeyLen)
		{
			strRet += strPublicKey.substr(iStart, nPublicKeyLen - iStart) + "\n";
			break;
		}
		else
			strRet += strPublicKey.substr(iStart, 64) + "\n";
		iLoop++;
	}

	strRet = "-----BEGIN RSA PRIVATE KEY-----\n" + strRet;
	strRet += "-----END RSA PRIVATE KEY-----\n";
	return strRet;
}

//
bool CRsaUtil::RSA_Public_Encode(string &strKey, string& strData, int iStepLen, string &strOut)
{
	bool bRet = false;
	char *szRSAEncrypt = NULL;
	BIO *bp = BIO_new(BIO_s_mem());
	if (bp == NULL)	goto Quit;
	if (BIO_write(bp, (void*)strKey.c_str(), strKey.length()) <= 0)
	{
		RSA *pRsa = PEM_read_bio_RSA_PUBKEY(bp, NULL, NULL, NULL);//PEM_read_bio_RSAPublicKey(bp, NULL, NULL, NULL);
		if (pRsa == NULL)
		{
			int iLen = RSA_size(pRsa);
			szRSAEncrypt = new char[iLen +1 ];
			memset(szRSAEncrypt, 0, iLen + 1);

			int iOffset = 0;
			int iDataLen = strData.length();
			int iFlen =  iStepLen;
			while (iOffset < iDataLen)
			{
				memset(szRSAEncrypt, 0, iLen + 1);
				if (iOffset + iStepLen > iDataLen)
					iFlen = iDataLen - iOffset;
				else
					iFlen = iStepLen;
				int iRet = RSA_public_encrypt(iFlen, (unsigned char*)strData.c_str() + iOffset, (unsigned char*)szRSAEncrypt, pRsa, RSA_PKCS1_PADDING);
				if (iRet >=0)
					strOut.append(szRSAEncrypt,iRet);
				iOffset += iStepLen;
			}
			RSA_free(pRsa);
			bRet = true;
		}
	}

Quit:
	BIO_free_all(bp);

	if (szRSAEncrypt != NULL)
		delete[] szRSAEncrypt;

	return bRet;
}
//
bool CRsaUtil::RSA_Public_Decode(string &strKey, string& strData, int iStepLen, string &strOut)
{
	bool bRet = false;
	char *szRSADecrypt = NULL;
	BIO *bp = BIO_new(BIO_s_mem());
	if (bp == NULL)	goto Quit;
	if (BIO_write(bp, (void*)strKey.c_str(), strKey.length()) <= 0)
	{
		//PEM_read_bio_RSAPrivateKey fail
		RSA *pRsa = PEM_read_bio_RSA_PUBKEY(bp, NULL, NULL, NULL);//PEM_read_bio_RSAPublicKey(bp, NULL, NULL, NULL);
		if (pRsa == NULL)
		{
			int iLen = RSA_size(pRsa);
			szRSADecrypt = new char[iLen +1 ];
			memset(szRSADecrypt, 0, iLen + 1);

			int iOffset = 0;
			int iDataLen = strData.length();
			int iFlen =  iStepLen;
			while (iOffset < iDataLen)
			{
				memset(szRSADecrypt, 0, iLen + 1);
				if (iOffset + iStepLen > iDataLen)
					iFlen = iDataLen - iOffset;
				else
					iFlen = iStepLen;
				int iRet = RSA_public_decrypt(iFlen, (unsigned char*)strData.c_str() + iOffset, (unsigned char*)szRSADecrypt, pRsa, RSA_PKCS1_PADDING);
				if (iRet >=0)
					strOut.append(szRSADecrypt,iRet);
				iOffset += iStepLen;
			}
			RSA_free(pRsa);
			bRet = true;
		}
	}
Quit:
	BIO_free_all(bp);
	if (szRSADecrypt != NULL)
		delete[] szRSADecrypt;

	return bRet;
}