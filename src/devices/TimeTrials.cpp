#include <devices/TimeTrials.h>
#include <vector>
#include <hardware/Button.h>

GPIODeviceDebounce timetrials_sensor = GPIODeviceDebounce(TIMETRIALS_SENSOR_PIN, Pull::UP, GPIO_IRQ_EDGE_FALL, 10000);
TimeHandler clock;
std::vector<uint32_t> lap_log = std::vector<uint32_t>(5, 0);
uint8_t lap_log_idx = 1;
uint8_t lap_log_read = 1;


Button restart_button = Button(5);
Button read_button = Button(4);

bool button_pressed = false;

namespace devices
{
    void timetrials_init()
    {
        timetrials_sensor.AddAction([](const Event* ev, void* ptr)
        {
            if(!button_pressed)
            {
                absolute_time_t current_time = clock.now_us - clock.GetGlobalTimeWhenCreated();
                uint32_t time_formatted = current_time / 10000;
                lap_log[lap_log_idx] = time_formatted;
                uint32_t diff = time_formatted - lap_log[lap_log_idx-1];
                uint32_t left, right;
                if (time_formatted > 5999)
                {
                    left = time_formatted / 6000;
                    if((time_formatted % 6000) < 1000)
                    {
                        right = time_formatted % 6000 / 100;
                    }
                    else
                    {
                        right = time_formatted % 6000 / 10;
                    } 
                }
                else
                {
                    left = time_formatted / 100;
                    right = time_formatted % 100;
                }
                segment_display.DisplayBoth(1, left, right, true);
                printf("Adding %d:%i", left, right); printf(" to lap_log[%d]\n", lap_log_idx);
                lap_log_idx = (lap_log_idx < 5) ? lap_log_idx + 1 : 1;
            }
        });

        restart_button.AddAction([](const Event* ev, void*){
            ButtonEvent* b_event = ev->GetEventAsType<ButtonEvent>();
            if (b_event -> WasPressed())
            {
                if (!button_pressed)
                {
                    button_pressed = true;
                    printf("button_pressed = true\n"); 
                }
                else
                { 
                    clock = TimeHandler();
                    for (int i = 1; i < lap_log_idx; i++)
                    {
                        lap_log[i] = 0;
                    }
                    lap_log_idx = 1;
                    lap_log_read = 1; 
                    button_pressed = false;
                    printf("button_pressed = false\n"); 
                }
            }
        });
        read_button.AddAction([](const Event * ev, void*){
            ButtonEvent* b_event = ev->GetEventAsType<ButtonEvent>();
            if (b_event -> WasPressed())
            {
                uint32_t left, right, diff;
                if(lap_log_read != 1)
                {
                    diff = lap_log[lap_log_read] - lap_log[lap_log_read - 1];
                }
                else
                {
                    diff = lap_log[lap_log_read];
                }
                if (diff > 5999)
                {
                    left = diff / 6000;
                    if((diff % 6000) < 1000)
                    {
                        right = diff % 6000 / 100;
                    }
                    else
                    {
                        right = diff % 6000 / 10;
                    } 
                }
                else
                {
                    left = diff / 100;
                    right = diff % 100;
                }
                segment_display.DisplayBoth(1, left, right, true); 
                printf("displaying %d:%i", left, right); printf(", lap_log_idx = %d, lap_log_read = %i\n", lap_log_idx, lap_log_read);    
                if(lap_log_read == (lap_log_idx - 1) || lap_log_read == 5 || lap_log[1] == 0)
                {
                    lap_log_read = 1;
                }
                else
                {
                    lap_log_read++;
                }        
            }
        });
    }

    void timetrials_begin()
    {
        clock = TimeHandler();
        lap_log.reserve(5);
        lap_log.assign(5, 0);
    }

    void timetrials_update()
    {
        if (button_pressed == true)
        {
            __nop;
        }
        else
        {
            clock.Update();
            absolute_time_t current_time = clock.now_us - clock.GetGlobalTimeWhenCreated();
            uint32_t time_formatted = current_time / 10000;
            uint32_t left, right;
            if (time_formatted > 5999)
            {
                left = time_formatted / 6000;
                if((time_formatted % 6000) < 1000)
                {
                    right = time_formatted % 6000 / 100;
                }
                else
                {
                    right = time_formatted % 6000 / 10;
                } 
            }
            else
            {
                left = time_formatted / 100;
                right = time_formatted % 100;
            }
            segment_display.DisplayBoth(0, left, right, true);
        }
    }

    void timetrials_end()
    {
        
    }
}