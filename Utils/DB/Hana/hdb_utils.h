#ifndef _H_UTIL_H
#define _H_UTIL_H

#include <memory.h>
#include <vector>
#include <assert.h>
#ifdef _WIN32
#include <windows.h> // required by sqlext.h for WIN32
#endif
#include "sqlext.h"

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

static inline
DATA_ATTR_T GenDataAttr(DATA_TYPE_T type, bool null_able, unsigned char param1, unsigned char param2)
{
    DATA_ATTR_T t;
    t.type = type;
    t.null_able = null_able;
    t.p = param1;
    t.s = param2;
    return t;
};

static inline
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

////////////////////////////////////////////////////////////////////////////////////////////////////
class BaseColumn {
public:
    BaseColumn(const char *col_name, const DATA_ATTR_T &attr) {
        memcpy(&mDataAttr, &attr, sizeof(DATA_ATTR_T));
    };
    virtual ~BaseColumn() {};

    const DATA_ATTR_T &GetDataAttr() const {
        return mDataAttr;
    };
    const bool NullAble() const {
        return mDataAttr.null_able;
    };
    virtual void Reserve(size_t count) {
        if (NullAble()) {
            mNullVec.reserve(count);
        }
    };
    virtual size_t GetCount() = 0;
    const char *GetColName() const {
        return mColName.c_str();
    };
    void CopyFrom(const BaseColumn &col) {
        mDataAttr = col.mDataAttr;
        mColName = col.mColName;
        mNullVec = col.mNullVec;
    };
    virtual SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar) = 0;

protected:
    DATA_ATTR_T mDataAttr;
    std::string mColName;
    std::vector<bool> mNullVec;
};

template<class T, DATA_TYPE_T data_type>
class ColT : public BaseColumn
{
public:
    ColT(const char *col_name, bool null_able = false) 
        : BaseColumn(col_name, GenDataAttr(data_type, null_able, 0, 0))
    {};
    ColT(const char *col_name, DATA_ATTR_T attr) 
        : BaseColumn(col_name, attr)
    {};
    virtual ~ColT() {};
    virtual void Reserve(size_t count) {
        BaseColumn::Reserve(count);
        mDataVec.reserve(count);
    };
    virtual size_t GetCount() {
        return mDataVec.size();
    };
    void CopyFrom(ColT<T, data_type> &col) {
        BaseColumn::CopyFrom(col);
        mDataVec = col.mDataVec;
    };
    virtual SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar) {
        SQLRETURN rc;
        switch(mDataAttr.type) {
        case T_TYNYINT:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_CHAR,
                1, 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_SMALLINT:
            assert(false);
            break;
        case T_INTEGER:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                4, 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_BIGINT:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
                sizeof(SQLBIGINT), 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_REAL:
            assert(false);
            break;
        case T_DOUBLE:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                3, 6, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_DATE:
            assert(false);
            break;
        case T_TIME:
            assert(false);
            break;
        case T_TIMESTAMP:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
                sizeof(SQL_TIMESTAMP_STRUCT), 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_SECONDDATE:
        case T_CHAR:
        case T_NCHAR:
        case T_VARCHAR:
        case T_NVARCHAR:
        case T_SMALLDECIMAL:
        case T_DECIMAL:
        case T_DECIMAL_PS:
        case T_BINARY:
        case T_VARBINARY:
        case T_BLOB:
        case T_TEXT:
            // TODO: to implement later
        default:
            assert(false);
            return false;
        };
        return rc;
    };

protected:
    std::vector<T> mDataVec;
};

