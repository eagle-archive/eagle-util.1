// TomcatAccess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Common/LogUtils.h"
#include "Common/CommUtils.h"
extern "C" {
#include "iniparser.h"
}


#define INI_FILE    "SimProviderServer.ini"

typedef struct {
    std::string     log_file;
    std::string     ip_db_csv;
} GLOBAL_CONFIGS_T;

GLOBAL_CONFIGS_T gConfigs;


bool LoadGlobalConfigs()
{
    dictionary *dict = iniparser_load(INI_FILE);
    if (NULL == dict)
    {
        Log(LOG_ERROR, "cannot open config file: %s", INI_FILE);
        return false;
    }

    std::string err_str;
    if (false == ReadIniString(dict, "MAIN:log_file", gConfigs.log_file, err_str))
    {
        goto exit;
    }
    if (false == ReadIniString(dict, "MAIN:ip_db_csv", gConfigs.ip_db_csv, err_str))
    {
        goto exit;
    }

exit:
    iniparser_freedict(dict);
    if (!err_str.empty())
    {
        Log(LOG_ERROR, "%s", err_str.c_str());
    }
    return !err_str.empty();
}

int main(int argc, char* argv[])
{
    return 0;
}
