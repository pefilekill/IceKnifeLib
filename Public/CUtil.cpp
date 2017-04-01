
#include <time.h>
#include <sys/timeb.h>
#include "CUtil.h"
#include <Windows.h>

#define CHUNK 16384
#define windowBits 15
#define GZIP_ENCODING 16


void CUtil::Test()
{


	return ;
}

bool CUtil::CompressDeflate(const string& dataSrc, string& dataOut)
{
	bool bRet = false;
	uLong ulMaxComLen = compressBound(dataSrc.length());
	Byte *byCom = new Byte[ulMaxComLen];
	memset(byCom, 0, ulMaxComLen);
	if (compress(byCom, &ulMaxComLen, (Bytef*)(dataSrc.c_str()), dataSrc.length()) !=Z_OK)
		bRet = false;
	else
	{
		dataOut.append((char*)byCom, ulMaxComLen);
		bRet = true;
	}
	delete[] byCom;
	return bRet;
}

bool CUtil::UnCompressDeflate(const string& dataSrc, string& dataOut)
{
	DWORD dwSize = dataSrc.length();
	unsigned char *szDest = new unsigned char[dwSize * 10];
	memset(szDest, 0, dwSize *10);

	uLong ulDest = dwSize * 10;
	
	bool bRet = false;
	int iUnRet = uncompress(szDest,&ulDest, (Bytef*)dataSrc.c_str(),dataSrc.length());
	if(iUnRet != Z_OK)
		bRet = false;
	else
	{
		bRet = true;
		dataOut.append((char*)szDest, ulDest);
	}
	delete[] szDest;
	return bRet;
}
//
bool CUtil::UnCompressInflate(const string& dataSrc, string& dataOut)
{

	unsigned have;

	unsigned char out[CHUNK] = {0};

	/* allocate inflate state */
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	if (inflateInit(&strm) != Z_OK)
		return false;

	/* decompress until deflate stream ends or end of file */
	bool bUnResult = true;
	int iLoop = 0;
	do {
		if (iLoop * CHUNK > (int)dataSrc.length())
			break;
		bool bFull = (dataSrc.length() - iLoop * CHUNK) > CHUNK;
		
		strm.avail_in = bFull  ? CHUNK : dataSrc.length();
		strm.next_in = (Bytef *)dataSrc.c_str() + iLoop * CHUNK;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			int ret = inflate(&strm, Z_SYNC_FLUSH);
			if (ret != Z_OK)
			{
				//Z_MEM_ERROR
				bUnResult = false;
			}
			have = CHUNK - strm.avail_out;
			dataOut.append((char*)out, have);
		} while (have > 0);
		iLoop ++;
		/* done when inflate() says it's done */
	} while (true);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return bUnResult;

}



bool CUtil::CompressGzip(const std::string& dataSrc, std::string& dataOut, int level)
{
    unsigned char out[CHUNK];
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    if (deflateInit2(&strm, level, Z_DEFLATED, windowBits | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        return false;
    }
    strm.next_in = (unsigned char*)dataSrc.c_str();
    strm.avail_in = dataSrc.size();
    do
    {
        int have;
        strm.avail_out = CHUNK;
        strm.next_out = out;
        if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR)
        {
            return false;
        }
        have = CHUNK - strm.avail_out;
        dataOut.append((char*)out, have);
    }
    while (strm.avail_out == 0);
    if (deflateEnd(&strm) != Z_OK)
    {
        return false;
    }
    return true;
}

bool CUtil::UncompressGzip(const std::string& dataSrc, std::string& dataOut)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK)
    {
        return false;
    }

    strm.avail_in = dataSrc.size();
    strm.next_in = (unsigned char*)dataSrc.c_str();
    do
    {
        strm.avail_out = CHUNK;
        strm.next_out = out;
        ret = inflate(&strm, Z_NO_FLUSH);
        switch (ret)
        {
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            inflateEnd(&strm);
            return false;
        }
        have = CHUNK - strm.avail_out;
        dataOut.append((char*)out, have);
    }
    while (strm.avail_out == 0);

    if (inflateEnd(&strm) != Z_OK)
    {
        return false;
    }

    return true;
}
/* Compress gzip data */
/* data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 */
int CUtil::gzcompress(Bytef *srcData, uLong srcDataLen, Bytef *destData, uLong *destDataLen)
{
    z_stream c_stream;
    int err = 0;

    if(srcData && srcDataLen > 0)
    {
        c_stream.zalloc = NULL;
        c_stream.zfree = NULL;
        c_stream.opaque = NULL;
        //只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
        if(deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
        c_stream.next_in  = srcData;
        c_stream.avail_in  = srcDataLen;
        c_stream.next_out = destData;
        c_stream.avail_out  = *destDataLen;
        while(c_stream.avail_in != 0 && c_stream.total_out < *destDataLen)
        {
            if(deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
        }
        if(c_stream.avail_in != 0) return c_stream.avail_in;
        for(;;)
        {
            if((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
            if(err != Z_OK) return -1;
        }
        if(deflateEnd(&c_stream) != Z_OK) return -1;
        *destDataLen = c_stream.total_out;
        return 0;
    }
    return -1;
}
/* Uncompress gzip data */
/* zdata 数据 nzdata 原数据长度 data 解压后数据 ndata 解压后长度 */
int CUtil::gzdecompress(Byte *srcData, uLong srcDataLen,	Byte *destData, uLong *destDataLen)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] =
    {
        0x8 + 0x7 * 0x10,
        (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 139) & 0xFF,
    };
    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in  = srcData;
    d_stream.avail_in = 0;
    d_stream.next_out = destData;
    //只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
    if(inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK) return -1;
    //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while(d_stream.total_out < *destDataLen && d_stream.total_in < srcDataLen)
    {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if(err != Z_OK)
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
                {
                    return -1;
                }
            }
            else return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) return -1;
    *destDataLen = d_stream.total_out;
    return 0;
}