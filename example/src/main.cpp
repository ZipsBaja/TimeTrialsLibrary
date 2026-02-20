#include <stdio.h>
#include <devices/TimeTrials.h>

#include <pico/stdlib.h>

static SegmentDisplay::SegmentDisplaySettings display0 = SegmentDisplay::Create(pio0, 10, 11, 0, 7, true);
static SegmentDisplay::SegmentDisplaySettings display1 = SegmentDisplay::Create(pio0, 12, 13, 1, 7, true);

static SegmentDisplay::SegmentDisplaySettings displays[] = {display0, display1};

SegmentDisplay segment_display = SegmentDisplay(displays, 2);

int main()
{
    stdio_init_all();
    
    devices::timetrials_init();
    devices::timetrials_begin();


    while (1)
    {
        Event::HandleEvents();
        devices::timetrials_update();
    }

}