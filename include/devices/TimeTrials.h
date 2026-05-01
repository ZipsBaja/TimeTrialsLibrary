#pragma once

#include <hardware/GPIODevice.h>
#include <display/SSD1306.h>
#include <util/TimeHandler.h>
#include <interactive-ui/ScreenManager.h>
#include <interactive-ui/Screen.h>

#define TIMETRIALS_SENSOR_PIN 6

extern ScreenManager manager;
extern Screen main_screen;

namespace devices 
{
    void timetrials_init();
    void timetrials_begin();
    void timetrials_update();
}