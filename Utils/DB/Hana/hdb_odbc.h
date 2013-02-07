#ifndef _HDB_ODBC_H
#define _HDB_ODBC_H

#include <string>
#ifdef _WIN32
#include <windows.h> // required by sqlext.h for WIN32
#endif
#include <sqlext.h>
#include "hdb_odbc.h"

namespace hdb {

////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintOdbcError(SQLSMALLINT handletype, const SQLHANDLE& handle);
std::string GetOdbcError(SQLSMALLINT handletype, const SQLHANDLE& handle);

////////////////////////////////////////////////////////////////////////////////////////////////////
class OdbcConn {
public:
    OdbcConn(const char *dsn, const char *user, const char *password) 
        : mHenv(NULL), mHdbc(NULL)
    {
        mDsn = dsn;
        mUser = user;
        mPassword = password;
    };
    ~OdbcConn() {
        if (mHdbc) {
            SQLDisconnect(mHdbc);
            SQLFreeHandle(SQL_HANDLE_DBC, mHdbc);
        }
        if (mHenv) {
            SQLFreeHandle(SQL_HANDLE_ENV, mHenv);
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

protected:
    SQLHENV mHenv;
    SQLHDBC mHdbc;
    std::string mDsn;
    std::string mUser;
    std::string mPassword;
};

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
    bool Insert(ColRecords *pRecords);

protected:
    OdbcConn *mpConn;
    SQLHSTMT mHstmt;
};

}// namespace hdb

#endif // _HDB_ODBC_H
