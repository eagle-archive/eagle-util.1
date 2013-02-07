#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

#include "hdb_utils.h"

using namespace hdb;
using namespace std;

namespace hdb {

bool OdbcConn::Connect()
{
    SQLRETURN rc;

    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mHenv);
    if (SQL_SUCCEEDED(rc)) {
        rc = SQLSetEnvAttr(mHenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    }
    if (SQL_SUCCEEDED(rc)) {
        rc = SQLAllocHandle(SQL_HANDLE_DBC, mHenv, &mHdbc);
    }
    if (SQL_SUCCEEDED(rc)) {
        rc = SQLSetConnectAttr(mHdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
    }
#ifndef FAKE_DB_CONN
    /* Connect to the database */
    rc = SQLConnect(mHdbc, (SQLCHAR*)mDsn.c_str(), SQL_NTS,
        (SQLCHAR*)mUser.c_str(), SQL_NTS,
        (SQLCHAR*)mPassword.c_str(), SQL_NTS);
#endif //FAKE_DB_CONN
    return SQL_SUCCEEDED(rc);
}

void PrintOdbcError(SQLSMALLINT handletype, const SQLHANDLE& handle)
{
    SQLCHAR sqlstate[1024];
    SQLCHAR message[1024];
    if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL)) {
        printf("Message: %s SQLSTATE: %s\n", message, sqlstate);
    }
}

std::string GetOdbcError(SQLSMALLINT handletype, const SQLHANDLE& handle)
{
    SQLCHAR sqlstate[1024];
    SQLCHAR message[1024];
    if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL)) {
        char buff[1024];
        sprintf(buff, "Message: %s SQLSTATE: %s", message, sqlstate);
        return buff;
    }
    return "";
}

} // end of namespace hdb
