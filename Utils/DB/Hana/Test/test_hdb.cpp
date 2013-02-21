
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

void Test_Insert(OdbcConn *pConn, const char *table_create_sql, const char *csv_lines)
{
    bool ok;
    InsertExecutor ins_exe(pConn);
    ColRecords records;
    PARSED_TABLE_T parsed_table;

    ok = records.AddColsFromCreateSql(table_create_sql);
    if (!ok) {
        printf("Error in parsing create table SQL: %s\n", records.GetErrStr());
    }
    assert(ok);
    assert(records.GetColCount() == 6);

    std::string ins_stmt, err;
    ParseTableFromSql(table_create_sql, parsed_table, err);
    assert(!parsed_table.schema.empty());
    assert(!parsed_table.table_name.empty());

    ok = ins_exe.PrepareInsStmt(records.GetColumns(), (parsed_table.schema + '.' + parsed_table.table_name).c_str());
    assert(ok);

    records.ClearAllRows();
    if (csv_lines) {
        vector<string> lines;
        CsvLinePopulate(lines, csv_lines, '\n');
        for (size_t i = 0; i < lines.size(); i++) {
            records.AddRow(lines[i].c_str());
        }
    }
    if (records.GetRowCount() == 0) {
        records.GenerateFakeData(1000);
        assert(records.GetRowCount() == 3);
    }

    ok = ins_exe.ExecuteInsert(records);
    assert(ok);
}

void Test_Inserts()
{
    OdbcConn *pConn = Test_CreateConn("HD5", "I078212", "Sprint6800");

    static const struct {
        char *create_table;
        char *csv_lines;
    } test_params[] = {
        {
            "CREATE COLUMN TABLE I078212.TEST_CHAR (NAME1 VARCHAR(5), NAME2 VARCHAR(10) NOT NULL , NAME3 NVARCHAR(6), NAME4 CHAR(8) CS_FIXEDSTRING, NAME5 NCHAR(3), NAME6 ALPHANUM(7) CS_ALPHANUM)",
            "\"A7613\",\"A498550370\",\"N23838\",\"A6042970\",\"N78\",A368724\n"\
            "A7613,A498550370,N23838,A6042970,N78,A368724\n"\
            ",,,A6042970,N78,A368724\n"\
            "\"A8394\",\"A559617907\",\"N86199\",\"A5988647\",\"N05\",A427991",
        }
    };

    for (int i = 0; i < sizeof(test_params)/sizeof(*test_params); i++) {
        Test_Insert(pConn, test_params[i].create_table, test_params[i].csv_lines);
    }

    delete pConn;
    pConn = NULL;
}

bool TestHdb_Main()
{
    Test_Types();
    Test_Cols();
    Test_Records();
    Test_Inserts();

    return true;
};
