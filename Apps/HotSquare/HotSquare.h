#ifndef HOT_SQUARE_H_
#define HOT_SQUARE_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Constants

// define it to 0 to read all segments
#ifdef _DEBUG
#define SEGMENTS_CSV_READ_LIMIT  100
#else
#define SEGMENTS_CSV_READ_LIMIT  0
#endif


#define R_EARTH     6371004 // in meters
#define LAT_METERS_PER_DEGREE   (111194.99646)  // R_EARTH * 2 * PI / 360

#define SQUARE_ZOOM_LEVEL  21.4142821   // Zoom level 21.4142821 (HA ER BIN) is for 10M x 10M square

// Device [0, 360) into some heading levels
// E.g., for 8 heading levels: 0: [-22.5, 22.5), Level 1: [22.5, 45+22.5), ...
// NOTE: make sure it can be divided by 360
#define HEADING_LEVEL_NUM   72

#define TILE_ZOOM_LEVEL  17
#define TOTAL_TILE_NUM  (2 << (TILE_ZOOM_LEVEL - 1))

#define SEG_ASSIGN_DISTANCE_MAX     50 // in meter

// Check if there is C++11 support
#if !(defined(_MSC_VER) && _MSC_VER < 1600)
#define CPP11_SUPPORT
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FormatTimeStr(unsigned int uTimeMs);
std::string ElapsedTimeStr();
void StringReplace(std::string &strBase, const std::string &strSrc, const std::string &strDes);
bool CreateDirNested(const char *pDir);
bool GetLine(std::ifstream &fs, std::string &line);
void CsvLinePopulate(std::vector<std::string> &items, const std::string &line, char delimiter);

double GetDistanceInMeter(double lat1, double lng1, double lat2, double lng2);
double GetDistanceSameLatInMeter(double lat, double lng1, double lng2);
double GetDistanceSameLngInMeter(double lat1, double lat2);

#endif // HOT_SQUARE_H_
