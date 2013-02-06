#ifndef _H_UTIL_H
#define _H_UTIL_H

#include <memory.h>
#include <vector>
#include <assert.h>

namespace hdb {

typedef enum
{
    T_UNKNOWN = 0,
    T_TYNYINT = 1,
    T_SMALLINT = 2,
    T_INTEGER = 3,
    T_BIGINT = 4,
    T_REAL = 5,
    T_DOUBLE = 6,
    T_DATE = 7,
    T_TIME = 8,
    T_TIMESTAMP = 9,
    T_SECONDDATE = 10,
    T_CHAR = 11,    // CHAR(a)
    T_NCHAR = 12,   // NCHAR(a)
    T_VARCHAR = 13,
    T_NVARCHAR = 14,
    T_SMALLDECIMAL = 15,
    T_DECIMAL = 16,
    T_DECIMAL_PS = 17,  // DECIMAL(p,s)
    T_BINARY = 18,
    T_VARBINARY = 19,
    T_BLOB = 20,
    T_TEXT = 21,
} DATA_TYPE_T;

typedef struct {
    unsigned char type; // DATA_TYPE_T
    union {
        unsigned char a; // for CHAR(a) and NCHAR(a)
        unsigned char p; // p,s for DECIMAL(p,s)
    };
    unsigned char s;
    bool null_able;
} DATA_TYPE_S;

DATA_TYPE_S GenDataType(DATA_TYPE_T type, bool null_able, unsigned char p1, unsigned char p2);

class BaseColumn {
public:
    virtual ~BaseColumn() {};

    const DATA_TYPE_S &GetDataType() {
        return mDataType;
    };
    BaseColumn(const DATA_TYPE_S &type, const char *col_name) {
        memcpy(&mDataType, &type, sizeof(DATA_TYPE_S));
    };
    const bool NullAble() const {
        return mDataType.null_able;
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

protected:
    DATA_TYPE_S mDataType;
    std::vector<bool> mNullVec;
    std::string mColName;
};

template<class T, DATA_TYPE_T data_t>
class ColT : public BaseColumn
{
public:
    ColT(const char *col_name, bool null_able = false) 
        : BaseColumn(GenDataType(data_t, null_able, 0, 0), col_name)
    {};
    virtual ~ColT() {
    };
    virtual void Reserve(size_t count) {
        BaseColumn::Reserve(count);
        mDataVec.reserve(count);
    };
    virtual size_t GetCount() {
        return mDataVec.size();
    };

protected:
    std::vector<T> mDataVec;
};

typedef ColT<char, T_TYNYINT> TynyIntCol;
typedef ColT<short, T_SMALLINT> SmallIntCol;
typedef ColT<int, T_INTEGER> IntCol;
typedef ColT<double, T_DOUBLE> DoubleCol;
typedef ColT<float, T_REAL> RealCol;


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
    size_t GetCount() const {
        return mCount;
    };
    bool AddCol(const char *col_name, const DATA_TYPE_S &col_type);
    bool AddCol(const char *col_name, DATA_TYPE_T type, bool null_able = false) {
        return AddCol(col_name, GenDataType(type, null_able, 0, 0));
    };
    bool AddColFixedChar(const char *col_name, DATA_TYPE_T type, unsigned char num, bool null_able = false) {
        assert(type == T_CHAR || type == T_NCHAR);
        return AddCol(col_name, GenDataType(type, null_able, num, 0));
    };
    bool AddDecimalPs(const char *col_name, unsigned char p, unsigned char s, bool null_able = false) {
        return AddCol(col_name, GenDataType(T_DECIMAL_PS, null_able, p, s));
    };

protected:
    size_t mCount;
    std::vector<BaseColumn *> mPtrCols;
};

}// namespace hdb

#endif // _H_UTIL_H
