#include "util/inc/yy_time.h"
#include "core/inc/yy_exception.h"
#include <time.h>

NS_YY_BEGIN


#ifdef _WIN32
#include <Windows.h>



static const int kMicroSecondsPerSecond = 1000 * 1000;
struct timezone
{
    int tz_minuteswest;
    int tz_dsttime;
};

const sint64 DELTA_EPOCH_IN_MICROSECS= 11644473600000000;

int gettimeofday(struct timeval *tv, struct timezone*)
{
    FILETIME ft;
    sint64 tmpres = 0;
    memset(&ft,0, sizeof(ft));
    GetSystemTimeAsFileTime(&ft);

    tmpres = ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres /= 10;  /*convert into microseconds*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tv->tv_sec = (__int32)(tmpres*0.000001);
    tv->tv_usec =(long)(tmpres%1000000);


    return 0;
}


enum WeekDay
{
    SUNDAY = 0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
};

WeekDay GetCurWeekDay()
{
    tm current_date;
    time_t seconds;
    time(&seconds);
    errno_t err = localtime_s(&current_date, &seconds);
    throw_assert(0 == err, "errno:"<<err);

    return (WeekDay)current_date.tm_wday;
}


WeekDay GetWeekDay(int year, int month, int day)
{
    time_t rawtime;
    tm timeinfo;
    /* get current timeinfo and modify it to the user's choice */
    time ( &rawtime );
    errno_t err = localtime_s(&timeinfo, &rawtime );
    throw_assert(0 == err, "errno:"<<err);

    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;

    /* call mktime: timeinfo->tm_wday will be set */
    mktime (&timeinfo );
    return (WeekDay)timeinfo.tm_wday;
}

#elif __ANDROID__
#elif __APPLE__
#endif
sint64 TimeStamp()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return sint64(((tv.tv_sec % 86400) * 1000) + tv.tv_usec * 0.001);
    //struct timeval tv;
    //gettimeofday(&tv, NULL);

    //sint64 seconds = tv.tv_sec;
    //return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}

std::string StrTimeStamp()
{
    char buf[200] = { 0 };
    timeval tv;
    gettimeofday(&tv, NULL);
    tm* pt = NULL;

#ifdef _WIN32
    pt = _localtime32(&tv.tv_sec);

#elif __ANDROID__
    pt = localtime(&tv.tv_sec);

#elif __APPLE__
#endif

    sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d.%06d",
        pt->tm_year + 1900, pt->tm_mon + 1, pt->tm_mday,
        pt->tm_hour, pt->tm_min, pt->tm_sec,
        int(tv.tv_usec *0.001));
    return buf;
}



NS_YY_END
