#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

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

    if (false == read_ini_string(dict, "DB:DSN", p_globals->DSN)) {
        iniparser_freedict(dict);
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

    if (false == read_ini_string(dict, "DB:CREATE_TABLE_SQL", p_globals->CREATE_TABLE_SQL)) {
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

    if (false == read_ini_string(dict, "main:CSV_PATH", p_globals->CSV_PATH)) {
        iniparser_freedict(dict);
        return false;
    }

    iniparser_freedict(dict);
    return true;
}

bool init_globals(const char *config_file) {
    if (false == read_configs_from_ini(config_file, &GLOBALS))
        return false;

    return true;
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

bool GetLine(std::ifstream &fs, std::string &line) {
    line.clear();
    do{
        if(getline(fs, line)) {
            if(fs.good() && line.empty()){
                continue;
            }
            return true;
        } else {
            return false;
        }
    } while(true);
    return false;
}

std::string FormatTimeStr(unsigned long uTimeMs)
{
    char buff[64];
    sprintf(buff, "%2d:%02d:%03d",
        (uTimeMs/60000), (uTimeMs/1000) % 60, uTimeMs % 1000);
    return buff;
}

static unsigned int g_dwStart = ::GetTickCount();
std::string ElapsedTimeStr() {
    return FormatTimeStr(::GetTickCount() - g_dwStart);
}
