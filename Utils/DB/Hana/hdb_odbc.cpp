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

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<char, T_TINYINT> &col)
{
    return SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT,
        0, 0, (SQLPOINTER)col.GetData(), 0, 0);
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<short, T_SMALLINT> &col)
{
    assert(false); // TODO: to implement
    return 0;
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<int, T_INTEGER> &col)
{
    return SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
        0, 0, (SQLPOINTER)col.GetData(), 0, 0);
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQLBIGINT, T_BIGINT> &col)
{
    return SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
        0, 0, (SQLPOINTER)col.GetData(), 0, 0);
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<float, T_REAL> &col)
{
    return SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_REAL,
        0, 0, (SQLPOINTER)col.GetData(), 0, 0);
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<double, T_DOUBLE> &col)
{
    return SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
        0, 0, (SQLPOINTER)col.GetData(), 0, 0);
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQL_DATE_STRUCT, T_DATE> &col)
{
    assert(false); // TODO: to implement
    return 0;
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQL_TIME_STRUCT, T_TIME> &col)
{
    assert(false); // TODO: to implement
    return 0;
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<SQL_TIMESTAMP_STRUCT, T_TIMESTAMP> &col)
{
    return SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
        0, 0, (SQLPOINTER)col.GetData(), 0, 0);
}

SQLRETURN SqlBindParam(SQLHSTMT hstmt, SQLUSMALLINT ipar, const ColT<double, T_DECIMAL_PS> &col)
{
    return SQLBindParameter(hstmt, ipar, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
        0, 0, (SQLPOINTER)col.GetData(), 0, 0);
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

bool InsertExecutor::GetInsStmt(const std::vector<BaseColumn *> &pCols, const char *table_name, std::string &stmt)
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
            strncat(values, "?)", sizeof(values));
        } else {
            strncat(ins_into, ", ", sizeof(ins_into));
            strncat(values, "?,", sizeof(values));
        }
    }
    strncat(ins_into, values, sizeof(ins_into));
    stmt = ins_into;
    return true;
}

bool InsertExecutor::PrepareInsStmt(const std::vector<BaseColumn *> &pCols, const char *table_name) const
{
    string ins_into;
    GetInsStmt(pCols, table_name, ins_into);
    SQLRETURN rc = SQLPrepare(mHstmt, (SQLCHAR *)ins_into.c_str(), SQL_NTS);
    return SQL_SUCCEEDED(rc);
}

bool InsertExecutor::ExecuteInsert(const ColRecords &records) const
{
    SQLULEN ParamsProcessed;
    SQLRETURN rc;

    rc = SQLSetStmtAttr(mHstmt, SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)SQL_PARAM_BIND_BY_COLUMN, 0);
    if (SQL_SUCCEEDED(rc)) {
        SQLUINTEGER count = (SQLUINTEGER)records.GetRowCount();
        rc = SQLSetStmtAttr(mHstmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)count, 0);
    }
    if (SQL_SUCCEEDED(rc)) {
        rc = SQLSetStmtAttr(mHstmt, SQL_ATTR_PARAMS_PROCESSED_PTR, &ParamsProcessed, 0);
    }

    /* Bind the parameters in the column-wise fashion. */
    if (SQL_SUCCEEDED(rc)) {
        rc = records.BindAllColumns(mHstmt);
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
