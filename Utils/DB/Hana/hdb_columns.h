#ifndef _HDB_COLUMNS_H
#define _HDB_COLUMNS_H

#include <memory.h>
#include <vector>
#include <assert.h>
#ifdef _WIN32
#include <windows.h> // required by sqlext.h for WIN32
#endif
#include <sqlext.h>
#include <time.h>
#include "hdb_types.h"

namespace hdb {

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
    virtual void *GetData() = 0;
    virtual void GenerateFakeData(size_t count) = 0;
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
    void PushBack(const T &val) {
        mDataVec.push_back(val);
    };
    virtual void *GetData() {
        return mDataVec.data();
    };
    virtual void GenerateFakeData(size_t count) {
        mDataVec.clear();
        mDataVec.reserve(count);
        // Based on OO design, it is not recommended to use switch-case here. The recommended way is
        // to implement all the virtual function in sub-classes, which is too tedious and will generate
        // too much code.
        switch(mDataAttr.type) {
        case T_TYNYINT:
            {
                TynyIntCol *pcol = (TynyIntCol *)this;
                for (size_t i = 0; i < count; i++) {
                    pcol->PushBack((char)(rand() % 128));
                }
            }
            break;
        case T_SMALLINT:
            {
                SmallIntCol *pcol = (SmallIntCol *)this;
                for (size_t i = 0; i < count; i++) {
                    pcol->PushBack((short)(rand() % 32768));
                }
            }
            break;
        case T_INTEGER:
            {
                IntCol *pcol = (IntCol *)this;
                for (size_t i = 0; i < count; i++) {
                    pcol->PushBack(rand());
                }
            }
            break;
        case T_BIGINT:
            {
                BigIntCol *pcol = (BigIntCol *)this;
                for (size_t i = 0; i < count; i++) {
			        long long t = (long long) ((double) rand() / RAND_MAX * 99999999);
                    pcol->PushBack(t);
                }
            }
            break;
        case T_REAL:
            {
                RealCol *pcol = (RealCol *)this;
                for (size_t i = 0; i < count; i++) {
			        float t = ((float) rand() / RAND_MAX * 99999);
                    pcol->PushBack(t);
                }
            }
            break;
        case T_DOUBLE:
            {
                DoubleCol *pcol = (DoubleCol *)this;
                for (size_t i = 0; i < count; i++) {
			        double t = ((double) rand() / RAND_MAX * 9999999);
                    pcol->PushBack(t);
                }
            }
            break;
        case T_DATE:
            {
                DateCol *pcol = (DateCol *)this;
                for (size_t i = 0; i < count; i++) {
                    SQL_DATE_STRUCT date;
                    GetCurDate(date);
                    pcol->PushBack(date);
                }
            }
            break;
        case T_TIME:
            {
                TimeCol *pcol = (TimeCol *)this;
                for (size_t i = 0; i < count; i++) {
                    SQL_TIME_STRUCT time;
                    GetCurTime(time);
                    pcol->PushBack(time);
                }
            }
            break;
        case T_TIMESTAMP:
            {
                TimeStampCol *pcol = (TimeStampCol *)this;
                for (size_t i = 0; i < count; i++) {
                    SQL_TIMESTAMP_STRUCT st;
                    GetCurTimestamp(st);
                    pcol->PushBack(st);
                }
            }
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
            return;
        };
    };

    virtual SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar) {
        SQLRETURN rc;
        switch(mDataAttr.type) {
        case T_TYNYINT:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT,
                0, 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_SMALLINT:
            assert(false);
            break;
        case T_INTEGER:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                0, 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_BIGINT:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
                0, 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_REAL:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_REAL,
                0, 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_DOUBLE:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                0, 0, (SQLPOINTER)mDataVec.data(), 0, 0);
            break;
        case T_DATE:
            assert(false);
            break;
        case T_TIME:
            assert(false);
            break;
        case T_TIMESTAMP:
            rc = SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
                0, 0, (SQLPOINTER)mDataVec.data(), 0, 0);
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
    std::vector<BaseColumn *> &GetColumns() {
        return mPtrCols;
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
    SQLRETURN SqlBindAllColumns(SQLHSTMT hstmt);
    bool AddRow(const char *line); // one line of CSV

protected:
    bool AddCol(const char *col_name, const DATA_ATTR_T &col_type);

protected:
    size_t mCount;
    std::vector<BaseColumn *> mPtrCols;
};


}// namespace hdb

#endif // _HDB_COLUMNS_H
