#include <time.h>
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <string>
#include "insert_sim_utils.h"
extern "C" {
#include "iniparser.h"
}

using namespace std;

UTIL_GLOBALS GLOBALS;

static
bool read_ini_string(dictionary *dict, const char*section, string &buff) {
    char *value = iniparser_getstring(dict, section, NULL);
    if (NULL == value) {
        printf("ERROR: cannot find %s in config file!\n", section);
        return false;
    }
    buff = value;
    return true;
}

static
bool read_ini_int(dictionary *dict, const char*section, int &value) {
    value = iniparser_getint(dict, section, 0xCDCDCDCD);
    if ((unsigned int)value == 0xCDCDCDCD) {
        printf("ERROR: cannot find %s in config file!\n", section);
        return false;
    }
    return true;
}

static
bool read_configs_from_ini(const char *config_file, UTIL_GLOBALS *p_globals) {
    dictionary *dict = iniparser_load(config_file);
    if (NULL == dict) {
        printf("ERROR: cannot open config file: %s\n", config_file);
        return false;
    }

    if (false == read_ini_string(dict, "DB:USER", p_globals->USER)) {
        iniparser_freedict(dict);
        return false;
    }

    if (false == read_ini_string(dict, "DB:PASSWORD", p_globals->PASSWORD)) {
        iniparser_freedict(dict);
        return false;
    }

    if (false == read_ini_string(dict, "DB:TABLE_NAME", p_globals->TABLE_NAME)) {
        iniparser_freedict(dict);
        return false;
    }

    p_globals->N_THREADS = iniparser_getint(dict, "main:N_THREADS", 0xCDCDCDCD);
    if (0xCDCDCDCD == (unsigned int)GLOBALS.N_THREADS) {
        printf("ERROR: cannot find N_THREADS in config file\n");
        iniparser_freedict(dict);
        return false;
    }

    if (false == read_ini_int(dict, "main:N_THREADS", p_globals->N_THREADS)) {
        iniparser_freedict(dict);
        return false;
    }

    if (false == read_ini_int(dict, "main:N_RECORDS", p_globals->N_RECORDS)) {
        iniparser_freedict(dict);
        return false;
    }

    if (false == read_ini_int(dict, "main:RATE", p_globals->RATE)) {
        iniparser_freedict(dict);
        return false;
    }

    if (false == read_ini_int(dict, "main:USE_REAL_DATA", p_globals->USE_REAL_DATA)) {
        iniparser_freedict(dict);
        return false;
    }
    if (p_globals->USE_REAL_DATA) {
        if (false == read_ini_string(dict, "main:REAL_DATA_PATH", p_globals->REAL_DATA_PATH)) {
            iniparser_freedict(dict);
            return false;
        }
    }

    iniparser_freedict(dict);
    return true;
}

bool init_globals(const char *config_file) {
    if (false == read_configs_from_ini(config_file, &GLOBALS))
        return false;

    return true;
}

void show_ODBC_error(const char* msg, SQLRETURN r, SQLHSTMT hstmt) {
    SQLCHAR Error[1024 * 2];
    SQLCHAR State[10];
    SQLSMALLINT ErrLen;
    SQLSMALLINT i = 1;
    SQLINTEGER NativeErr;
    printf("\n ERROR %s  %d \n", msg, r);

    SQLRETURN rc;
    do {
        rc = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, State, &NativeErr, Error,
                sizeof(Error), &ErrLen);
        if (rc == SQL_NO_DATA)
            break;
        printf("ODBC ERROR: (%d): hstmt: %d: %s\n", i, rc, Error);
        i++;
    } while (rc == SQL_SUCCESS);
}

long get_time_in_ms() {
#ifdef _WIN32
    return (long)GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

void update_rate_status() {
    long cur = get_time_in_ms();
    if (cur - GLOBALS.last_check_time >= 1000) {
        long count_diff =  (long)(GLOBALS.insert_count - GLOBALS.last_insert_count);
        long time_diff = cur - GLOBALS.last_check_time;
        long actual_rate = (long)(count_diff / (time_diff/1000.0) + 0.5);

#ifdef _WIN32
        InterlockedExchange(&GLOBALS.actual_rate, actual_rate);
        InterlockedExchange(&GLOBALS.last_check_time, cur);
        InterlockedExchange64(&GLOBALS.last_insert_count, GLOBALS.insert_count);
#else
        __sync_lock_test_and_set(&GLOBALS.actual_rate, actual_rate);
        __sync_lock_test_and_set(&GLOBALS.last_check_time, cur);
        __sync_lock_test_and_set(&GLOBALS.last_insert_count, GLOBALS.insert_count);
#endif

        int up_time = (int)(cur - GLOBALS.start_time);
        int fraction = up_time % 1000;
        int sec = (up_time / 1000) % 100;
        int min = (up_time / 1000 / 60) % 100;
        int hour = (up_time / 1000 / 60 / 60) % 100;
        printf("%02d:%02d:%02d:%03d - total inserts: %lld, actual rate: %ld\n",
                hour, min, sec, fraction, GLOBALS.insert_count, GLOBALS.actual_rate);
    }
}
