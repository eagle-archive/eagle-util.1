#ifndef __VEHICLERECORDS_COL_H_
#define __VEHICLERECORDS_COL_H_

#include <vector>
#include <string>
#ifdef _WIN32
#include <windows.h> // required by sqlext.h for WIN32
#endif
#include "sqlext.h"
#include "insert_sim_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
CREATE COLUMN TABLE GPS29 (
    "VECHID" BIGINT CS_FIXED,
    "LNG" DOUBLE CS_DOUBLE,
    "LAT" DOUBLE CS_DOUBLE,
    "SPEED" DOUBLE CS_DOUBLE,
    "HEADING" DOUBLE CS_DOUBLE,
    "GPSTIME" LONGDATE CS_LONGDATE,
    "INLOAD" TINYINT CS_INT,
    "INSERVICE" TINYINT CS_INT,
    "SEGMENT_ID" BIGINT CS_FIXED,
    "TIME_SLOT" INTEGER CS_INT)
*/


typedef struct {
    long long   vechid;
    double      lng;
    double      lat;
    double      speed;
    double      heading; // double in original DB
    SQL_TIMESTAMP_STRUCT    gpstime;
    short       inload;
    short       inservice;

    long long   segment_id;
    int         time_slot;
} VEHICLE_RECORD;

/*
    Class for vehicle records in column mode
*/
class VehicleRecords_Col {
public:
    VehicleRecords_Col() {
        mCount = 0;
    };
    VehicleRecords_Col(const VehicleRecords_Col& from) {
        CopyFrom(from);
    };
    virtual ~VehicleRecords_Col() {};

public:
    void Clear();
    void Reserve(size_t count);
    void CopyFrom(const VehicleRecords_Col& from);
    void GenerateRecords(int count);
    bool FromRecords(std::vector<VEHICLE_RECORD> &recs);
    bool ReadFrom(std::ifstream &is, size_t count);
    size_t GetCount() {return mCount;};

public:
    std::vector<SQLBIGINT>              ARR_VECHID;
    std::vector<SQLDOUBLE>              ARR_LNG;
    std::vector<SQLDOUBLE>              ARR_LAT;
    std::vector<SQLDOUBLE>              ARR_SPEED;
    std::vector<SQLDOUBLE>              ARR_HEADING;
    std::vector<SQL_TIMESTAMP_STRUCT>   ARR_GPSTIME;
    std::vector<SQLCHAR>                ARR_INLOAD;
    std::vector<SQLCHAR>                ARR_INSERVICE;
    std::vector<SQLBIGINT>              ARR_SEGMENT_ID;
    std::vector<SQLINTEGER>             ARR_TIME_SLOT;

protected:
    size_t mCount;
};

#ifdef __cplusplus
}
#endif
#endif // __VEHICLERECORDS_COL_H_
