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

double distance_in_meter(double lat1, double lng1, double lat2, double lng2);
double distance_in_meter_same_lat(double lat, double lng1, double lng2);
double distance_in_meter_same_lng(double lat1, double lat2);
void get_lat_lng_degree(double lat0, double lng0, double distance, double angle, double &lat, double &lng);
void get_lat_lng_rad(double lat0, double lng0, double distance, double angle_rad, double &lat, double &lng);
double get_heading_in_degree(const COORDINATE &from, const COORDINATE &to);
double get_heading_in_degree(const double &from_lat, const double &from_lng,
                             const double &to_lat, const double &to_lng);

// WGS84 to Mars
void wgs84_to_mars(double wgLat, double wgLon, double &mgLat, double &mgLon);
// Mars to BD-09
void bd_encrypt(const double gg_lat, const double gg_lon, double &bd_lat, double &bd_lon);
// BD-09 to Mars
void bd_decrypt(const double bd_lat, const double bd_lon, double &gg_lat, double &gg_lon);

}

#endif // _GEO_UTILS_H
