#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <assert.h>
#include <time.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include "CommUtils.h"


std::string GetCurrentPath()
{
    boost::filesystem::path full_path( boost::filesystem::current_path());
    return full_path.string();
}

unsigned long long GetTimeInMs64() {
#ifdef _WIN32
    return ::GetTickCount64();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

void GetCurTimestamp(SQL_TIMESTAMP_STRUCT &st) {
    time_t t;
    time(&t);
    struct tm stm;
#ifdef _WIN32
    localtime_s(&stm, &t);
#else
    localtime_r(&t, &stm);
#endif

    st.year = stm.tm_year + 1900;
    st.month = stm.tm_mon + 1;
    st.day = stm.tm_mday;
    st.hour = stm.tm_hour;
    st.minute = stm.tm_min;
    st.second = stm.tm_sec;
    st.fraction = 0;
}

bool LoadSocketLib()
{
#ifdef _WIN32
    WSADATA wsaData;
    int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (NO_ERROR != nResult) {
        Log(LOG_ERROR, "failed to init Winsock!");
        return false;
    }
#endif
    return true;
}

void SleepMs(long ms)
{
    boost::this_thread::sleep( boost::posix_time::milliseconds(ms) );
}

bool SetSendTimeOutInMs(SOCKET sockfd, long timeout)
{
    int ret;
#ifdef _WIN32
    unsigned int tout = timeout;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout) );
#else
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
#endif
    return ret == 0;
}

bool SetRecvTimeOutInMs(SOCKET sockfd, long timeout)
{
    int ret;
#ifdef _WIN32
    unsigned int tout = timeout;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout) );
#else
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
#endif
    return ret == 0;
}

bool ReadIniString(dictionary *dict, const char *section, std::string &str)
{
    char *value = iniparser_getstring(dict, section, NULL);
    if (NULL == value) {
        Log(LOG_WARNING, "cannot find %s in config file", section);
        return false;
    }
    str = value;
    return true;
}

bool ReadIniInt(dictionary *dict, const char*section, int &value)
{
    value = iniparser_getint(dict, section, 0xCDCDCDCD);
    if ((unsigned int)value == 0xCDCDCDCD) {
        Log(LOG_WARNING, "cannot find %s in config file", section);
        return false;
    }
    return true;
}

static inline
void trim_left_and_push_back(std::vector<std::string> &strs, std::string &str)
{
    const char *s = str.c_str();
    while (*s == ' ' || *s == '\t') s++;
    strs.push_back(s);
}

void CsvLinePopulate(std::vector<std::string> &record, const std::string &line, char delimiter)
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
            trim_left_and_push_back(record, curstring);
            curstring = "";
        } else if (!inquotes && (c == '\r' || c == '\n')) {
            trim_left_and_push_back(record, curstring);
            return;
        } else {
            curstring.push_back(c);
        }
        linepos++;
    }

    trim_left_and_push_back(record, curstring);
}

bool WriteStrToFile(const char *path, const std::string &data)
{
    std::ofstream out(path);
    if (!out.good()) return false;
    out << data;
    return true;
}