typedef ColT<char, T_TYNYINT> TynyIntCol;
typedef ColT<short, T_SMALLINT> SmallIntCol;
typedef ColT<int, T_INTEGER> IntCol;
typedef ColT<SQLBIGINT, T_BIGINT> BigIntCol;
typedef ColT<float, T_REAL> RealCol;
typedef ColT<double, T_DOUBLE> DoubleCol;
typedef ColT<SQL_DATE_STRUCT, T_DATE> DateCol;
typedef ColT<SQL_TIME_STRUCT, T_TIME> TimeCol;
typedef ColT<SQL_TIMESTAMP_STRUCT, T_TIMESTAMP> TimeStampCol;
// T_SECONDDATE ?
// T_CHAR
// T_NCHAR
// T_VARCHAR
// T_NVARCHAR
// T_SMALLDECIMAL
// T_DECIMAL
// T_DECIMAL_PS
// T_BINARY
// T_VARBINARY
// T_BLOB
// T_TEXT

////////////////////////////////////////////////////////////////////////////////////////////////////
class ColRecords {
public:
    ColRecords() {
        mCount = 0;
    };
    virtual ~ColRecords() {
        Clear();
    };

public:
    void Clear() {
        for (size_t i = 0; i < mPtrCols.size(); i++) {
            delete mPtrCols[i];
        }
        mPtrCols.clear();
    };
    void Reserve(size_t count) {
        for (size_t i = 0; i < mPtrCols.size(); i++) {
            mPtrCols[i]->Reserve(count);
        }
    };
    size_t GetRowCount() const {
        return mCount;
    };
    size_t GetColCount() const {
        return mPtrCols.size();
    };
    BaseColumn *GetColumn(size_t index) {
        return mPtrCols[index];
    };
    bool AddCol(const char *col_name, DATA_TYPE_T type, bool null_able = false) {
        return AddCol(col_name, GenDataAttr(type, null_able, 0, 0));
    };
    bool AddColFixedChar(const char *col_name, DATA_TYPE_T type, unsigned char num, bool null_able = false) {
        assert(type == T_CHAR || type == T_NCHAR);
        return AddCol(col_name, GenDataAttr(type, null_able, num, 0));
    };
    bool AddDecimalPs(const char *col_name, unsigned char p, unsigned char s, bool null_able = false) {
        return AddCol(col_name, GenDataAttr(T_DECIMAL_PS, null_able, p, s));
    };

protected:
    inline bool AddCol(const char *col_name, const DATA_ATTR_T &col_type);

protected:
    size_t mCount;
    std::vector<BaseColumn *> mPtrCols;
};

bool ColRecords::AddCol(const char *col_name, const DATA_ATTR_T &col_type)
{
    assert(col_name != NULL);
    BaseColumn *pCol = NULL;

    switch(col_type.type) {
    case T_TYNYINT:
        pCol = new TynyIntCol(col_name, col_type.null_able);
        break;
    case T_SMALLINT:
        pCol = new SmallIntCol(col_name, col_type.null_able);
        break;
    case T_INTEGER:
        pCol = new IntCol(col_name, col_type.null_able);
        break;
    case T_BIGINT:
        pCol = new BigIntCol(col_name, col_type.null_able);
        break;
    case T_REAL:
        pCol = new RealCol(col_name, col_type.null_able);
        break;
    case T_DOUBLE:
        pCol = new DoubleCol(col_name, col_type.null_able);
        break;
    case T_DATE:
        pCol = new DateCol(col_name, col_type.null_able);
        break;
    case T_TIME:
        pCol = new TimeCol(col_name, col_type.null_able);
        break;
    case T_TIMESTAMP:
        pCol = new TimeStampCol(col_name, col_type.null_able);
        break;
    case T_SECONDDATE:
    case T_CHAR:
    case T_NCHAR:
    case T_VARCHAR:
    case T_NVARCHAR:
    case T_SMALLDECIMAL:
    case T_DECIMAL:
    case T_DECIMAL_PS:
    case T_BINARY:
    case T_VARBINARY:
    case T_BLOB:
    case T_TEXT:
        // TODO: to implement later
    default:
        assert(false);
        return false;
    };

    mPtrCols.push_back(pCol);
    return true;
}

}// namespace hdb

#endif // _H_UTIL_H
