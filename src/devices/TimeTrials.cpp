#include <devices/TimeTrials.h>
#include <vector>
#include <hardware/Button.h>
#include <interactive-ui/components/TextComponent.h>
#include <cstring>

void cpy_lap_time(char[], short);


char current_time_str[6];
char time_str[6] = "TIME:";
char reset_str[6];
char read_str[5] = "READ";
char stopped_str[8];
char stop_str[5] = "STOP";

char star1_str[2];
char star2_str[2];
char star3_str[2];
char star4_str[2];
char star5_str[2];


TextComponent time_display = TextComponent(&manager, Vec2i32{63, 2}, current_time_str, &fonts::default_font, 10, &main_screen);
TextComponent time_label = TextComponent(&manager, Vec2i32{2, 2}, time_str, &fonts::default_font, 10, &main_screen);
TextComponent reset_label = TextComponent(&manager, Vec2i32{2, 55}, reset_str, &fonts::default_font, 10, &main_screen);
TextComponent read_label = TextComponent(&manager, Vec2i32{103, 55}, read_str, &fonts::default_font, 10, &main_screen);
TextComponent stopped_label = TextComponent(&manager, Vec2i32{48, 33}, stopped_str, &fonts::default_font, 10, &main_screen);
TextComponent stop_label = TextComponent(&manager, Vec2i32{2, 55}, stop_str, &fonts::default_font, 10, &main_screen);

TextComponent star1 = TextComponent(&manager, Vec2i32{2, 30}, star1_str, &fonts::default_font, 10, &main_screen);
TextComponent star2 = TextComponent(&manager, Vec2i32{8, 30}, star2_str, &fonts::default_font, 10, &main_screen);
TextComponent star3 = TextComponent(&manager, Vec2i32{14, 30}, star3_str, &fonts::default_font, 10, &main_screen);
TextComponent star4 = TextComponent(&manager, Vec2i32{20, 30}, star4_str, &fonts::default_font, 10, &main_screen);
TextComponent star5 = TextComponent(&manager, Vec2i32{26, 30}, star5_str, &fonts::default_font, 10, &main_screen);

char lap1_str[7];
char lap2_str[7];
char lap3_str[7];
char lap4_str[7];
char lap5_str[7];

char lap1_time_str[6];
char lap2_time_str[6];
char lap3_time_str[6];
char lap4_time_str[6];
char lap5_time_str[6];

TextComponent lap1_label = TextComponent(&manager, Vec2i32{2,2}, lap1_str, &SSD1306::default_font, 10, &main_screen);
TextComponent lap2_label = TextComponent(&manager, Vec2i32{2,14}, lap2_str, &SSD1306::default_font, 10, &main_screen);
TextComponent lap3_label = TextComponent(&manager, Vec2i32{2,26}, lap3_str, &SSD1306::default_font, 10, &main_screen);
TextComponent lap4_label = TextComponent(&manager, Vec2i32{2,38}, lap4_str, &SSD1306::default_font, 10, &main_screen);
TextComponent lap5_label = TextComponent(&manager, Vec2i32{2,50}, lap5_str, &SSD1306::default_font, 10, &main_screen);

TextComponent lap1_time_label = TextComponent(&manager, Vec2i32{63,2}, lap1_time_str, &SSD1306::default_font, 10, &main_screen);
TextComponent lap2_time_label = TextComponent(&manager, Vec2i32{63,14}, lap2_time_str, &SSD1306::default_font, 10, &main_screen);
TextComponent lap3_time_label = TextComponent(&manager, Vec2i32{63,26}, lap3_time_str, &SSD1306::default_font, 10, &main_screen);
TextComponent lap4_time_label = TextComponent(&manager, Vec2i32{63,38}, lap4_time_str, &SSD1306::default_font, 10, &main_screen);
TextComponent lap5_time_label = TextComponent(&manager, Vec2i32{63,50}, lap5_time_str, &SSD1306::default_font, 10, &main_screen);

