#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS // Disable security warning message on MSVC
#endif

#include <string>
#include <fstream>
#include "hdb_utils.h"

using namespace hdb;
using namespace std;

namespace hdb {

static
void csvline_populate(vector<string> &record, const char *line, char delimiter)
{
    int linepos = 0;
    bool inquotes = false;
    char c;
    int linemax = (int)strlen(line);
    string curstring;
    record.clear();

    while(line[linepos]!=0 && linepos < linemax)
    {
        c = line[linepos];

        if (!inquotes && curstring.length()==0 && c=='"')
        {
            //beginquotechar
            inquotes=true;
        }
        else if (inquotes && c=='"')
        {
            //quotechar
            if ( (linepos+1 <linemax) && (line[linepos+1]=='"') ) 
            {
                //encountered 2 double quotes in a row (resolves to 1 double quote)
                curstring.push_back(c);
                linepos++;
            }
            else
            {
                //endquotechar
                inquotes=false; 
            }
        }
        else if (!inquotes && c==delimiter)
        {
            //end of field
            record.push_back( curstring );
            curstring="";
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            record.push_back( curstring );
            return;
        }
        else
        {
            curstring.push_back(c);
        }
        linepos++;
    }
    record.push_back( curstring );
    return;
}

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

SQLRETURN ColRecords::BindAllColumns(SQLHSTMT hstmt) const
{
    SQLRETURN rc;
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
    csvline_populate(strs, line, delimiter);
    if (strs.size() < count) {
        return false;
    }

    for (size_t i = 0; i < count; i++) {
        if (false == mPtrCols[i]->AddFromStr(strs[i].c_str())) {
            return false;
        }
    }
    return true;
}

static bool GetLine(std::ifstream &fs, std::string &line)
{
    line.clear();
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
        }
    }
    return total;
}

} // end of namespace hdb
