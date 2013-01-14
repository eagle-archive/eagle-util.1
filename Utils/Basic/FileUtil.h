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
bool CreateDirNested(const char *pDir);

bool WriteAllLines(const TCHAR *name, const CStringArray &linesArray);
bool WriteStrToFile(const char *name, const std::string &data);
bool GetLine(std::ifstream &fs, std::string &line);
bool ReadAllFromFile(const char *path, std::string &data);

}

#endif // _FILE_UTIL_H
