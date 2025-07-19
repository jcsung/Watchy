#include "Watchy_Vanilla.h"

void WatchyVanilla::drawWatchFace() {
    initialize();
    businessLogic();
    displayLogic();
}

void WatchyVanilla::businessLogic() {
    cacheBatteryPercentage();
    cacheTimes();
    cacheDate();
    cacheSteps();
}

void WatchyVanilla::displayLogic() {
    drawBattery();
    drawConnectivity();
    drawTime();
    drawDate();
    drawSteps();
    // drawWeather();
}

void WatchyVanilla::cacheBatteryPercentage() {
    float V = getBatteryVoltage();
    batteryPercentage = (int)((V - BATTERY_VOLTAGE_MIN) / (BATTERY_VOLTAGE_MAX - BATTERY_VOLTAGE_MIN) * 100);
}

void WatchyVanilla::cacheTimes() {
    uint16_t i;
    uint16_t carry;
    for (i = 0; i < NUM_TZ; i++) {
        times[i].hour = currentTime.Hour;
        times[i].minute = currentTime.Minute;

        carry = (times[i].minute + relTzMin[i]) / 60;
        times[i].minute = (times[i].minute + relTzMin[i]) % 60;
        times[i].hour = (times[i].hour + carry + relTzHour[i]) % 24;
    }
}

void WatchyVanilla::cacheDate() {
    date.weekday = dayShortStr(currentTime.Wday);
    date.year = currentTime.Year + 1970;
    date.month = currentTime.Month;
    date.day = currentTime.Day;
}

void WatchyVanilla::cacheSteps() {
    // Reset at midnight of local time
    if (currentTime.Hour == 0 && currentTime.Minute == 0) {
        sensor.resetStepCounter();
    }

    stepCount = sensor.getCounter();
}

void WatchyVanilla::drawTime() {
    uint16_t i;

    String adjustedTime;

    lineY += DEFAULT_LINE_SPACING;

    for(i = 0; i < NUM_TZ; i++) {
        display.setFont(&DSEG7Classic_Regular12pt7b);
        lineY += time.h;
        display.setCursor(0, lineY);
        if (times[i].hour < 10) {
            display.print(0);
        }
        display.print(times[i].hour);
        display.print(':');
        if (times[i].minute < 10) {
            display.print(0);
        }
        display.print(times[i].minute);
        display.setFont(&FreeMono12pt7b);
        display.setCursor(time.w + DEFAULT_TIMEZONE_SPACING, lineY);
        display.print(tzName[i]);
        lineY += DEFAULT_LINE_SPACING;
    }
}

void WatchyVanilla::drawDate() {
    display.setFont(&FreeMono12pt7b);
    lineY += other.h;
    display.setCursor(0, lineY);
    display.print(date.weekday);
    display.print(' ');
    display.print(date.year);
    display.print('-');
    if (date.month < 10) {
        display.print(0);
    }
    display.print(date.month);
    display.print("-");
    if (date.day < 10) {
        display.print(0);
    }
    display.println(date.day);
    lineY += DEFAULT_LINE_SPACING;
}

void WatchyVanilla::drawSteps() {
    display.setFont(&FreeMono12pt7b);
    lineY += other.h;

    display.setCursor(0, lineY);
    display.print("Steps: ");
    display.println(stepCount);

    lineY += DEFAULT_LINE_SPACING;
}

void WatchyVanilla::drawBattery() {
    display.setFont(&FreeMono12pt7b);
    lineY += other.h;

    display.setFont(&FreeMono12pt7b);
    display.setCursor(0, lineY);
    display.print("Battery:  ");
    if (batteryPercentage < 100) {
        display.print(' ');
    }
    display.print(batteryPercentage);
    display.println('%');

    lineY += DEFAULT_LINE_SPACING;
}

void WatchyVanilla::drawConnectivity() {
    display.setFont(&FreeMono12pt7b);
    lineY += other.h;

    display.setFont(&FreeMono12pt7b);
    display.setCursor(0, lineY);
    display.print("Wifi: ");
    display.print(WIFI_CONFIGURED ? 'Y' : 'N');

    display.print(" BLE: ");
    display.println(BLE_CONFIGURED ? 'Y' : 'N');

    lineY += DEFAULT_LINE_SPACING;
}

void WatchyVanilla::drawWeather() {
    display.setFont(&FreeMono12pt7b);
    lineY += other.h;

    display.setCursor(0, lineY);
    if (WIFI_CONFIGURED) {
        int16_t x1, y1;
        uint16_t w, h;
        weatherData currentWeather = getWeatherData();
        int8_t temperature = currentWeather.temperature;
        int16_t weatherConditionCode = currentWeather.weatherConditionCode;

        display.print(temperature);

        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.print(" F");

        display.print(' ');

        String weatherIcon("");
        // https://openweathermap.org/weather-conditions
        if (weatherConditionCode > 801) {
            // Cloudy
            weatherIcon = "CLOUDY";
        }
        else if (weatherConditionCode == 801) {
            // Few Clouds
            weatherIcon = "L CLOUD";
        }
        else if (weatherConditionCode == 800) {
            // Clear
            weatherIcon = "CLEAR";
        }
        else if (weatherConditionCode >= 700) {
            // Atmosphere
            weatherIcon = "ATMO";
        }
        else if (weatherConditionCode >= 600) {
            // Snow
            weatherIcon = "SNOW";
        }
        else if (weatherConditionCode >= 500) {
            // Rain
            weatherIcon = "RAIN";
        }
        else if (weatherConditionCode >= 300) {
            // Drizzle
            weatherIcon = "L RAIN";
        }
        else if (weatherConditionCode >= 200) {
            // Thunderstorm
            weatherIcon = "STORM";
        }
        display.println(weatherIcon);

        display.setCursor(w + DEFAULT_DEGREE_SYMBOL_SPACING, lineY - h / 2);
        display.print('o');
    }
    else {
        display.println("NOT  CONNECTED");
    }
}

Position WatchyVanilla::getNextLinePositionTime() {
    return getNextLinePositionHelper(&DSEG7Classic_Regular12pt7b, TIME_CALLIBRATION_STRING);
}

Position WatchyVanilla::getNextLinePositionOther() {
    return getNextLinePositionHelper(&FreeMono12pt7b, OTHER_CALLIBRATION_STRING);
}

Position WatchyVanilla::getNextLinePositionHelper(const GFXfont *font, String callibration) {
    int16_t x1, y1;
    uint16_t w, h;

    display.setFont(font);
    display.getTextBounds(callibration, 0, 0, &x1, &y1, &w, &h);

    Position pos;
    pos.x1 = x1;
    pos.y1 = y1;
    pos.w = w;
    pos.h = h;

    return pos;
}

void WatchyVanilla::initialize() {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    lineY = 0;
    time = getNextLinePositionTime();
    other = getNextLinePositionOther();
}