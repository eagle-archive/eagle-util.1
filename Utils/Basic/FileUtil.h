#ifndef _FILE_UTIL_H
#define _FILE_UTIL_H

#include <string>
#include <afxwin.h>         // MFC core and standard components

namespace utils {

bool FileExists(const char* pathname);
bool DirectoryExists(const char* directory);
bool RecursiveCreateDirectory(const char *directory);

std::string GetCurModulePathname();
std::string GetFilePath(const char* pathname);
std::string GetFileName(const char* pathname);
std::string GetFileTitle(const char* pathname);

bool WriteAllLines(const TCHAR * name, const CStringArray &linesArray);

}

#endif // _FILE_UTIL_H
