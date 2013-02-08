#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

#include "hdb_utils.h"

using namespace hdb;
using namespace std;

namespace hdb {

DATA_ATTR_T GenDataAttr(DATA_TYPE_T type, bool null_able, int param1, int param2)
{
    DATA_ATTR_T t;
    t.type = type;
    t.null_able = null_able;
    t.p = param1;
    t.s = param2;
    return t;
};

const char *DateTypeToStr(DATA_TYPE_T type)
{
    switch(type)
    {
    case T_TYNYINT: return "TYNYINT";
    case T_SMALLINT: return "SMALLINT";
    case T_INTEGER: return "INTEGER";
    case T_BIGINT: return "BIGINT";
    case T_REAL: return "REAL";
    case T_DOUBLE: return "DOUBLE";
    case T_DATE: return "DATE";
    case T_TIME: return "TIME";
    case T_TIMESTAMP: return "TIMESTAMP";
    case T_SECONDDATE: return "SECONDDATE";
    case T_CHAR: return "CHAR";
    case T_NCHAR: return "NCHAR";
    case T_VARCHAR: return "VARCHAR";
    case T_NVARCHAR: return "NVARCHAR";
    case T_SMALLDECIMAL: return "SMALLDECIMAL";
    case T_DECIMAL: return "DECIMAL";
    case T_DECIMAL_PS: return "DECIMAL_PS";
    case T_BINARY: return "BINARY";
    case T_VARBINARY: return "VARBINARY";
    case T_BLOB: return "BLOB";
    case T_TEXT: return "TEXT";
    default: return "UNKNOWN";
    }
}

void GetCurTm(struct tm &stm) {
	time_t t;
	time(&t);
#ifdef _WIN32
	localtime_s(&stm, &t);
#else
	localtime_r(&t, &stm);
#endif
};

void GetCurTimestamp(SQL_TIMESTAMP_STRUCT &st) {
	struct tm stm;
    GetCurTm(stm);
	st.year = stm.tm_year + 1900;
	st.month = stm.tm_mon + 1;
	st.day = stm.tm_mday;
	st.hour = stm.tm_hour;
	st.minute = stm.tm_min;
	st.second = stm.tm_sec;
	st.fraction = rand();
};

void GetCurDate(SQL_DATE_STRUCT &date) {
	struct tm stm;
    GetCurTm(stm);
	date.year = stm.tm_year + 1900;
	date.month = stm.tm_mon + 1;
	date.day = stm.tm_mday;
};

void GetCurTime(SQL_TIME_STRUCT &time) {
	struct tm stm;
    GetCurTm(stm);
	time.hour = stm.tm_hour;
	time.minute = stm.tm_min;
	time.second = stm.tm_sec;
};

bool StrToValue(const char *s, SQL_DATE_STRUCT &v)
{
    if (s == NULL || *s == '\0') return false;
    int year, month, day;
    if (3 == sscanf(s, "%d-%d-%d", &year, &month, &day)) {
        v.year = year;
        v.month = month;
        v.day = day;
        return true;
    }
    return false;
}

bool StrToValue(const char *s, SQL_TIME_STRUCT &v)
{
    if (s == NULL || *s == '\0') return false;
    int hour, minute, second;
    if (3 == sscanf(s, "%d:%d:%d", &hour, &minute, &second)) {
        v.hour = hour;
        v.minute = minute;
        v.second = second;
        return true;
    }
    return false;
}

bool StrToValue(const char *s, SQL_TIMESTAMP_STRUCT &v)
{
    if (s == NULL || *s == '\0') return false;
    int year, month, day, hour, minute, second, fraction;
    if (7 == sscanf(s, "%d-%d-%d %d:%d:%d:%d", &year, &month, &day, &hour, &minute, &second, &fraction)) {
        v.year = year;
        v.month = month;
        v.day = day;
        v.hour = hour;
        v.minute = minute;
        v.second = second;
        v.fraction = fraction;
        return true;
    }
    return false;
}

} // end of namespace hdb
