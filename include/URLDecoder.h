
//URLDecoder
#pragma once
#include <string>

using namespace std;

class URLDecoder
{
public:
	static unsigned char ToHex(unsigned char x);

	static unsigned char FromHex(unsigned char x);

	static string UrlEncode(const std::string& str);

	static string UrlDecode(const std::string& str);
protected:
private:
};


