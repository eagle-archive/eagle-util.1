#ifndef _GEO_UTILS_H
#define _GEO_UTILS_H


#define R_EARTH         6371004 // in meters
#define EARTH_RADIUS    6378137
#define POLAR_RADIUS    6356725

namespace geo {


typedef struct {
    double lat;
    double lng;
} COORDINATE;

double rad(double degree);
double degree(double rad);

double GetDistanceInMeter(double lat1, double lng1, double lat2, double lng2);
double GetDistanceSameLatInMeter(double lat, double lng1, double lng2);
double GetDistanceSameLngInMeter(double lat1, double lat2);
void GetLatLngDegree(double lat0, double lng0, double distance, double angle, double &lat, double &lng);
void GetLatLngRad(double lat0, double lng0, double distance, double angle_rad, double &lat, double &lng);
double GetHeadingInDegree(const COORDINATE &from, const COORDINATE &to);
double GetHeadingInDegree(const double &from_lat, const double &from_lng,
                          const double &to_lat, const double &to_lng);

}

#endif // _GEO_UTILS_H
