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
#include "hdb_odbc.h"

namespace hdb {

////////////////////////////////////////////////////////////////////////////////////////////////////
class BaseColumn {
public:
    BaseColumn(const char *col_name, const DATA_ATTR_T &attr) {
        SetColName(col_name);
        memcpy(&mDataAttr, &attr, sizeof(DATA_ATTR_T));
    };
    virtual ~BaseColumn() {};

    const DATA_ATTR_T &GetDataAttr() const {
        return mDataAttr;
    };
    const bool NullAble() const {
        return mDataAttr.null_able;
    };
    virtual void Reserve(size_t count) = 0;
    virtual size_t GetCount() const = 0;
    const char *GetColName() const {
        return mColName.c_str();
    };
    void SetColName(const char *name) {
        mColName = (name != NULL) ? name : "";
    };
    void CopyFrom(const BaseColumn &col) {
        mDataAttr = col.mDataAttr;
        mColName = col.mColName;
    };
    virtual void *GetData() = 0;
    virtual const void *GetData() const = 0;
    virtual void *GetStrLenOrIndVec() = 0;
    virtual const void *GetStrLenOrIndVec() const = 0;
    virtual void GenerateFakeData(size_t count) = 0;
    virtual SQLRETURN BindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar) const = 0;
    virtual bool AddFromStr(const char *str) = 0;
    virtual void RemoveAllRows() = 0;

protected:
    DATA_ATTR_T mDataAttr;
    std::string mColName;
};

template<class T, DATA_TYPE_T data_type>
class ColT : public BaseColumn
{
public:
    ColT(const char *col_name = NULL, bool null_able = true)
        : BaseColumn(col_name, GenDataAttr(data_type, null_able, 0, 0))
    {};
    ColT(const char *col_name, DATA_ATTR_T attr) 
        : BaseColumn(col_name, attr)
    {
        mDataAttr.type = data_type;
    };
    virtual ~ColT() {};

