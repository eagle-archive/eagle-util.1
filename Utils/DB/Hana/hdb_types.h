#ifndef _HDB_TYPES_H
#define _HDB_TYPES_H

namespace hdb {

typedef enum
{
    T_UNKNOWN = 0,
    T_TYNYINT = 1,
    T_SMALLINT,
    T_INTEGER,
    T_BIGINT,
    T_REAL,
    T_DOUBLE,
    T_DATE,
    T_TIME,
    T_TIMESTAMP,
    T_SECONDDATE,
    T_CHAR,     // CHAR(a)
    T_NCHAR,    // NCHAR(a)
    T_VARCHAR,
    T_NVARCHAR,
    T_SMALLDECIMAL,
    T_DECIMAL,
    T_DECIMAL_PS,  // DECIMAL(p,s)
    T_BINARY,
    T_VARBINARY,
    T_BLOB,
    T_TEXT,
} DATA_TYPE_T;

typedef struct {
    unsigned char type; // DATA_TYPE_T
    union {
        unsigned char a; // for CHAR(a) and NCHAR(a)
        unsigned char p; // p,s for DECIMAL(p,s)
    };
    unsigned char s;
    bool null_able;
} DATA_ATTR_T;


DATA_ATTR_T GenDataAttr(DATA_TYPE_T type, bool null_able, int param1, int param2);
const char *DateTypeToStr(DATA_TYPE_T type);

static inline bool StrToValue(const char *s, char &v) {
    if (s == NULL || *s == '\0') return false;
    v = (char)atoi(s);
    return true;
}
static inline bool StrToValue(const char *s, short &v) {
    if (s == NULL || *s == '\0') return false;
    v = (short)atoi(s);
    return true;
}
static inline bool StrToValue(const char *s, int &v) {
    if (s == NULL || *s == '\0') return false;
    v = atoi(s);
    return true;
}
static inline bool StrToValue(const char *s, SQLBIGINT &v) {
    if (s == NULL || *s == '\0') return false;
    v = _atoi64(s);
    return true;
}
static inline bool StrToValue(const char *s, float &v) {
    if (s == NULL || *s == '\0') return false;
    v = (float)atof(s);
    return true;
}
static inline bool StrToValue(const char *s, double &v) {
    if (s == NULL || *s == '\0') return false;
    v = atof(s);
    return true;
}
bool StrToValue(const char *s, SQL_DATE_STRUCT &v);
bool StrToValue(const char *s, SQL_TIME_STRUCT &v);
bool StrToValue(const char *s, SQL_TIMESTAMP_STRUCT &v);

}// namespace hdb

#endif // _HDB_TYPES_H
