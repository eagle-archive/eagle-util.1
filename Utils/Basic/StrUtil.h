#ifndef _STR_UTIL_H
#define _STR_UTIL_H

#include <string>

namespace utils {

bool WideStrToAnsiStr(std::string &str, const WCHAR * wstr);
void StringReplace(std::string &strBase, const std::string &strSrc, const std::string &strDes);

std::string &TrimStr(const std::string& str, const char *whitespace = " \t");
std::string &ReduceStr(const std::string& str, const char *fill = " ", const char *whitespace = " \t");

}

#endif // _STR_UTIL_H
