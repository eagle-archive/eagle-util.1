#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

#include "hdb_utils.h"

using namespace hdb;
using namespace std;

namespace hdb {

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

bool OdbcConn::Connect()
{
    SQLRETURN rc = SQL_SUCCESS;

    if (!mHenv) {
        rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mHenv);
        if (SQL_SUCCEEDED(rc)) {
            rc = SQLSetEnvAttr(mHenv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        }
    }
    if (!mHdbc) {
        if (SQL_SUCCEEDED(rc)) {
            rc = SQLAllocHandle(SQL_HANDLE_DBC, mHenv, &mHdbc);
        }
        if (SQL_SUCCEEDED(rc)) {
            rc = SQLSetConnectAttr(mHdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
        }
    }
#ifndef FAKE_DB_CONN
    /* Connect to the database */
    if (SQL_SUCCEEDED(rc)) {
        rc = SQLConnect(mHdbc, (SQLCHAR*)mDsn.c_str(), SQL_NTS,
            (SQLCHAR*)mUser.c_str(), SQL_NTS,
            (SQLCHAR*)mPassword.c_str(), SQL_NTS);
    }
#endif //FAKE_DB_CONN
    mConnected = SQL_SUCCEEDED(rc);
    return mConnected;
}

bool InsertExecutor::PrepareInsStmt(std::vector<BaseColumn *> pCols, const char *table_name)
{
    char ins_into[1024] = "INSERT INTO ";
    char values[512] = "VALUES (";

    strncat(ins_into, table_name, sizeof(ins_into));
    strncat(ins_into, " (", sizeof(ins_into));

    size_t size = pCols.size();
    for (size_t i = 0; i < size; i++) {
        strncat(ins_into, pCols[i]->GetColName(), sizeof(ins_into));
        if (i == size - 1) {
            strncat(ins_into, ") ", sizeof(ins_into));
            strncat(values, ")", sizeof(values));
        } else {
            strncat(ins_into, ", ", sizeof(ins_into));
            strncat(values, "?,", sizeof(values));
        }
    }
    strncat(ins_into, values, sizeof(ins_into));

    SQLRETURN rc = SQLPrepare(mHstmt, (SQLCHAR *)ins_into, SQL_NTS);
    return SQL_SUCCEEDED(rc);
}

bool InsertExecutor::ExecuteInsert(ColRecords *pRecords)
{
    SQLULEN ParamsProcessed;
    SQLRETURN rc;

    rc = SQLSetStmtAttr(mHstmt, SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)SQL_PARAM_BIND_BY_COLUMN, 0);
    if (SQL_SUCCEEDED(rc)) {
        SQLUINTEGER count = (SQLUINTEGER)pRecords->GetRowCount();
        rc = SQLSetStmtAttr(mHstmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)count, 0);
    }
    if (SQL_SUCCEEDED(rc)) {
        rc = SQLSetStmtAttr(mHstmt, SQL_ATTR_PARAMS_PROCESSED_PTR, &ParamsProcessed, 0);
    }

    /* Bind the parameters in the column-wise fashion. */
    if (SQL_SUCCEEDED(rc)) {
        rc = pRecords->SqlBindAllColumns(mHstmt);
    }

#ifndef FAKE_DB_CONN
    if (SQL_SUCCEEDED(rc)) {
        rc = SQLExecute(mHstmt);
    }
#endif
    if (SQL_SUCCEEDED(rc)) {
        rc = SQLEndTran(SQL_HANDLE_DBC, mpConn->GetHDbc(), SQL_COMMIT);
    }
    return SQL_SUCCEEDED(rc);
}

} // end of namespace hdb
