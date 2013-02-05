#ifdef _WIN32
// Disable security warning message on MSVC
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "VehicleRecords_Col.h"

#ifdef _WIN32
// Windows does not have snprintf, use _snprintf instead
#define snprintf _snprintf
#endif

using namespace std;

#ifndef _WIN32
// time64_t related functions for Linux
typedef long long time64_t;
extern "C" time64_t mktime64(struct tm *t);
extern "C" struct tm *localtime64_r(const time64_t *t, struct tm *p);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
// Local functions

static inline
long long TimestampToInt64(const SQL_TIMESTAMP_STRUCT &st) {
	struct tm stm;
	memset(&stm, 0, sizeof(struct tm));
	stm.tm_year = st.year - 1900;
	stm.tm_mon = st.month - 1;
	stm.tm_mday = st.day;
	stm.tm_hour = st.hour;
	stm.tm_min = st.minute;
	stm.tm_sec = st.second;
#ifdef _WIN32
	return _mktime64(&stm);
#else
	return mktime64(&stm);
#endif
}

static inline
void Int64ToTimestamp(long long t64, SQL_TIMESTAMP_STRUCT &st) {
	struct tm stm;
#ifdef _WIN32
	_localtime64_s(&stm, &t64);
#else
	localtime64_r(&t64, &stm);
#endif
	st.year = stm.tm_year + 1900;
	st.month = stm.tm_mon + 1;
	st.day = stm.tm_mday;
	st.hour = stm.tm_hour;
	st.minute = stm.tm_min;
	st.second = stm.tm_sec;
	st.fraction = 0;
}

static inline
void GetCurTimestamp(SQL_TIMESTAMP_STRUCT &st) {
	time_t t;
	time(&t);
	struct tm stm;
#ifdef _WIN32
	localtime_s(&stm, &t);
#else
	localtime_r(&t, &stm);
#endif

	st.year = stm.tm_year + 1900;
	st.month = stm.tm_mon + 1;
	st.day = stm.tm_mday;
	st.hour = stm.tm_hour;
	st.minute = stm.tm_min;
	st.second = stm.tm_sec;
	st.fraction = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// class VehicleRecord

void VehicleRecords_Col::Clear() {
	this->mCount = 0;
	this->ARR_VECHID.clear();
	this->ARR_LNG.clear();
	this->ARR_LAT.clear();
	this->ARR_SPEED.clear();
	this->ARR_HEADING.clear();
	this->ARR_GPSTIME.clear();
	this->ARR_INLOAD.clear();
	this->ARR_INSERVICE.clear();
	this->ARR_SEGMENT_ID.clear();
	this->ARR_TIME_SLOT.clear();
}

void VehicleRecords_Col::Reserve(int count) {
	this->ARR_VECHID.reserve(count);
	this->ARR_LNG.reserve(count);
	this->ARR_LAT.reserve(count);
	this->ARR_SPEED.reserve(count);
	this->ARR_HEADING.reserve(count);
	this->ARR_GPSTIME.reserve(count);
	this->ARR_INLOAD.reserve(count);
	this->ARR_INSERVICE.reserve(count);
	this->ARR_SEGMENT_ID.reserve(count);
	this->ARR_TIME_SLOT.reserve(count);
}

void VehicleRecords_Col::CopyFrom(const VehicleRecords_Col& from) {
	assert(this != &from);
	this->mCount = from.mCount;

	ARR_VECHID = from.ARR_VECHID;
	ARR_LNG = from.ARR_LNG;
	ARR_LAT = from.ARR_LAT;
	ARR_SPEED = from.ARR_SPEED;
	ARR_HEADING = from.ARR_HEADING;
	ARR_GPSTIME = from.ARR_GPSTIME;
	ARR_INLOAD = from.ARR_INLOAD;
	ARR_INSERVICE = from.ARR_INSERVICE;
	ARR_SEGMENT_ID = from.ARR_SEGMENT_ID;
	ARR_TIME_SLOT = from.ARR_TIME_SLOT;
}

bool VehicleRecords_Col::ReadFrom(std::ifstream &is, int count) {
	Clear();

    std::string line;
    for (int i = 0; i < count; i++) {
        if (!GetLine(is, line)) {
            break;
        }

        // TODO: ...
    }

    return (mCount > 0);
}

// Generate random records
void VehicleRecords_Col::GenerateRecords(int count) {
	Clear();
	this->mCount = count;
	Reserve(count);

	for (int i = 0; i < count; i++) {
		{
			unsigned long long t = (unsigned long long) ((double) rand()
					/ RAND_MAX * 99999999);
			this->ARR_VECHID.push_back(t);
		}
		{
			double t1 = (double) rand() / RAND_MAX * 10 + 40 + rand() / RAND_MAX;
			double t2 = (double) rand() / RAND_MAX * 10 + 120 + rand() / RAND_MAX;
			this->ARR_LNG.push_back(t1);
			this->ARR_LAT.push_back(t2);
            this->ARR_SPEED.push_back(0);
            this->ARR_HEADING.push_back(rand() % 360);
		}
		{
			SQL_TIMESTAMP_STRUCT st;
			GetCurTimestamp(st);
			st.minute = rand() % 60;
			st.second = rand() % 60;
			this->ARR_GPSTIME.push_back(st);
		}
		{
			this->ARR_INLOAD.push_back(rand()%2);
            this->ARR_INSERVICE.push_back(rand()%2);

			unsigned long long t = (unsigned long long) ((double) rand() / RAND_MAX * 99999999);
            this->ARR_SEGMENT_ID.push_back(t);

            this->ARR_TIME_SLOT.push_back(rand()%144);
		}
	}
}
