#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

#include <algorithm>
#include "hdb_utils.h"

using namespace hdb;
using namespace std;

namespace hdb {

static const char *TYPE_STRS[] = {
    "UNKNOWN",
    "TINYINT",
    "SMALLINT",
    "INTEGER",
    "BIGINT",
    "REAL",
    "DOUBLE",
    "DATE",
    "TIME",
    "TIMESTAMP",
    "SECONDDATE",
    "CHAR",
    "NCHAR",
    "VARCHAR",
    "NVARCHAR",
    "ALPHANUM",
    "SMALLDECIMAL",
    "DECIMAL",
    "DECIMAL_PS",
    "BINARY",
    "VARBINARY",
    "BLOB",
    "TEXT",
};

DATA_ATTR_T GenDataAttr(DATA_TYPE_T type, bool null_able, int param1, int param2)
{
    DATA_ATTR_T t;
    t.type = type;
    t.null_able = null_able;
    t.p = param1;
    t.s = param2;
    return t;
};

const char *DataTypeToStr(DATA_TYPE_T type)
{
    return ((unsigned int)type > T_MAX) ? TYPE_STRS[0] : TYPE_STRS[(unsigned int)type];
}

DATA_TYPE_T StrToDataType(const char *type_str)
{
    string typestr(type_str);
    StrToUpper(typestr);

    for (int i = 0; i <= T_MAX; i++) {
        if (typestr == TYPE_STRS[i]) {
            return (DATA_TYPE_T)i;
        }
    }

    static const struct {
        char *type_str;
        DATA_TYPE_T type;
    } MORE_TYPES[] = {
        {"DAYDATE",     T_DATE},
        {"SECONDTIME",  T_TIME},
        {"LONGDATE",    T_TIMESTAMP},
    };
    for (int i = 0; i < sizeof(MORE_TYPES)/sizeof(MORE_TYPES[0]); i++) {
        if (typestr == MORE_TYPES[i].type_str) {
            return MORE_TYPES[i].type;
        }
    }

    static const struct {
        char *type_str_sub;
        DATA_TYPE_T type;
    } MORE_TYPES_BEGIN_WITH[] = {
        {"DECIMAL(",    T_DECIMAL_PS},
        {"VARCHAR(",    T_VARCHAR},
        {"NVARCHAR(",   T_NVARCHAR},
        {"CHAR(",       T_CHAR},
        {"NCHAR(",      T_NCHAR},
        {"ALPHANUM(",   T_ALPHANUM},
    };
    for (int i = 0; i < sizeof(MORE_TYPES_BEGIN_WITH)/sizeof(MORE_TYPES_BEGIN_WITH[0]); i++) {
        if (typestr.find(MORE_TYPES_BEGIN_WITH[i].type_str_sub) == 0) {
            return MORE_TYPES_BEGIN_WITH[i].type;
        }
    }
    return T_UNKNOWN;
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
    if (7 == sscanf(s, "%d-%d-%d %d:%d:%d.%d", &year, &month, &day, &hour, &minute, &second, &fraction)) {
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

// "    too much\t   \tspace\t\t\t  " => "too much\t   \tspace"
std::string &TrimStr(std::string& str, const char *whitespace /*= " \t"*/)
{
    const size_t strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) {
        str.clear(); // no content
    } else {
        const size_t strEnd = str.find_last_not_of(whitespace);
        const size_t strRange = strEnd - strBegin + 1;
        str = str.substr(strBegin, strRange);
    }
    return str;
}

// "    too much\t   \tspace\t\t\t  " => "too-much-space" if fill is "-"
std::string &ReduceStr(std::string& str, const char *fill/*= " "*/, const char *whitespace /*=" \t"*/)
{
    // trim first
    TrimStr(str, whitespace);

    // replace sub ranges
    auto beginSpace = str.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = str.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        str.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + strlen(fill);
        beginSpace = str.find_first_of(whitespace, newStart);
    }

    return str;
}

void StrToUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void StrToLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void CsvLinePopulate(vector<string> &record, const char *line, char delimiter)
{
    int linepos = 0;
    bool inquotes = false;
    char c;
    int linemax = (int)strlen(line);
    string curstring;
    record.clear();

    while(line[linepos]!=0 && linepos < linemax)
    {
        c = line[linepos];

        if (!inquotes && curstring.length()==0 && c=='"')
        {
            //beginquotechar
            inquotes=true;
        }
        else if (inquotes && c=='"')
        {
            //quotechar
            if ( (linepos+1 <linemax) && (line[linepos+1]=='"') ) 
            {
                //encountered 2 double quotes in a row (resolves to 1 double quote)
                curstring.push_back(c);
                linepos++;
            }
            else
            {
                //endquotechar
                inquotes=false; 
            }
        }
        else if (!inquotes && c==delimiter)
        {
            //end of field
            record.push_back( curstring );
            curstring="";
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            record.push_back( curstring );
            return;
        }
        else
        {
            curstring.push_back(c);
        }
        linepos++;
    }
    record.push_back( curstring );
    return;
}

static void SplitParams(vector<string> &record, const char *params)
{
    CsvLinePopulate(record, params, ',');

    for (size_t i = 0; i < record.size(); i++) {
        if (record[i].find('(') != string::npos) {
            string upper(record[i]);
            StrToUpper(upper);
            if (upper.find("DECIMAL(") != string::npos) {
                if (i + 1 < record.size()) {
                    record[i] += ',';
                    record[i] += record[i+1];
                    record.erase(record.begin() + i + 1);
                }
            }
        }
    }
}

static void ParseParamStr(const string &param, int &p1, int &p2)
{
    DATA_TYPE_T type = StrToDataType(param.c_str());
    assert(T_UNKNOWN != type);

    if (type == T_DECIMAL_PS) {
        const char *s1 = strchr(param.c_str(), '(');
        if (s1) {
            s1++;
            p1 = atoi(s1);

            const char *s2 = strchr(s1, ',');
            if (s2) {
                p2 = atoi(s2+1);
            }
        }
    } else if (T_CHAR == type || T_NCHAR == type || T_VARCHAR == type || T_NVARCHAR == type || T_ALPHANUM == type) {
        const char *s1 = strchr(param.c_str(), '(');
        if (s1) {
            p1 = atoi(s1+1);
        }
    }
}

bool ParseTableFromSql(const char *create_sql, PARSED_TABLE_T &table, std::string &err_str)
{
    if (create_sql == NULL) {
        err_str = "Null pointer for passed in SQL!";
        return false;
    }

    PARSED_TABLE_T parsed_table;
    parsed_table.create_sql = create_sql;
    parsed_table.column = false;

    const char *s_begin = strchr(create_sql, '(');
    if (s_begin == NULL) {
        err_str = "Not found '(' in SQL passed in.";
        return false;
    }

    {
        string create(create_sql);
        create.erase(s_begin - 1 - create_sql);
        ReduceStr(create); // Now, e.g., create = CREATE COLUMN TABLE "I078212"."GPS29"
        vector<string> subs;
        CsvLinePopulate(subs, create.c_str(), ' ');

        size_t sub_count = subs.size();
        if (sub_count < 3) {
            err_str = "Too few items in \"" + create + '\"';
            return false;
        }
        if (sub_count > 3 && !_stricmp(subs[1].c_str(), "COLUMN")) {
            parsed_table.column = true;
        }

        {
            vector<string> strs;
            CsvLinePopulate(strs, subs[sub_count - 1].c_str(), '.');
            if (strs.size() == 1) {
                parsed_table.schema.clear();
                parsed_table.table_name = strs[0];
            } else if (strs.size() == 2) {
                parsed_table.schema = strs[0];
                parsed_table.table_name = strs[1];
            } else {
                err_str = "Too few items in \"" + create + '\"';
                return false;
            }
            StrToUpper(parsed_table.schema);
            StrToUpper(parsed_table.table_name);
        }
    }

    s_begin++;
    string str(s_begin);
    // strip the sub-string after "PARTITION BY"
    size_t partition_by_pos = str.find("PARTITION BY");
    if (partition_by_pos != string::npos) {
        str.resize(partition_by_pos);
    }
    const char *s_end = strrchr(str.c_str(), ')');
    if (!s_end) {
        err_str = "Not found ')' in SQL passed in.";
        return false;
    }
    str.erase(s_end - str.c_str());

    SplitParams(parsed_table.col_strs, str.c_str());
    size_t col_count = parsed_table.col_strs.size();
    if (col_count == 0) {
        return false;
    }
    for (size_t i = 0; i < col_count; i++) {
        string &col_str = parsed_table.col_strs[i];
        ReduceStr(col_str);

        vector<string> subs;
        CsvLinePopulate(subs, col_str.c_str(), ' ');
        if (subs.size() < 2) {
            err_str = "Error in parsing: " + col_str + ": too few items!";
            return false;
        }
        parsed_table.col_names.push_back(subs[0]);

        StrToUpper(subs[1]);
        parsed_table.col_str_types.push_back(subs[1]);

        int p1 = 0, p2 = 0;
        DATA_TYPE_T type = StrToDataType(subs[1].c_str());
        if (T_UNKNOWN == type) {
            err_str = "Unknown type: " + subs[1];
            return false;
        }
        parsed_table.col_types.push_back(type);

        if (T_DECIMAL_PS == type || T_CHAR == type || T_NCHAR == type || 
            T_VARCHAR == type || T_NVARCHAR == type || T_ALPHANUM == type) {
            ParseParamStr(subs[1], p1, p2);
        }

        // check null-able for the parameter
        bool null_able = true;
        if (subs.size() > 2) {
            string rest;
            for (size_t i = 2; i < subs.size(); i++) {
                rest += subs[i];
                rest += ' ';
            }
            StrToUpper(rest);
            if (NULL != strstr(rest.c_str(), "NOT NULL ")) {
                null_able = false;
            }
        }
        parsed_table.col_attrs.push_back(GenDataAttr(type, null_able, p1, p2));
    }

    table = parsed_table;
    return true;
}

void UnImplemented()
{
    *(int *)0 = 0; // to crash!
}

} // end of namespace hdb
