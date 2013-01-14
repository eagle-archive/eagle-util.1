#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <string>
#include "HotSquare.h"

using namespace std;



std::string FormatTimeStr(unsigned int uTimeMs)
{
    char buff[64];
    sprintf(buff, "%2d:%02d:%03d",
        (uTimeMs/60000), (uTimeMs/1000) % 60, uTimeMs % 1000);
    return buff;
}

static unsigned int g_dwStart = ::GetTickCount();
std::string ElapsedTimeStr() {
    return FormatTimeStr(::GetTickCount() - g_dwStart);
}


static inline double rad(double d)
{
    return d * M_PI / 180.0;
}

// Refer to http://blog.sina.com.cn/s/blog_65d859c30101akih.html
double GetDistanceInMeter(double lat1, double lng1, double lat2, double lng2)
{
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double a = radLat1 - radLat2;
    double b = rad(lng1) - rad(lng2);
    double sin_a_2 = sin(a/2);
    double sin_b_2 = sin(b/2);
    double s = 2 * asin(sqrt((sin_a_2*sin_a_2) + cos(radLat1)*cos(radLat2)* (sin_b_2*sin_b_2)));
    return s * 6378137; // 6378137 is earth radius in meter
}

void StringReplace(string &strBase, const string &strSrc, const string &strDes)
{
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type desLen = strDes.size();
    pos = strBase.find(strSrc, pos); 
    while ((pos != string::npos)) {
        strBase.replace(pos, srcLen, strDes);
        pos=strBase.find(strSrc, (pos+desLen));
    }
}


#include <direct.h>
#include <io.h>

bool CreatDirNested(const char *pDir)
{
    int i = 0;
    int iRet;
    int iLen;
    char* pszDir;

    if(NULL == pDir) {
        return false;
    }

    pszDir = _strdup(pDir);
    iLen = strlen(pszDir);

    // 创建中间目录
    for (i = 0;i < iLen;i ++) {
        if (pszDir[i] == '\\' || pszDir[i] == '/') { 
            pszDir[i] = '\0';

            //如果不存在,创建
            iRet = _access(pszDir,0);
            if (iRet != 0) {
                iRet = _mkdir(pszDir);
                if (iRet != 0) {
                    return false;
                } 
            }
#if !defined(_WIN32)
            //支持linux,将所有\换成/
            pszDir[i] = '/';
#endif
        }
    }

    iRet = _mkdir(pszDir);
    free(pszDir);
    return iRet == 0;
}
