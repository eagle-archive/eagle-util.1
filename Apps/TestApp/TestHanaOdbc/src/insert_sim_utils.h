#ifndef __INSERT_SIM_UTILS_H_
#define __INSERT_SIM_UTILS_H_

#ifdef _WIN32
#include "windows.h"
#endif
#include "sqlext.h"
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

// for debug purpose only, comment it out for real DB connection
//#define FAKE_DB_CONN

#define CONFIG_FILE "TestHanaOdbc.ini"


struct UTIL_GLOBALS {
    // config
    std::string USER;
    std::string PASSWORD;
    std::string TABLE_NAME;

    int  N_THREADS;
    int  N_RECORDS;
    int  RATE;

    int  USE_REAL_DATA;
    std::string REAL_DATA_PATH;

    // status
    long start_time;
    long long insert_count;
    long long last_insert_count;
    long last_check_time;
    long actual_rate;

    // threads info and control
    //int ins_threads_end_flag;
    //int ins_threads_ended;
};


extern UTIL_GLOBALS GLOBALS;

bool init_globals(const char *config_file);
void update_rate_status();
void show_ODBC_error(const char* msg, SQLRETURN r, SQLHSTMT hstmt);
long get_time_in_ms();

bool init_real_data_buffer();
void real_data_buffer_destory();

#ifdef __cplusplus
}
#endif
#endif // __INSERT_SIM_UTILS_H_
