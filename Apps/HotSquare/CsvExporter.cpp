#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <fstream>
#include <iostream>
#include "HotSquare.h"
#include "CsvExporter.h"

using namespace std;

CsvExporter::CsvExporter(const char *sOutFolder, const char *sSchema, const char *sTableName, const char *sTemplateFolder)
{
    mstrBaseFolder = sOutFolder;
    StringReplace(mstrBaseFolder, "/", "\\");
    if (!mstrBaseFolder.empty() && mstrBaseFolder[mstrBaseFolder.size()-1] != '\\') {
        mstrBaseFolder += '\\';
    }
    mstrTemplateFolder = sTemplateFolder;
    if (!mstrTemplateFolder.empty() && mstrTemplateFolder[mstrTemplateFolder.size()-1] != '\\') {
        mstrTemplateFolder += '\\';
    }
    mstrDataFolder = mstrBaseFolder + "index\\" + mstrSchema + "\\" + mstrTableName.substr(0, 2) + '\\';

    mstrSchema = sSchema;
    mstrTableName = sTableName;
}

CsvExporter::~CsvExporter()
{
}

string CsvExporter::GetDataFilePath()
{
    return mstrDataFolder + "data.csv";
}

bool CsvExporter::GenerateExportFiles()
{
    if (!CreateFolders()) {
        return false;
    }
    return true;
}

bool CsvExporter::CreateFolders()
{
    if (!CreatDirNested(mstrDataFolder.c_str())) {
        mstrError = "Cannot create sub-folder for CSV data files.";
        return false;
    }
    return true;
}
