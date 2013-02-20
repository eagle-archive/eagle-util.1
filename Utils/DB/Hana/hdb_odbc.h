#ifndef _HDB_ODBC_H
#define _HDB_ODBC_H

#include <string>
#ifdef _WIN32
#include <windows.h> // required by sqlext.h for WIN32
#endif
#include <sqlext.h>
#include "hdb_columns.h"

namespace hdb {


////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintOdbcError(SQLSMALLINT handletype, const SQLHANDLE& handle);
std::string GetOdbcError(SQLSMALLINT handletype, const SQLHANDLE& handle);

template<class T, DATA_TYPE_T data_type> class ColT;
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<char, T_TINYINT> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<short, T_SMALLINT> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<int, T_INTEGER> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQLBIGINT, T_BIGINT> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<float, T_REAL> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<double, T_DOUBLE> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQL_DATE_STRUCT, T_DATE> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQL_TIME_STRUCT, T_TIME> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQL_TIMESTAMP_STRUCT, T_TIMESTAMP> &col);
SQLRETURN SqlBindInParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<double, T_DECIMAL_PS> &col);

////////////////////////////////////////////////////////////////////////////////////////////////////
class OdbcConn {
public:
    OdbcConn(const char *dsn, const char *user, const char *password) 
        : mHenv(NULL), mHdbc(NULL), mConnected(false)
    {
        mDsn = dsn;
        mUser = user;
        mPassword = password;
    };
    ~OdbcConn() {
        if (mHdbc) {
            if (mConnected) {
                SQLDisconnect(mHdbc);
                mConnected = false;
            }
            SQLFreeHandle(SQL_HANDLE_DBC, mHdbc);
            mHdbc = NULL;
        }
        if (mHenv) {
            SQLFreeHandle(SQL_HANDLE_ENV, mHenv);
            mHenv = NULL;
        }
    };
    SQLHENV GetHEnv() const {
        return mHenv;
    };
    SQLHDBC GetHDbc() const {
        return mHdbc;
    };
    std::string GetDbcErrorStr() const {
        return GetOdbcError(SQL_HANDLE_DBC, mHdbc);
    };
    bool Connect();
    bool IsConnected() const {
        return mConnected;
    };

protected:
    std::string mDsn;
    std::string mUser;
    std::string mPassword;
    SQLHENV mHenv;
    SQLHDBC mHdbc;
    bool mConnected;
};

class BaseColumn;
class ColRecords;
class InsertExecutor {
public:
    InsertExecutor(OdbcConn *pConn) {
        mpConn = pConn;
        SQLAllocHandle(SQL_HANDLE_STMT, pConn->GetHDbc(), &mHstmt);
    };
    ~InsertExecutor() {
        if (mHstmt) {
            SQLFreeHandle(SQL_HANDLE_STMT, mHstmt);
            mHstmt = NULL;
        }
        mpConn = NULL;
    };
    SQLHSTMT GetHStmt() const {
        return mHstmt;
    };
    std::string GetErrorStr() const {
        return GetOdbcError(SQL_HANDLE_STMT, mHstmt);
    };
    static bool GetInsStmt(const std::vector<BaseColumn *> &pCols, const char *table_name, std::string &stmt);
    bool PrepareInsStmt(const std::vector<BaseColumn *> &pCols, const char *table_name) const;
    bool ExecuteInsert(const ColRecords &records) const;

protected:
    OdbcConn *mpConn;
    SQLHSTMT mHstmt;
};

}// namespace hdb

#endif // _HDB_ODBC_H
