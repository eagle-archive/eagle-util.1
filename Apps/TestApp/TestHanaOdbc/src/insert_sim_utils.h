#ifndef __INSERT_SIM_UTILS_H_
#define __INSERT_SIM_UTILS_H_

#ifdef _WIN32
#include "windows.h"
#endif
#include "sqlext.h"
#include <string>
#include <fstream>

// for debug purpose only, comment it out for real DB connection
//#define FAKE_DB_CONN

#define CONFIG_FILE "TestHanaOdbc.ini"


struct UTIL_GLOBALS {
    // config
    std::string DSN;
    std::string USER;
    std::string PASSWORD;
    std::string TABLE_NAME;

    int  N_THREADS;
    int  N_RECORDS;
    int  RATE;

    std::string CSV_PATH;

    // status
    long start_time;
    long long insert_count;
    long long last_insert_count;
    long last_check_time;
    long actual_rate;
};


extern UTIL_GLOBALS GLOBALS;

bool init_globals(const char *config_file);
void update_rate_status();
long get_time_in_ms();

bool init_real_data_buffer();
void real_data_buffer_destory();

bool GetLine(std::ifstream &fs, std::string &line);
std::string FormatTimeStr(unsigned long uTimeMs);
std::string ElapsedTimeStr();

#endif // __INSERT_SIM_UTILS_H_