GPIODeviceDebounce timetrials_sensor = GPIODeviceDebounce(TIMETRIALS_SENSOR_PIN, Pull::UP, GPIO_IRQ_EDGE_FALL, 2000);
TimeHandler clock;
std::vector<uint32_t> lap_log = std::vector<uint32_t>(5, 0);
uint8_t lap_log_idx = 1;
uint8_t lap_log_read = 1;
uint32_t time_formatted;


Button restart_button = Button(2);
Button read_button = Button(7);

uint8_t stars_shown = 0;

bool button_pressed = false;
bool read_button_pressed = false;

namespace devices
{
    void timetrials_init()
    {
        manager.SetTargetRefreshRate(50.f);
        time_display.SetFontScale(2);
        time_label.SetFontScale(2);



        timetrials_sensor.AddAction([](const Event* ev, void* ptr)
        {
            if(!button_pressed)
            {
                absolute_time_t current_time = clock.now_us - clock.GetGlobalTimeWhenCreated();
                time_formatted = current_time / 10000;
                if(lap_log_idx == 6)
                {
                    lap_log_idx = 1;
                    for(short i = 1; i < 6; i++)
                    {
                        lap_log[i] = 0;
                    }
                }
                lap_log[lap_log_idx] = time_formatted;
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
                lap_log_idx++;
                switch(stars_shown) 
                {
                    case 0:
                        strncpy(star1_str, "*", sizeof(star1_str));
                        stars_shown++;
                        break;
                    case 1:
                        strncpy(star2_str, "*", sizeof(star2_str));
                        stars_shown++;
                        break;
                    case 2:
                        strncpy(star3_str, "*", sizeof(star3_str));
                        stars_shown++;
                        break;
                    case 3:
                        strncpy(star4_str, "*", sizeof(star4_str));
                        stars_shown++;
                        break;
                    case 4:
                        strncpy(star5_str, "*", sizeof(star5_str));
                        stars_shown++;
                        break;
                    case 5:
                        strncpy(star2_str, "", sizeof(star2_str));
                        strncpy(star3_str, "", sizeof(star3_str));
                        strncpy(star4_str, "", sizeof(star4_str));
                        strncpy(star5_str, "", sizeof(star5_str));
                        stars_shown = 1;
                        break;                    
                }
            }
        });

        restart_button.AddAction([](const Event* ev, void*){
            ButtonEvent* b_event = ev->GetEventAsType<ButtonEvent>();
            if (b_event -> WasPressed())
            {
                if (!button_pressed)
                {
                    strncpy(reset_str, "RESET", sizeof(reset_str));
                    strncpy(stop_str, "", sizeof(stop_str));
                    strncpy(stopped_str, "STOPPED", sizeof(stopped_str));
                    button_pressed = true;
                }
                else
                { 
                    clock = TimeHandler();
                    lap_log.assign(5, 0);
                    lap_log_idx = 1;
                    lap_log_read = 1;
                    strncpy(current_time_str, "00:00", sizeof(current_time_str));
                    strncpy(reset_str, "", sizeof(reset_str));
                    strncpy(stop_str, "STOP", sizeof(stop_str)); 
                    strncpy(stopped_str, "", sizeof(stopped_str)); 
                    strncpy(star1_str, "", sizeof(star1_str));
                    strncpy(star2_str, "", sizeof(star2_str));
                    strncpy(star3_str, "", sizeof(star3_str));
                    strncpy(star4_str, "", sizeof(star4_str));
                    strncpy(star5_str, "", sizeof(star5_str));   
                    stars_shown = 0;          
                    button_pressed = false;
                }
            }
        });
       
        read_button.AddAction([](const Event * ev, void*){
            ButtonEvent* b_event = ev->GetEventAsType<ButtonEvent>();
            if (b_event -> WasPressed())
            {
                if(!read_button_pressed)
                {
                    read_button_pressed = true;
                    strncpy(current_time_str, "", sizeof(current_time_str));
                    strncpy(time_str, "", sizeof(time_str));
                    strncpy(reset_str, "", sizeof(reset_str));
                    strncpy(read_str, "", sizeof(read_str));
                    strncpy(stopped_str, "", sizeof(stopped_str));
                    strncpy(stop_str, "", sizeof(stop_str));
                    strncpy(star1_str, "", sizeof(star1_str));
                    strncpy(star2_str, "", sizeof(star2_str));
                    strncpy(star3_str, "", sizeof(star3_str));
                    strncpy(star4_str, "", sizeof(star4_str));
                    strncpy(star5_str, "", sizeof(star5_str));


                    strncpy(lap1_str, "LAP 1:", sizeof(lap1_str));
                    strncpy(lap2_str, "LAP 2:", sizeof(lap2_str));
                    strncpy(lap3_str, "LAP 3:", sizeof(lap3_str));
                    strncpy(lap4_str, "LAP 4:", sizeof(lap4_str));
                    strncpy(lap5_str, "LAP 5:", sizeof(lap5_str));

                    cpy_lap_time(lap1_time_str, 1);
                    cpy_lap_time(lap2_time_str, 2);
                    cpy_lap_time(lap3_time_str, 3);
                    cpy_lap_time(lap4_time_str, 4);
                    cpy_lap_time(lap5_time_str, 5);

                }
                else
                {
                    read_button_pressed = false;

                    strncpy(lap1_str, "", sizeof(lap1_str));
                    strncpy(lap2_str, "", sizeof(lap2_str));
                    strncpy(lap3_str, "", sizeof(lap3_str));
                    strncpy(lap4_str, "", sizeof(lap4_str));
                    strncpy(lap5_str, "", sizeof(lap5_str));

                    strncpy(lap1_time_str, "", sizeof(lap1_str));
                    strncpy(lap2_time_str, "", sizeof(lap2_str));
                    strncpy(lap3_time_str, "", sizeof(lap3_str));
                    strncpy(lap4_time_str, "", sizeof(lap4_str));
                    strncpy(lap5_time_str, "", sizeof(lap5_str));


                    strncpy(time_str, "TIME:", sizeof(time_str));
                    strncpy(read_str, "READ", sizeof(read_str));
                    strncpy(stop_str, "STOP", sizeof(stop_str));

                    switch(stars_shown) 
                    {
                        case 0:
                            break;
                        case 1:
                            strncpy(star1_str, "*", sizeof(star1_str));
                            break;
                        case 2:
                            strncpy(star1_str, "*", sizeof(star1_str));
                            strncpy(star2_str, "*", sizeof(star2_str));
                            break;
                        case 3:
                            strncpy(star1_str, "*", sizeof(star1_str));
                            strncpy(star2_str, "*", sizeof(star2_str));
                            strncpy(star3_str, "*", sizeof(star3_str));
                            break;
                        case 4:
                            strncpy(star1_str, "*", sizeof(star1_str));
                            strncpy(star2_str, "*", sizeof(star2_str));
                            strncpy(star3_str, "*", sizeof(star3_str));
                            strncpy(star4_str, "*", sizeof(star4_str));
                            break;
                        case 5:
                            strncpy(star1_str, "*", sizeof(star1_str));
                            strncpy(star2_str, "*", sizeof(star2_str));
                            strncpy(star3_str, "*", sizeof(star3_str));
                            strncpy(star4_str, "*", sizeof(star4_str));
                            strncpy(star5_str, "*", sizeof(star5_str));
                            break;                    
                    }
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
        manager.Update();
        if (button_pressed)
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
            if(!read_button_pressed)
            {
                snprintf(current_time_str, sizeof(current_time_str), "%02d:%02d", left, right);
            }
        }
    }
}

void cpy_lap_time(char* lap_str, short lap_num)
{
    uint32_t left, right, diff;
    if(lap_num != 1)
    {
        diff = lap_log[lap_num] - lap_log[lap_num - 1];
    }
    else
    {
        diff = lap_log[lap_num];
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
    
    if(left > 0 || right > 0)
    {
        snprintf(lap_str, sizeof(lap1_str), "%02d:%02d", left, right);
    }
    else
    {
        strncpy(lap_str, "-/-", sizeof(lap_str));
    }    
}
