#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <string>
#include <direct.h>
#include <io.h>
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

// Get the distance on earth surface between two points
// Refer to http://blog.sina.com.cn/s/blog_65d859c30101akih.html
double GetDistanceInMeter(double lat1, double lng1, double lat2, double lng2)
{
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double a = radLat1 - radLat2;
    double b = rad(lng1) - rad(lng2);
    double sin_a_2 = sin(a/2);
    double sin_b_2 = sin(b/2);
    double s = 2 * asin(sqrt(sin_a_2 * sin_a_2 + cos(radLat1) * cos(radLat2) * (sin_b_2 * sin_b_2)));
    return s * R_EARTH;
}

// Get the distance on earth surface between two points with the same lat coordinate
double GetDistanceSameLatInMeter(double lat, double lng1, double lng2)
{
    double r = (R_EARTH * M_PI / 180) * cos(rad(lat)) * (lng2 - lng1);
    return r < 0 ? -r : r;
}

// Get the distance on earth surface between two points with the same lng coordinate
double GetDistanceSameLngInMeter(double lat1, double lat2)
{
    double r = (R_EARTH * M_PI / 180) * (lat2 - lat1);
    return r < 0 ? -r : r;
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

bool CreateDirNested(const char *pDir)
{
    int iRet = 0;
    int iLen;
    char* pszDir;

    if(NULL == pDir) {
        return false;
    }
    if (_access(pDir, 0) == 0) {
        return true;
    }

    pszDir = _strdup(pDir);
    iLen = (int)strlen(pszDir);

    for (int i = 0;i < iLen; i++) {
        if (pszDir[i] == '\\' || pszDir[i] == '/') { 
            pszDir[i] = '\0';

            // create the file if it does not exist
            iRet = _access(pszDir, 0);
            if (iRet != 0) {
                iRet = _mkdir(pszDir);
                if (iRet != 0) {
                    return false;
                }
            }
            pszDir[i] = '/'; // Replace '\' with '/' to support Linux
        }
    }
    
    if (_access(pszDir, 0) != 0) {
        iRet = _mkdir(pszDir);
    }
    free(pszDir);
    return iRet == 0;
}

static inline
void TrimLeftAndPushBack(vector<string> &strs, string &str) {
    const char *s = str.c_str();
    while (*s == ' ' || *s == '\t') s++;
    strs.push_back(s);
}

void CsvLinePopulate(vector<string> &record, const string &line, char delimiter)
{
    int linepos = 0;
    int inquotes = false;
    char c;
    int linemax = (int)line.length();
    std::string curstring;
    record.clear();

    while (line[linepos] != 0 && linepos < linemax) {

        c = line[linepos];

        if (!inquotes && curstring.length() == 0 && c == '"') {
            //beginquotechar
            inquotes = true;
        } else if (inquotes && c == '"') {
            //quotechar
            if ((linepos + 1 < linemax) && (line[linepos + 1] == '"')) {
                //encountered 2 double quotes in a row (resolves to 1 double quote)
                curstring.push_back(c);
                linepos++;
            } else {
                //endquotechar
                inquotes = false;
            }
        } else if (!inquotes && c == delimiter) {
            //end of field
            TrimLeftAndPushBack(record, curstring);
            curstring = "";
        } else if (!inquotes && (c == '\r' || c == '\n')) {
            TrimLeftAndPushBack(record, curstring);
            return;
        } else {
            curstring.push_back(c);
        }
        linepos++;
    }

    TrimLeftAndPushBack(record, curstring);
}
