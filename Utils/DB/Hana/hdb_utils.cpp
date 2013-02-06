
#include <vector>
#include "hdb_utils.h"

using namespace hdb;
using namespace std;

namespace hdb {

DATA_TYPE_S GenDataType(DATA_TYPE_T type, bool null_able, unsigned char p1, unsigned char p2)
{
    DATA_TYPE_S t;
    t.type = type;
    t.null_able = null_able;
    t.p = p1;
    t.s = p2;
    return t;
}

bool ColRecords::AddCol(const char *col_name, const DATA_TYPE_S &col_type)
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

    default:
        assert(false);
        return false;
    };

    mPtrCols.push_back(pCol);
    return true;
};

} // end of namespace hdb
