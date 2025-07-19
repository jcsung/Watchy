#ifndef WATCHY_VANILLA_H
#define WATCHY_VANILLA_H

#include <Watchy.h>

#include "DSEG7Classic_Regular12pt7b.h"
#include "FreeMono12pt7b.h"

const uint16_t DEFAULT_LINE_SPACING = 2;
const uint16_t DEFAULT_TIMEZONE_SPACING = 5;
const uint16_t DEFAULT_DEGREE_SYMBOL_SPACING = 4;
const float BATTERY_VOLTAGE_MIN = 3.48;
const float BATTERY_VOLTAGE_MAX = 3.91;
const String TIME_CALLIBRATION_STRING = "88:888";
const String OTHER_CALLIBRATION_STRING = "X1Il^qpyi";
const uint16_t NUM_TZ = 4;
const uint16_t NUM_CHARS = 14;

const int16_t relTzHour[] = {0, 12, 15, 16};
const int16_t relTzMin[] = {0, 30, 0, 0};
const String tzName[] = {" PST", " IST", "WITA", " JST"};

struct Position {
    int16_t x1, y1;
    uint16_t w, h;
};

struct Date {
    String weekday;
    uint16_t month, day, year;
};

struct Time {
    uint16_t hour, minute;
};

class WatchyVanilla : public Watchy {
    using Watchy::Watchy;

    public:
        void drawWatchFace();

    private:
        void businessLogic();
        void displayLogic();
        void initialize();
        void cacheBatteryPercentage();
        void cacheTimes();
        void cacheDate();
        void cacheSteps();
        void drawTime();
        void drawDate();
        void drawConnectivity();
        void drawSteps();
        void drawWeather();
        void drawBattery();
        Position getNextLinePositionTime();
        Position getNextLinePositionOther();
        Position getNextLinePositionHelper(const GFXfont *, String);
        uint16_t batteryPercentage;
        Date date;
        Time times[NUM_TZ];
        uint32_t stepCount;
        uint16_t lineY;
        Position time;
        Position other;
};

#endif