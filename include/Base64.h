//实现base64的加密和解密
//输出和输入改为string以降低内存没有释放的概率

#pragma once
#include <string>
#include <stdlib.h>
using namespace std;

const char gBase[]= {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="};


static union
{
    //为了方便操作
    struct
    {
        unsigned long d:6;
        unsigned long c:6;
        unsigned long b:6;
        unsigned long a:6;
    } s;
    unsigned char c[3];
} u;


class CBase64
{
public:
	//char* data,unsigned long inlen,unsigned long *outlen
    static string gEncBase64(string strData)
    {
		string strRet = "";
        //编码
        char *p=NULL,*ret=NULL,*p1=NULL;
        int tlen=0,i=0,j=0;
        if(strData.length() == 0)
            goto QuitLable;//传入参数不对
        tlen=strData.length()/3;
        if(strData.length()%3!=0)
            tlen=tlen+1;
        tlen=tlen*4;
        //*outlen=tlen;//编码后的数据长度
        if((ret=(char*)malloc(tlen+1))==NULL)
            goto QuitLable;//内存开辟失败
        memset(ret,0,tlen+1);
        p=(char*)strData.c_str();
        p1=ret;
        tlen=strData.length();
        while(tlen>0)
        {
            //循环编码
            u.c[0]=u.c[1]=u.c[2]=0;
            for(i=0; i<3; i++)
            {
                //取3字节
                if(tlen<=0) break;
                u.c[2-i]=(char)*p;
                tlen--;
                p++;
            }
            if(i==0)
                break;//没取到
            switch(i)
            {
            //根据取到的字节数分情况
            case 1://取到1字节
                *p1=gBase[u.s.a];
                p1++;
                *p1=gBase[u.s.b];
                p1++;
                *p1=gBase[64];
                p1++;//'=';
                *p1=gBase[64];
                p1++;//'=';
                break;
            case 2://取到2字节
                *p1=gBase[u.s.a];
                p1++;
                *p1=gBase[u.s.b];
                p1++;
                *p1=gBase[u.s.c];
                p1++;
                *p1=gBase[64];
                p1++;//'=';
                break;
            case 3://取到3字节
                *p1=gBase[u.s.a];
                p1++;
                *p1=gBase[u.s.b];
                p1++;
                *p1=gBase[u.s.c];
                p1++;
                *p1=gBase[u.s.d];
                p1++;
                break;
            default:
                break;
            }
        }
		strRet = ret;
QuitLable:
		if (ret != NULL)
			free(ret);

        return strRet;
    }
	//char* data,unsigned long inlen,unsigned long *outlen
    static string gDecBase64(string strData)
    {
		string strRet = "";
        //解码
        //	int c1,c2,c4;//c3,
        char ch1=0, ch2=0, ch3=0, ch4=0, *p=NULL, *ret=NULL;//
        char *p1=NULL, *p2=NULL, *p3=NULL, *p4=NULL;
        int tlen=0,rlen=0;
        if(strData.length() == 0)
            goto QuitLable;//传入参数错误
        tlen=strData.length()*3/4;
        if((ret=(char*)malloc(tlen+1))==NULL)
            goto QuitLable;//内存开辟失败
        memset(ret,0,tlen+1);
        p= (char*)strData.c_str();//data;
        tlen=strData.length();
        rlen=0;
        while(tlen>0)
        {
            //循环解码
            if(tlen>0)
            {
                //取第1位
                ch1=*p;
                p++;
                tlen--;
                if((p1=(char*)strchr(gBase,ch1))==NULL)//不在表中
                    goto QuitLable;
            }
            if(tlen>0)
            {
                //取第2位
                ch2=*p;
                p++;
                tlen--;
                if((p2=(char*)strchr(gBase,ch2))==NULL)//不在表中
                    goto QuitLable;
            }
            if(tlen>0)
            {
                //取第3位
                ch3=*p;
                p++;
                tlen--;
                if((p3=(char*)strchr(gBase,ch3))==NULL)//不在表中
                    goto QuitLable;
            }
            if(tlen>0)
            {
                //取第4位
                ch4=*p;
                p++;
                tlen--;
                if((p4=(char*)strchr(gBase,ch4))==NULL)//不在表中
                    goto QuitLable;
            }
            if((ch1==gBase[64])||(ch2==gBase[64])
                    ||(ch3==gBase[64] && ch4!=gBase[64]))
                goto QuitLable;//编码有错误
            u.s.a=p1-gBase;
            u.s.b=p2-gBase;
            if (ch3==gBase[64])
            {
                //原数据只有1个字节
                *(ret+rlen)=(char)u.c[2];
                rlen++;
            }
            else if (ch4==gBase[64])
            {
                //原数据只有2个字节
                u.s.c=p3-gBase;
                *(ret+rlen)=(char)u.c[2];
                rlen++;
                *(ret+rlen)=(char)u.c[1];
                rlen++;
            }
            else
            {
                //原数据有3个字节
                u.s.c=p3-gBase;
                u.s.d=p4-gBase;
                *(ret+rlen)=(char)u.c[2];
                rlen++;
                *(ret+rlen)=(char)u.c[1];
                rlen++;
                *(ret+rlen)=(char)u.c[0];
                rlen++;
            }
        }
        //*outlen=rlen;
		strRet.append(ret, rlen);
//		strRet = ret;
QuitLable:
		if (ret != NULL)
			free(ret);
        return strRet;
    }
protected:
private:
};


