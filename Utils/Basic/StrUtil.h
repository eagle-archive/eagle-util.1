#ifndef _STR_UTIL_H
#define _STR_UTIL_H

#include <string>

namespace utils {

bool WideStrToAnsiStr(std::string &str, const WCHAR * wstr);
void StringReplace(std::string &strBase, const std::string &strSrc, const std::string &strDes);

}

#endif // _STR_UTIL_H
