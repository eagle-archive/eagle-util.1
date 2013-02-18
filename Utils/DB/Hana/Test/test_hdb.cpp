
#include <assert.h>
#include "../hdb_utils.h"

using namespace hdb;
using namespace std;

bool Test_Types()
{
    assert(string("UNKNOWN") == DataTypeToStr((DATA_TYPE_T)-1));
    assert(string("TINYINT") == DataTypeToStr(T_TINYINT));
    assert(string("TEXT") == DataTypeToStr(T_TEXT));

    assert(StrToDataType("AAAA") == T_UNKNOWN);
    assert(StrToDataType("UNKNOWN") == T_UNKNOWN);
    assert(StrToDataType("TINYINT") == T_TINYINT);
    assert(StrToDataType("TEXT") == T_TEXT);
    return true;
}

bool TestHdb_Main()
{
    Test_Types();

    bool ok;
    int size = sizeof(DATA_ATTR_T);
    assert(size == 4);

    TinyIntCol ticol("aaa");
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
    recs.GenerateFakeData(15);

    ok = recs.AddColsFromCreateSql("CREATE COLUMN TABLE \"HEB_OSM\".\"WAY_SEGMENTS\" (\"ID\" DECIMAL(19,0) CS_FIXED, \"FROM_LAT\" DOUBLE CS_DOUBLE, \"FROM_LNG\" "\
        "DOUBLE CS_DOUBLE, \"TO_LAT\" DOUBLE CS_DOUBLE, \"TO_LNG\" DOUBLE CS_DOUBLE, \"WAY_ID\" DECIMAL(19,0) CS_FIXED, \"ONE_WAY\" INTEGER CS_INT, \"LENGTH\" "\
        "DOUBLE CS_DOUBLE, \"WEIGHT\" DOUBLE CS_DOUBLE)");
    assert(recs.GetColCount() == 9);
    assert(ok);

    ok = recs.AddColsFromCreateSql("CREATE COLUMN TABLE PROTO_ODBC_MULTI (GPSDATA_ID BIGINT, DEVID VARCHAR(20), STIME TIMESTAMP, ALARMFLAG INTEGER, STATE INTEGER, "\
        "LATITUDE DOUBLE, LONGTITUDE DOUBLE, SPEED DOUBLE, ORIENTATION DOUBLE, GPSTIME TIMESTAMP, ODOMETER DOUBLE, OILGAUGE DOUBLE) PARTITION BY HASH (DEVID, STIME) PARTITIONS GET_NUM_SERVERS();");
    assert(ok);
    assert(recs.GetColCount() == 12);

    return true;
};
