#ifndef _COMMON_UTILS_H
#define _COMMON_UTILS_H

#ifdef _WIN32
#define snprintf _snprintf
#endif

#include <stdio.h>
#include <string>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
typedef int SOCKET;
#endif
#include "sqlext.h"
extern "C" {
#include "iniparser.h"
}
#include "LogUtils.h"


void GetCurTimestamp(SQL_TIMESTAMP_STRUCT &st);
void SleepMs(long ms);
unsigned long long GetTimeInMs64();
std::string GetCurrentPath();
void CsvLinePopulate(std::vector<std::string> &record, const std::string &line, char delimiter);
bool WriteStrToFile(const char *path, const std::string &data);

bool ReadIniInt(dictionary *dict, const char*section, int &value);
bool ReadIniString(dictionary *dict, const char *section, std::string &str);

bool LoadSocketLib();
bool SetSendTimeOutInMs(SOCKET sockfd, long timeout);
bool SetRecvTimeOutInMs(SOCKET sockfd, long timeout);

#endif // _COMMON_UTILS_H
