#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

#include "hdb_utils.h"

using namespace hdb;
using namespace std;

namespace hdb {


////////////////////////////////////////////////////////////////////////////////////////////////////

bool ColRecords::AddCol(const char *col_name, const DATA_ATTR_T &col_type)
{
    assert(col_name != NULL);
    BaseColumn *pCol = NULL;

    switch(col_type.type) {
    case T_TYNYINT:
        pCol = new TynyIntCol(col_name, col_type.null_able);
        break;
    case T_SMALLINT:
        pCol = new SmallIntCol(col_name, col_type.null_able);
        break;
    case T_INTEGER:
        pCol = new IntCol(col_name, col_type.null_able);
        break;
    case T_BIGINT:
        pCol = new BigIntCol(col_name, col_type.null_able);
        break;
    case T_REAL:
        pCol = new RealCol(col_name, col_type.null_able);
        break;
    case T_DOUBLE:
        pCol = new DoubleCol(col_name, col_type.null_able);
        break;
    case T_DATE:
        pCol = new DateCol(col_name, col_type.null_able);
        break;
    case T_TIME:
        pCol = new TimeCol(col_name, col_type.null_able);
        break;
    case T_TIMESTAMP:
        pCol = new TimeStampCol(col_name, col_type.null_able);
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

    mPtrCols.push_back(pCol);
    return true;
}

SQLRETURN ColRecords::SqlBindAllColumns(SQLHSTMT hstmt)
{
    SQLRETURN rc;
    for (size_t i = 0; i < mPtrCols.size(); i++) {
        rc = mPtrCols[i]->SqlBindParam(hstmt, (SQLUSMALLINT)(i+1));
        if (!SQL_SUCCEEDED(rc)) return rc;
    }
    return rc;
}

} // end of namespace hdb
