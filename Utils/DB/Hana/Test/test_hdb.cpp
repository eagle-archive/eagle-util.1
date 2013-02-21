
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

bool Test_Cols()
{
    TinyIntCol ticol("aaa");
    SmallIntCol sicol("bbb");
    IntCol icol("ccc");
    BigIntCol bigcol("bigint");
    DoubleCol dcol("ddd");

    CharCol charcol("CHAR", 5);
    NCharCol ncharcol("nchar", 8);
    VarCharCol vcharcol("VARCHAR", 10);
    NVarCharCol nvarchar("nvarchar", 20);
    AlphaNumCol alphanumcol("AlphaNum", 12);

    return true;
}

bool Test_Records()
{
    bool ok;
    ColRecords recs;
    ok = recs.AddCol("INT", T_INTEGER);
    assert(ok);
    ok = recs.AddCol("DOUBLE", T_DOUBLE);
    assert(ok);
    assert(recs.GetColCount() == 2);

    ok = recs.AddColsFromCreateSql("CREATE COLUMN TABLE \"I078212\".\"GPS29\" ("\
        "\"VECHID\" BIGINT  CS_FIXED, "\
        "\"LNG\" DOUBLE CS_DOUBLE, "\
        "\"LAT\" DOUBLE CS_DOUBLE, "\
        "\"SPEED\" DOUBLE CS_DOUBLE, "\
        "\"HEADING\" DOUBLE CS_DOUBLE, "\
        "\"GPSTIME\" LONGDATE CS_LONGDATE, "\
        "\"INLOAD\" TINYINT CS_INT, "\
        "\"INSERVICE\" TINYINT CS_INT, "\
        "\"SEGMENT_ID\" BIGINT CS_FIXED, "\
        "\"TIME_SLOT\" INTEGER CS_INT)");
    assert(ok);
    assert(recs.GetColCount() == 10);
    recs.GenerateFakeData(15);

    ok = recs.AddColsFromCreateSql("CREATE COLUMN TABLE \"HEB_OSM\".\"WAY_SEGMENTS\" ("\
        "\"ID\" DECIMAL(19,0) CS_FIXED, "\
        "\"FROM_LAT\" DOUBLE CS_DOUBLE, "\
        "\"FROM_LNG\" DOUBLE CS_DOUBLE, "\
        "\"TO_LAT\" DOUBLE CS_DOUBLE, "\
        "\"TO_LNG\" DOUBLE CS_DOUBLE, "\
        "\"WAY_ID\" DECIMAL(19,0) CS_FIXED, "\
        "\"ONE_WAY\" INTEGER CS_INT, "\
        "\"LENGTH\" DOUBLE CS_DOUBLE, "\
        "\"WEIGHT\" DOUBLE CS_DOUBLE)");
    assert(recs.GetColCount() == 9);
    assert(ok);

    ok = recs.AddColsFromCreateSql("CREATE COLUMN TABLE PROTO_ODBC_MULTI ("\
        "GPSDATA_ID BIGINT, "\
        "DEVID VARCHAR(20), "\
        "STIME TIMESTAMP, "\
        "ALARMFLAG INTEGER, "\
        "STATE INTEGER, "\
        "LATITUDE DOUBLE, "\
        "LONGTITUDE DOUBLE, "\
        "SPEED DOUBLE, "\
        "ORIENTATION DOUBLE, "\
        "GPSTIME TIMESTAMP, "\
        "ODOMETER DOUBLE, "\
        "OILGAUGE DOUBLE) "\
        "PARTITION BY HASH (DEVID, STIME) PARTITIONS GET_NUM_SERVERS();");
    assert(ok);
    assert(recs.GetColCount() == 12);

    ok = recs.AddColsFromCreateSql("CREATE COLUMN TABLE \"HEB_OSM\".\"WAY_SEGMENTS\" ("\
        "GPSDATA_ID ABCD_TYPE, "\
        "\"WEIGHT\" DOUBLE CS_DOUBLE)");
    assert(!ok);

    return true;
}

OdbcConn *Test_CreateConn(const char *dsn, const char *user, const char *passwd)
{
    OdbcConn *pConn = NULL;

    pConn = new OdbcConn(dsn, user, passwd);
    assert(pConn);

    bool ok = pConn->Connect();
    if (!ok) {
        printf("Error in SQLConnect(): %s\n", pConn->GetDbcErrorStr().c_str());
    }
    assert(ok);

    return pConn;
}

void Test_CharColInsert(OdbcConn *pConn)
{
    bool ok;
    InsertExecutor ins_exe(pConn);
    ColRecords records;
    PARSED_TABLE_T parsed_table;

    const char *table_create = 
        "CREATE COLUMN TABLE I078212.TEST_CHAR (NAME1 VARCHAR(5), NAME2 VARCHAR(10) NOT NULL , NAME3 NVARCHAR(6), NAME4 CHAR(8) CS_FIXEDSTRING, NAME5 NCHAR(3), NAME6 ALPHANUM(7) CS_ALPHANUM)";
    ok = records.AddColsFromCreateSql(table_create);
    if (!ok) {
        printf("Error in parsing create table SQL: %s\n", records.GetErrStr());
    }
    assert(ok);
    assert(records.GetColCount() == 6);

    std::string ins_stmt, err;
    ParseTableFromSql(table_create, parsed_table, err);
    assert(!parsed_table.schema.empty());
    assert(!parsed_table.table_name.empty());

    ok = ins_exe.PrepareInsStmt(records.GetColumns(), (parsed_table.schema + '.' + parsed_table.table_name).c_str());
    assert(ok);

    records.ClearAllRows();
    records.GenerateFakeData(3);
    assert(records.GetRowCount() == 3);

    ok = ins_exe.ExecuteInsert(records);
    assert(ok);
}

bool TestHdb_Main()
{
    Test_Types();
    Test_Cols();
    Test_Records();

#if 1
    OdbcConn *pConn = Test_CreateConn("HD5", "I078212", "Sprint6800");
    Test_CharColInsert(pConn);
    delete pConn;
    pConn = NULL;
#endif

    return true;
};
