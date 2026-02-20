#pragma once

#include <hardware/GPIODevice.h>
#include <display/SegmentDisplay.h>
#include <util/TimeHandler.h>

#define TIMETRIALS_SENSOR_PIN 6

extern SegmentDisplay segment_display;

extern GPIODeviceDebounce timetrials_sensor;
extern TimeHandler clock;

namespace devices 
{
    void timetrials_init();
    void timetrials_begin();
    void timetrials_update();
    void timetrial_end();
}