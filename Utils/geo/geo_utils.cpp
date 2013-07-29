#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "geo_utils.h"


using namespace std;

namespace geo {

// degree to rad
double rad(double degree)
{
    return degree * M_PI / 180;
}

// rad to degree
double degree(double rad)
{
    return rad * 180 / M_PI;
}

// Get the distance on earth surface between two points
// Refer to http://blog.sina.com.cn/s/blog_65d859c30101akih.html
double distance_in_meter(double lat1, double lng1, double lat2, double lng2)
{
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double a = radLat1 - radLat2;
    double b = rad(lng1) - rad(lng2);
    double sin_a_2 = sin(a/2);
    double sin_b_2 = sin(b/2);
    double s = 2 * asin(sqrt(sin_a_2 * sin_a_2 + cos(radLat1) * cos(radLat2) * (sin_b_2 * sin_b_2)));
    return s * R_EARTH;
}

static double Ec(double lat)
{
    return POLAR_RADIUS + (EARTH_RADIUS - POLAR_RADIUS) * (90 - lat) / 90;
}

static double Ed(double lat)
{
    return Ec(lat) * cos(rad(lat));
}

// Get (lat, lng) from (lat0, lng0), with distance and angle
// Parameters: distance in meter
//             angle: from north, clock-wize, in degrees
void get_lat_lng_degree(double lat0, double lng0, double distance, double angle, double &lat, double &lng)
{
    get_lat_lng_rad(lat0, lng0, distance, rad(angle), lat, lng);
}

void get_lat_lng_rad(double lat0, double lng0, double distance, double angle_rad, double &lat, double &lng)
{
    double dx = distance * sin(angle_rad);
    double dy = distance * cos(angle_rad);

    lng = (dx / Ed(lat0) + rad(lng0)) * 180 / M_PI;
    lat = (dy / Ec(lat0) + rad(lat0)) * 180 / M_PI;
}


// Get the distance on earth surface between two points with the same lat coordinate
double distance_in_meter_same_lat(double lat, double lng1, double lng2)
{
    double r = (R_EARTH * M_PI / 180) * cos(rad(lat)) * (lng2 - lng1);
    return r < 0 ? -r : r;
}

// Get the distance on earth surface between two points with the same lng coordinate
double distance_in_meter_same_lng(double lat1, double lat2)
{
    double r = (R_EARTH * M_PI / 180) * (lat2 - lat1);
    return r < 0 ? -r : r;
}

// return the angle from north, clock-wize, in degrees
double get_heading_in_degree(const COORDINATE &from, const COORDINATE &to)
{
    double heading;
    double x1 = to.lng - from.lng;
    double y1 = to.lat - from.lat;
    const double x2 = 0;
    const double y2 = 1;
    double cos_value = (x1*x2 + y1*y2) / (sqrt(x1*x1 + y1*y1) * (sqrt(x2*x2 + y2*y2)));
    double delta_radian = acos(cos_value);
    if(x1 > 0) {
        heading = delta_radian * 180.0 / M_PI;
    }else{
        heading = 360.0 - delta_radian * 180.0 / M_PI;
    }
    return heading;
}

double get_heading_in_degree(const double &from_lat, const double &from_lng,
                             const double &to_lat, const double &to_lng)
{
    COORDINATE from, to;
    from.lat = from_lat;
    from.lng = from_lng;
    to.lat = to_lat;
    to.lng = to_lng;
    return get_heading_in_degree(from, to);
}

//
// GCJ-02 to BD-09
// see http://blog.csdn.net/yorling/article/details/9175913
//
void bd_encrypt(const double gg_lat, const double gg_lon, double &bd_lat, double &bd_lon)
{
    double x = gg_lon, y = gg_lat;
    double z = sqrt(x * x + y * y) + 0.00002 * sin(y * M_PI);
    double theta = atan2(y, x) + 0.000003 * cos(x * M_PI);
    bd_lon = z * cos(theta) + 0.0065;
    bd_lat = z * sin(theta) + 0.006;
}

//
// BD-09 to GCJ-02 (Mars Geodetic System)
// see http://blog.csdn.net/yorling/article/details/9175913
//
void bd_decrypt(const double bd_lat, const double bd_lon, double &gg_lat, double &gg_lon)
{
    double x = bd_lon - 0.0065, y = bd_lat - 0.006;
    double z = sqrt(x * x + y * y) - 0.00002 * sin(y * M_PI);
    double theta = atan2(y, x) - 0.000003 * cos(x * M_PI);
    gg_lon = z * cos(theta);
    gg_lat = z * sin(theta);
}


static bool outOfChina(double lat, double lon)
{
    if (lon < 72.004 || lon > 137.8347)
        return true;
    if (lat < 0.8293 || lat > 55.8271)
        return true;
    return false;
}

static double transformLat(double x, double y)
{
    double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));
    ret += (20.0 * sin(6.0 * x * M_PI) + 20.0 * sin(2.0 * x * M_PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(y * M_PI) + 40.0 * sin(y / 3.0 * M_PI)) * 2.0 / 3.0;
    ret += (160.0 * sin(y / 12.0 * M_PI) + 320 * sin(y * M_PI / 30.0)) * 2.0 / 3.0;
    return ret;
}

static double transformLon(double x, double y)
{
    double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));
    ret += (20.0 * sin(6.0 * x * M_PI) + 20.0 * sin(2.0 * x * M_PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(x * M_PI) + 40.0 * sin(x / 3.0 * M_PI)) * 2.0 / 3.0;
    ret += (150.0 * sin(x / 12.0 * M_PI) + 300.0 * sin(x / 30.0 * M_PI)) * 2.0 / 3.0;
    return ret;
}

//
// World Geodetic System (WGS-84) ==> Mars Geodetic System (GCJ-02)
// see http://blog.csdn.net/yorling/article/details/9175913
//
void wgs84_to_mars(double wgLat, double wgLon, double &mgLat, double &mgLon)
{
    if (outOfChina(wgLat, wgLon))
    {
        mgLat = wgLat;
        mgLon = wgLon;
        return;
    }

    //
    // Krasovsky 1940
    //
    // a = 6378245.0, 1/f = 298.3
    // b = a * (1 - f)
    // ee = (a^2 - b^2) / a^2;
    const double a = 6378245.0;
    const double ee = 0.00669342162296594323;

    double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
    double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
    double radLat = wgLat / 180.0 * M_PI;
    double magic = sin(radLat);
    magic = 1 - ee * magic * magic;
    double sqrtMagic = sqrt(magic);
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * M_PI);
    dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * M_PI);
    mgLat = wgLat + dLat;
    mgLon = wgLon + dLon;
}


}
