#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "insert_sim_utils.h"
#include "VehicleRecords_Col.h"

#ifdef _WIN32
// Windows does not have snprintf, use _snprintf instead
#define snprintf _snprintf
#endif

using namespace std;

static SQLHENV g_henv = NULL;
static SQLHDBC g_hdbc = NULL;
static SQLHSTMT g_hstmt = NULL;
static std::ifstream g_is_csv;

int bulk_insert_destory() {
    if (g_hstmt) {
        SQLFreeHandle(SQL_HANDLE_STMT, g_hstmt);
        g_hstmt = NULL;
    }
    if (g_hdbc) {
        SQLDisconnect(g_hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, g_hdbc);
        g_hdbc = NULL;
    }
    if (g_henv) {
        SQLFreeHandle(SQL_HANDLE_ENV, g_henv);
        g_henv = NULL;
    }

    return 0;
}

void show_error(SQLSMALLINT handletype, const SQLHANDLE& handle){
    SQLCHAR sqlstate[1024];
    SQLCHAR message[1024];
    if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
    cout<< "Message: " << message << " SQLSTATE: " << sqlstate << endl;
}

bool bulk_insert_init() {
    bulk_insert_destory();
    SQLRETURN rc;

    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_henv);
    rc = SQLSetEnvAttr(g_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    rc = SQLAllocHandle(SQL_HANDLE_DBC, g_henv, &g_hdbc);

    rc = SQLSetConnectAttr(g_hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
    //rc = SQLSetConnectAttr(g_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)20, 0);

#ifndef FAKE_DB_CONN
    /* Connect to the database */
    rc = SQLConnect(g_hdbc, (SQLCHAR*)GLOBALS.DSN.c_str(), SQL_NTS,
        (SQLCHAR*)GLOBALS.USER.c_str(), SQL_NTS,
        (SQLCHAR*)GLOBALS.PASSWORD.c_str(), SQL_NTS);
    if (rc == SQL_ERROR) {
        show_error(SQL_HANDLE_DBC, g_hdbc);
        printf("Error in SQLConnect()\n");
        return false;
    }
#endif //FAKE_DB_CONN

    rc = SQLAllocHandle(SQL_HANDLE_STMT, g_hdbc, &g_hstmt);
    return true;
}

static
SQLRETURN bind_all_columns(SQLHSTMT hstmt, VehicleRecords_Col &records) {
    SQLRETURN rc;

    SQLUSMALLINT ParaNum = 1;
    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
            sizeof(SQLBIGINT), 0, (SQLPOINTER)records.ARR_VECHID.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
            3, 6, (SQLPOINTER)records.ARR_LNG.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
            3, 6, (SQLPOINTER)records.ARR_LAT.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
            3, 6, (SQLPOINTER)records.ARR_SPEED.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
            3, 6, (SQLPOINTER)records.ARR_HEADING.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
            sizeof(SQL_TIMESTAMP_STRUCT), 0, (SQLPOINTER)records.ARR_GPSTIME.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_CHAR,
            1, 0, (SQLPOINTER)records.ARR_INLOAD.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_CHAR,
            1, 0, (SQLPOINTER)records.ARR_INSERVICE.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
            sizeof(SQLBIGINT), 0, (SQLPOINTER)records.ARR_SEGMENT_ID.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;


    rc = SQLBindParameter(hstmt, ParaNum++, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
            4, 0, (SQLPOINTER)records.ARR_TIME_SLOT.data(), 0, 0);
    if (!SQL_SUCCEEDED(rc)) return rc;

    return rc;
}

static
void get_data(VehicleRecords_Col &records, std::ifstream &is, int num) {
    // records.GenerateRecords(num);
    records.ReadFrom(is, num);
}


static void PrintStatus(size_t rec_count)
{
    static CONSOLE_SCREEN_BUFFER_INFO sConsoleInfo;
    static size_t s_total_count = 0;
    if (sConsoleInfo.dwSize.X == 0) {
        ::GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sConsoleInfo);
    }
    ::SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), sConsoleInfo.dwCursorPosition);
    s_total_count += rec_count;
    printf("\r%s: processing #%lld\n", ElapsedTimeStr().c_str(), (long long)s_total_count);
}

static
void *insert_executor(void *arg) {
    SQLRETURN rc;
    long t_index = (long)arg;

    while(true) {
        SQLHSTMT hstmt = g_hstmt;
        SQLHDBC hdbc = g_hdbc;

        /*
            rc = SQLExecDirect(hstmt, (SQLCHAR*)"ALTER SESSION DISABLE LOG FLUSH", SQL_NTS);
            if (rc == SQL_ERROR) {
                show_error(SQL_HANDLE_STMT, hstmt);
            }
        */

        SQLCHAR Statement[1024];
        snprintf((char *)Statement, sizeof(Statement),
                "INSERT INTO %s (VECHID, LNG, LAT, SPEED, HEADING, GPSTIME, INLOAD, INSERVICE, SEGMENT_ID, TIME_SLOT) "\
                "VALUES (?,?,?,?,?,?,?,?,?,?)", GLOBALS.TABLE_NAME.c_str());
        rc = SQLPrepare(hstmt, Statement, SQL_NTS);
        if (rc == SQL_ERROR) {
            show_error(SQL_HANDLE_STMT, hstmt);
        }

        /* Prepare rows to insert */
        VehicleRecords_Col records;
        get_data(records, g_is_csv, GLOBALS.N_RECORDS);
        if (records.GetCount() == 0) {
            return NULL;
        }
        PrintStatus(records.GetCount());

        /* Set statement attributes for batch processing with column-wise binding */
        SQLULEN ParamsProcessed;
        rc = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)SQL_PARAM_BIND_BY_COLUMN, 0);
        int n_record = (int)records.GetCount();
        rc = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)n_record, 0);
        rc = SQLSetStmtAttr(hstmt, SQL_ATTR_PARAMS_PROCESSED_PTR, &ParamsProcessed, 0);

        /* Bind the parameters in the column-wise fashion. */
        rc = bind_all_columns(hstmt, records);
        if (!SQL_SUCCEEDED(rc)) {
            show_error(SQL_HANDLE_STMT, hstmt);
            return NULL;
        }

#ifndef FAKE_DB_CONN
        rc = SQLExecute(hstmt);
#endif
        if (!SQL_SUCCEEDED(rc)) {
            show_error(SQL_HANDLE_STMT, hstmt);
            return NULL;
        }

        rc = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
        if (!SQL_SUCCEEDED(rc)) {
            show_error(SQL_HANDLE_STMT, hstmt);
            return NULL;
        }
    }

    return NULL;
}

bool bulk_insert(const char *csv)
{
    g_is_csv.open(csv);
    if (!g_is_csv.good()) {
        printf("Error: cannot open file %s\n", csv);
        return false;
    }

    if (false == bulk_insert_init()) {
        bulk_insert_destory();
        return false;
    }

    long t = 0;
    insert_executor(&t);
    bulk_insert_destory();
    g_is_csv.close();
    return true;
}
