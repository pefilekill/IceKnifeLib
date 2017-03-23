//模块类

#pragma once

#include "zlib.h"
#include "PreDefine.h"
#include <string>


using namespace std;


class CUtil
{
public:
	void Test();
	// GZip Compression
	static bool CompressGzip(const string& dataSrc, string& dataOut, int level = -1);
	// GZip Decompression
	static bool UncompressGzip(const string& dataSrc, string& dataOut);
	//deflate  没有处理分段的情况
	static bool CompressDeflate(const string& dataSrc, string& daraOut);
	static bool UnCompressDeflate(const string& dataSrc, string& daraOut);
	//
	static bool UnCompressInflate(const string& dataSrc, string& dataOut);


	//gzip 压缩
	static int			gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);
	//gzip  解压
	static int			gzdecompress(Byte *srcData, uLong srcDataLen,	Byte *destData, uLong *destDataLen);

protected:
private:
};