    virtual void Reserve(size_t count) {
        mDataVec.reserve(count);
        if (NullAble()) {
            mStrLenOrIndVec.reserve(count);
        }
    };
    virtual size_t GetCount() const {
        return mDataVec.size();
    };
    virtual void *GetData() {
        return mDataVec.data();
    };
    virtual const void *GetData() const {
        return mDataVec.data();
    };
    virtual void *GetStrLenOrIndVec() {
        return mStrLenOrIndVec.data();
    };
    virtual const void *GetStrLenOrIndVec() const {
        return mStrLenOrIndVec.data();
    };
    virtual void GenerateFakeData(size_t count) {
        RemoveAllRows();
        Reserve(count);
        // Based on OO design, it is not recommended to use switch-case here. The recommended way is
        // to implement all the virtual function in sub-classes, which is too tedious and will generate
        // too much code.
        switch(mDataAttr.type) {
        case T_TINYINT:
            {
                TinyIntCol *pcol = (TinyIntCol *)this;
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
        case T_FLOAT:
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
            assert(false);
            break;
        case T_CHAR:
        case T_NCHAR:
        case T_VARCHAR:
        case T_NVARCHAR:
        case T_ALPHANUM:
            assert(false); // should not reach here!
            break;
        case T_SMALLDECIMAL:
        case T_DECIMAL:
            {
                DoubleCol *pcol = (DoubleCol *)this;
                for (size_t i = 0; i < count; i++) {
			        double t = ((double) rand() / RAND_MAX * 999999999);
                    pcol->PushBack(t);
                }
            }
            break;
        case T_DECIMAL_PS:
            {
                DoubleCol *pcol = (DoubleCol *)this;
                for (size_t i = 0; i < count; i++) {
			        double t = ((double) rand() / RAND_MAX * 9999999);
                    pcol->PushBack(t);
                }
            }
            break;
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
    virtual SQLRETURN BindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar) const {
        return SqlBindInParam(hstmt, ipar, *this);
    };
    virtual bool AddFromStr(const char *str) {
        T value;
        if (NullAble()) {
            bool is_null = (*str == '\0');
            mStrLenOrIndVec.push_back(is_null ? SQL_NULL_DATA : SQL_NTS);
            if (is_null) {
                memset(&value, 0, sizeof(T));
            } else {
                if (false == StrToValue(str, value)) {
                    return false;
                }
            }
        } else {
            if (false == StrToValue(str, value)) {
                return false;
            }
        }
        mDataVec.push_back(value);
        return true;
    };
    virtual void RemoveAllRows() {
        mDataVec.clear();
        mStrLenOrIndVec.clear();
    };

public:
    void CopyFrom(ColT<T, data_type> &col) {
        BaseColumn::CopyFrom(col);
        mDataVec = col.mDataVec;
        mStrLenOrIndVec = col.mStrLenOrIndVec;
    };
    void PushBack(const T &val) {
        assert(mDataAttr.type != T_CHAR && mDataAttr.type != T_NCHAR && mDataAttr.type != T_VARCHAR && mDataAttr.type != T_NVARCHAR);
        mDataVec.push_back(val);
        mStrLenOrIndVec.push_back(0);
    };

protected:
    std::vector<T> mDataVec;
    std::vector<SQLLEN> mStrLenOrIndVec;
};

template<class T, DATA_TYPE_T data_type>
class CharColT : public ColT<T, data_type>
{
public:
    CharColT(const char *col_name, int n, bool null_able = true)
        : ColT<T, data_type>(col_name, GenDataAttr(data_type, null_able, n, 0))
    {
    };
    virtual ~CharColT() {};
    virtual void Reserve(size_t count) {
        mDataVec.reserve(count * (mDataAttr.a + 1));
        mStrLenOrIndVec.reserve(count);
    };
    virtual size_t GetCount() const {
        return mStrLenOrIndVec.size();
    }
    virtual SQLRETURN BindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar) const {
        return SqlBindInParam(hstmt, ipar, *this);
    };
    virtual bool AddFromStr(const char *str) {
        mStrLenOrIndVec.push_back((NullAble() && *str == '\0') ? SQL_NULL_DATA : SQL_NTS);
        size_t len = mDataVec.size();
        mDataVec.resize(len +  mDataAttr.a + 1);
        if (T_NCHAR == mDataAttr.type || T_NVARCHAR == mDataAttr.type) {
            wstring wstr = StrToWStr(str);
#ifdef _WIN32
            wcsncpy_s((SQLWCHAR *)mDataVec.data() + len, mDataAttr.a + 1, (SQLWCHAR *)wstr.c_str(), mDataAttr.a);
#else
            UnImplemented();
#endif
        } else {
#ifdef _WIN32
            strncpy_s((char *)mDataVec.data() + len, mDataAttr.a + 1, str, mDataAttr.a);
#else
            strncpy((char *)mDataVec.data() + len, str, mDataAttr.a);
#endif
        }
        return true;
    };
    virtual void GenerateFakeData(size_t count) {
        RemoveAllRows();
        Reserve(count);
        switch(mDataAttr.type) {
        case T_CHAR:
        case T_VARCHAR:
        case T_ALPHANUM:
            {
                string buff;
                buff.resize(mDataAttr.a + 1);
                for (size_t i = 0; i < count; i++) {
			        long t = (long) ((double) rand() / RAND_MAX * 999999999);
#ifdef _WIN32
			        _snprintf_s((char*)buff.data(), buff.size(), mDataAttr.a, "A%09ld", t);
#else
                    snprintf((char*)buff.data(), mDataAttr.a, "A%09ld", t);
#endif
                    AddFromStr((const char *)buff.c_str());
                }
            }
            break;
        case T_NCHAR:
        case T_NVARCHAR:
            {
                wstring buff;
                buff.resize(mDataAttr.a + 1);
                for (size_t i = 0; i < count; i++) {
			        long t = (long) ((double) rand() / RAND_MAX * 999999999);
#ifdef _WIN32
			        _snwprintf_s((SQLWCHAR*)buff.data(), buff.size(), mDataAttr.a, L"N%09ld", t);
#else
                    UnImplemented();
#endif
                    PushBack((T*)buff.c_str());
                }
            }
            break;
        default:
            assert(false); // should not reach here!
            break;
        };
    };

public:
    void PushBack(const T *str) {
        if (T_NCHAR == mDataAttr.type || T_NVARCHAR == mDataAttr.type) {
            mStrLenOrIndVec.push_back((NullAble() && *str == '\0') ? SQL_NULL_DATA : SQL_NTS);
            size_t len = mDataVec.size();
            mDataVec.resize(len +  mDataAttr.a + 1);
#ifdef _WIN32
            wcsncpy_s((SQLWCHAR *)mDataVec.data() + len, mDataAttr.a + 1, (SQLWCHAR *)str, mDataAttr.a);
#else
            UnImplemented();
#endif
        } else {
            AddFromStr((const char *)str);
        }
    };
};

typedef ColT<char, T_TINYINT> TinyIntCol;
typedef ColT<short, T_SMALLINT> SmallIntCol;
typedef ColT<int, T_INTEGER> IntCol;
typedef ColT<SQLBIGINT, T_BIGINT> BigIntCol;
typedef ColT<float, T_REAL> RealCol;
typedef ColT<double, T_DOUBLE> DoubleCol;
typedef ColT<double, T_FLOAT> FloatCol;
typedef ColT<SQL_DATE_STRUCT, T_DATE> DateCol;
typedef ColT<SQL_TIME_STRUCT, T_TIME> TimeCol;
typedef ColT<SQL_TIMESTAMP_STRUCT, T_TIMESTAMP> TimeStampCol;
// T_SECONDDATE ?
typedef CharColT<SQLCHAR, T_CHAR> CharCol;
typedef CharColT<SQLWCHAR, T_NCHAR > NCharCol;
typedef CharColT<SQLVARCHAR, T_VARCHAR> VarCharCol;
typedef CharColT<SQLWCHAR, T_NVARCHAR> NVarCharCol;
typedef CharColT<SQLCHAR, T_ALPHANUM> AlphaNumCol;
typedef ColT<double, T_DECIMAL> DecimalCol;
typedef ColT<double, T_SMALLDECIMAL> SmallDecimalCol;
typedef ColT<double, T_DECIMAL_PS> DecimalPsCol; // NOTE: map double to decimal may not be precise!
// T_BINARY ?
// T_VARBINARY ?
// T_BLOB ?
// T_TEXT ?

// Dummy functions for CharColT derived classes, do not use them!
static inline SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQLCHAR, T_CHAR> &col) {return 0;};
static inline SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQLWCHAR, T_NCHAR> &col) {return 0;};
static inline SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQLVARCHAR, T_VARCHAR> &col) {return 0;};
static inline SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQLWCHAR, T_NVARCHAR> &col) {return 0;};
static inline SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQLCHAR, T_ALPHANUM> &col) {return 0;};
static inline bool StrToValue(const char *s, SQLCHAR &v) {return 0;};
static inline bool StrToValue(const char *s, SQLWCHAR &v) {return 0;};

