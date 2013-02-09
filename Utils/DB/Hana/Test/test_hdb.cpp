
#include <assert.h>
#include "../hdb_utils.h"

using namespace hdb;

bool TestHdb_Main()
{
    bool ok;
    int size = sizeof(DATA_ATTR_T);
    assert(size == 4);

    TynyIntCol ticol("aaa");
    SmallIntCol sicol("bbb");
    IntCol icol("ccc");
    DoubleCol dcol("ddd");

    ColRecords recs;
    ok = recs.AddCol("INT", T_INTEGER);
    assert(ok);
    ok = recs.AddCol("DOUBLE", T_DOUBLE);
    assert(ok);
    assert(recs.GetColCount() == 2);

    ok = recs.AddColsFromCreateSql("CREATE COLUMN TABLE \"I078212\".\"GPS29\" (\"VECHID\" BIGINT  CS_FIXED, \"LNG\" DOUBLE CS_DOUBLE, \"LAT\" DOUBLE CS_DOUBLE, "\
        "\"SPEED\" DOUBLE CS_DOUBLE, \"HEADING\" DOUBLE CS_DOUBLE, \"GPSTIME\" LONGDATE CS_LONGDATE, \"INLOAD\" TINYINT CS_INT, \"INSERVICE\" TINYINT CS_INT, "\
        "\"SEGMENT_ID\" BIGINT CS_FIXED, \"TIME_SLOT\" INTEGER CS_INT)");
    assert(ok);
    assert(recs.GetColCount() == 10);

    return true;
};
