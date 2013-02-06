
#include <assert.h>
#include "../hdb_utils.h"

using namespace hdb;

bool TestHdb_Main()
{
    int size = sizeof(DATA_TYPE_T);
    assert(size == 4);

    TynyIntCol ticol("aaa");
    SmallIntCol sicol("bbb");
    IntCol icol("ccc");
    DoubleCol dcol("ddd");

    ColRecords recs;
    recs.AddCol("INT", T_INTEGER);
    recs.AddCol("DOUBLE", T_DOUBLE);

    return true;
};