////////////////////////////////////////////////////////////////////////////////////////////////////

class ColRecords {
public:
    ColRecords() : mRowCount(0) {
    };
    virtual ~ColRecords() {
        ClearAllCols();
    };

public:
    void ClearAllCols() {
        mRowCount = 0;
        for (size_t i = 0; i < mPtrCols.size(); i++) {
            delete mPtrCols[i];
        }
        mPtrCols.clear();
    };
    void ClearAllRows() {
        mRowCount = 0;
        for (size_t i = 0; i < mPtrCols.size(); i++) {
            mPtrCols[i]->RemoveAllRows();
        }
    };
    void Reserve(size_t count) {
        for (size_t i = 0; i < mPtrCols.size(); i++) {
            mPtrCols[i]->Reserve(count);
        }
    };
    const char *GetErrStr() const {
        return mErrStr.c_str();
    };
    size_t GetRowCount() const {
        return mRowCount;
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
    bool AddCol(const char *col_name, DATA_TYPE_T type, bool null_able = true) {
        return AddCol(col_name, GenDataAttr(type, null_able, 0, 0));
    };
    bool AddColFixedChar(const char *col_name, DATA_TYPE_T type, unsigned char num, bool null_able = false) {
        assert(type == T_CHAR || type == T_NCHAR);
        return AddCol(col_name, GenDataAttr(type, null_able, num, 0));
    };
    bool AddColDecimalPs(const char *col_name, unsigned char p, unsigned char s, bool null_able = false) {
        return AddCol(col_name, GenDataAttr(T_DECIMAL_PS, null_able, p, s));
    };
    bool AddColsFromCreateSql(const char *create_sql);

    SQLRETURN BindAllInColumns(SQLHSTMT hstmt) const;
    bool AddRow(const char *line, char delimiter = ','); // one line of CSV
    int AddRows(std::ifstream &is_csv, int num, char delimiter = ',');
    void GenerateFakeData(size_t row_count);

protected:
    bool AddCol(const char *col_name, const DATA_ATTR_T &attr);

protected:
    size_t mRowCount;
    std::vector<BaseColumn *> mPtrCols;
    std::string mErrStr;
};


}// namespace hdb

#endif // _HDB_COLUMNS_H
