#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "GeoUtils.h"


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
double GetDistanceInMeter(double lat1, double lng1, double lat2, double lng2)
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
void GetLatLngDegree(double lat0, double lng0, double distance, double angle, double &lat, double &lng)
{
    GetLatLngRad(lat0, lng0, distance, rad(angle), lat, lng);
}

void GetLatLngRad(double lat0, double lng0, double distance, double angle_rad, double &lat, double &lng)
{
    double dx = distance * sin(angle_rad);
    double dy = distance * cos(angle_rad);

    lng = (dx / Ed(lat0) + rad(lng0)) * 180 / M_PI;
    lat = (dy / Ec(lat0) + rad(lat0)) * 180 / M_PI;
}


// Get the distance on earth surface between two points with the same lat coordinate
double GetDistanceSameLatInMeter(double lat, double lng1, double lng2)
{
    double r = (R_EARTH * M_PI / 180) * cos(rad(lat)) * (lng2 - lng1);
    return r < 0 ? -r : r;
}

// Get the distance on earth surface between two points with the same lng coordinate
double GetDistanceSameLngInMeter(double lat1, double lat2)
{
    double r = (R_EARTH * M_PI / 180) * (lat2 - lat1);
    return r < 0 ? -r : r;
}

// return the angle from north, clock-wize, in degrees
double GetHeadingInDegree(const COORDINATE &from, const COORDINATE &to)
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

double GetHeadingInDegree(const double &from_lat, const double &from_lng,
                          const double &to_lat, const double &to_lng)
{
    COORDINATE from, to;
    from.lat = from_lat;
    from.lng = from_lng;
    to.lat = to_lat;
    to.lng = to_lng;
    return GetHeadingInDegree(from, to);
}

}
