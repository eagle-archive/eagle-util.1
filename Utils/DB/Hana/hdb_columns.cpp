#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

#include <string>
#include <fstream>
#include "hdb_utils.h"

using namespace hdb;
using namespace std;

namespace hdb {


////////////////////////////////////////////////////////////////////////////////////////////////////

bool ColRecords::AddCol(const char *col_name, const DATA_ATTR_T &col_type)
{
    assert(col_name != NULL && col_name[0] != '\0');
    BaseColumn *pCol = NULL;

    switch(col_type.type) {
    case T_TINYINT:
        pCol = new TinyIntCol(col_name, col_type.null_able);
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
        assert(false);
        break;
    case T_DECIMAL_PS:
        assert(false);
        break;
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

SQLRETURN ColRecords::BindAllColumns(SQLHSTMT hstmt) const
{
    SQLRETURN rc = SQL_SUCCESS;
    for (size_t i = 0; i < mPtrCols.size(); i++) {
        rc = mPtrCols[i]->BindParam(hstmt, (SQLUSMALLINT)(i+1));
        if (!SQL_SUCCEEDED(rc)) return rc;
    }
    return rc;
}

// Add one line of CSV
bool ColRecords::AddRow(const char *line, char delimiter)
{
    size_t count = this->GetColCount();
    vector<string> strs;
    strs.reserve(count);
    CsvLinePopulate(strs, line, delimiter);
    if (strs.size() < count) {
        return false;
    }

    for (size_t i = 0; i < count; i++) {
        if (false == mPtrCols[i]->AddFromStr(strs[i].c_str())) {
            return false;
        }
    }
    mRowCount++;
    return true;
}

bool ColRecords::AddColsFromCreateSql(const char *create_sql)
{
    PARSED_TABLE_T parsed_table;
    if (!ParseTableFromSql(create_sql, parsed_table)) {
        return false;
    }
    size_t col_count = parsed_table.col_names.size();
    if (col_count == 0) {
        return false;
    }

    ClearAllCols();
    for (size_t i = 0; i < col_count; i++) {
        if (false == this->AddCol(parsed_table.col_names[i].c_str(), parsed_table.col_attrs[i])) {
            return false;
        }
    }
    return true;
}

static bool GetLine(std::ifstream &fs, std::string &line)
{
    do{
        if(getline(fs, line)) {
            if(fs.good() && line.empty()){
                continue;
            }
            return true;
        } else {
            return false;
        }
    } while(true);
    return false;
}

// return the number of lines actually added
int ColRecords::AddRows(std::ifstream &is_csv, int num, char delimiter)
{
    if (!is_csv.is_open() || !is_csv.good())
        return 0;

    string line;
    int total = 0;
    for (int i = 0; i < num; i++) {
        if (!GetLine(is_csv, line)) break;
        if (AddRow(line.c_str(), delimiter)) {
            total++;
        } else {
            printf("Warning: cannot parse CSV line: %s\n", line.c_str());
        }
    }
    mRowCount = total;
    return total;
}

void ColRecords::GenerateFakeData(size_t row_count)
{
    size_t col_count = this->GetColCount();
    for (size_t col = 0; col < col_count; col++) {
        mPtrCols[col]->GenerateFakeData(row_count);
    }
    mRowCount = row_count;
}

} // end of namespace hdb
