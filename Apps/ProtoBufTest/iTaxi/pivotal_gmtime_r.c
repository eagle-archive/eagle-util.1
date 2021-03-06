/* pivotal_gmtime_r - a replacement for gmtime/localtime/mktime
                      that works around the 2038 bug on 32-bit
                      systems. (Version 4)

   Copyright (C) 2009  Paul Sheer

   Redistribution and use in source form, with or without modification,
   is permitted provided that the above copyright notice, this list of
   conditions, the following disclaimer, and the following char array
   are retained.

   Redistribution and use in binary form must reproduce an
   acknowledgment: 'With software provided by http://2038bug.com/' in
   the documentation and/or other materials provided with the
   distribution, and wherever such acknowledgments are usually
   accessible in Your program.

   This software is provided "AS IS" and WITHOUT WARRANTY, either
   express or implied, including, without limitation, the warranties of
   NON-INFRINGEMENT, MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE. THE ENTIRE RISK AS TO THE QUALITY OF THIS SOFTWARE IS WITH
   YOU. Under no circumstances and under no legal theory, whether in
   tort (including negligence), contract, or otherwise, shall the
   copyright owners be liable for any direct, indirect, special,
   incidental, or consequential damages of any character arising as a
   result of the use of this software including, without limitation,
   damages for loss of goodwill, work stoppage, computer failure or
   malfunction, or any and all other commercial damages or losses. This
   limitation of liability shall not apply to liability for death or
   personal injury resulting from copyright owners' negligence to the
   extent applicable law prohibits such limitation. Some jurisdictions
   do not allow the exclusion or limitation of incidental or
   consequential damages, so this exclusion and limitation may not apply
   to You.

*/

const char pivotal_gmtime_r_stamp[] =
    "pivotal_gmtime_r. Copyright (C) 2009  Paul Sheer. Terms and "
    "conditions apply. Visit http://2038bug.com/ for more info.";

/* DOCUMENTATION: See http://2038bug.com/pivotal_gmtime_doc.html */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef long long time64_t;

static const int days[4][13] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366},
};

#define LEAP_CHECK(n)	((!(((n) + 1900) % 400) || (!(((n) + 1900) % 4) && (((n) + 1900) % 100))) != 0)
#define WRAP(a,b,m)	((a) = ((a) <  0  ) ? ((b)--, (a) + (m)) : (a))

time64_t pivot_time_t (const time_t * now, time64_t * _t)
{
    time64_t t;
    t = *_t;
    if (now && sizeof (time_t) == 4) {
        time_t _now;
        _now = *now;
        if (_now < 1231500000 /* Jan 2009 - date of writing */ )
            _now = 2147483647;
        if ((time64_t) t + ((time64_t) 1 << 31) < (time64_t) _now)
            t += (time64_t) 1 << 32;
    }
    return t;
}

static struct tm *_gmtime64_r (const time_t * now, time64_t * _t, struct tm *p)
{
    int v_tm_sec, v_tm_min, v_tm_hour, v_tm_mon, v_tm_wday, v_tm_tday;
    int leap;
    time64_t t;
    long m;
    t = pivot_time_t (now, _t);
    v_tm_sec = ((time64_t) t % (time64_t) 60);
    t /= 60;
    v_tm_min = ((time64_t) t % (time64_t) 60);
    t /= 60;
    v_tm_hour = ((time64_t) t % (time64_t) 24);
    t /= 24;
    v_tm_tday = t;
    WRAP (v_tm_sec, v_tm_min, 60);
    WRAP (v_tm_min, v_tm_hour, 60);
    WRAP (v_tm_hour, v_tm_tday, 24);
    if ((v_tm_wday = (v_tm_tday + 4) % 7) < 0)
        v_tm_wday += 7;
    m = (long) v_tm_tday;
    if (m >= 0) {
        p->tm_year = 70;
        leap = LEAP_CHECK (p->tm_year);
        while (m >= (long) days[leap + 2][12]) {
            m -= (long) days[leap + 2][12];
            p->tm_year++;
            leap = LEAP_CHECK (p->tm_year);
        }
        v_tm_mon = 0;
        while (m >= (long) days[leap][v_tm_mon]) {
            m -= (long) days[leap][v_tm_mon];
            v_tm_mon++;
        }
    } else {
        p->tm_year = 69;
        leap = LEAP_CHECK (p->tm_year);
        while (m < (long) -days[leap + 2][12]) {
            m += (long) days[leap + 2][12];
            p->tm_year--;
            leap = LEAP_CHECK (p->tm_year);
        }
        v_tm_mon = 11;
        while (m < (long) -days[leap][v_tm_mon]) {
            m += (long) days[leap][v_tm_mon];
            v_tm_mon--;
        }
        m += (long) days[leap][v_tm_mon];
    }
    p->tm_mday = (int) m + 1;
    p->tm_yday = days[leap + 2][v_tm_mon] + m;
    p->tm_sec = v_tm_sec, p->tm_min = v_tm_min, p->tm_hour = v_tm_hour, p->tm_mon = v_tm_mon, p->tm_wday = v_tm_wday;
    return p;
}

struct tm *gmtime64_r (const time64_t * _t, struct tm *p)
{
    time64_t t;
    t = *_t;
    return _gmtime64_r (NULL, &t, p);
}

struct tm *pivotal_gmtime_r (const time_t * now, const time_t * _t, struct tm *p)
{
    time64_t t;
    t = *_t;
    return _gmtime64_r (now, &t, p);
}

time64_t mktime64 (struct tm * t)
{
    int i, y;
    long day = 0;
    time64_t r;
    if (t->tm_year < 70) {
        y = 69;
        do {
            day -= 365 + LEAP_CHECK (y);
            y--;
        } while (y >= t->tm_year);
    } else {
        y = 70;
        while (y < t->tm_year) {
            day += 365 + LEAP_CHECK (y);
            y++;
        }
    }
    for (i = 0; i < t->tm_mon; i++)
        day += days[LEAP_CHECK (t->tm_year)][i];
    day += t->tm_mday - 1;
    t->tm_wday = (int) ((day + 4) % 7);
    r = (time64_t) day *86400;
    r += t->tm_hour * 3600;
    r += t->tm_min * 60;
    r += t->tm_sec;
    return r;
}

static struct tm *_localtime64_r (const time_t * now, time64_t * _t, struct tm *p)
{
    time64_t tl;
    time_t t;
    struct tm tm, tm_localtime, tm_gmtime;
    _gmtime64_r (now, _t, &tm);
    while (tm.tm_year > (2037 - 1900))
        tm.tm_year -= 28;
    t = mktime64 (&tm);
    localtime_r (&t, &tm_localtime);
    gmtime_r (&t, &tm_gmtime);
    tl = *_t;
    tl += (mktime64 (&tm_localtime) - mktime64 (&tm_gmtime));
    _gmtime64_r (now, &tl, p);
    p->tm_isdst = tm_localtime.tm_isdst;
    return p;
}

struct tm *pivotal_localtime_r (const time_t * now, const time_t * _t, struct tm *p)
{
    time64_t tl;
    tl = *_t;
    return _localtime64_r (now, &tl, p);
}

struct tm *localtime64_r (const time64_t * _t, struct tm *p)
{
    time64_t tl;
    tl = *_t;
    return _localtime64_r (NULL, &tl, p);
}

