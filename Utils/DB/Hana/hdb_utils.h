#ifndef _HDB_UTILS_H
#define _HDB_UTILS_H

#include <memory.h>
#include <vector>
#include <assert.h>
#ifdef _WIN32
#include <windows.h> // required by sqlext.h for WIN32
#endif
#include <sqlext.h>
#include <time.h>
#include "hdb_types.h"
#include "hdb_columns.h"
#include "hdb_odbc.h"

namespace hdb {

void GetCurTm(struct tm &stm);
void GetCurTimestamp(SQL_TIMESTAMP_STRUCT &st);
void GetCurDate(SQL_DATE_STRUCT &date);
void GetCurTime(SQL_TIME_STRUCT &time);

}// namespace hdb

#endif // _HDB_UTILS_H
