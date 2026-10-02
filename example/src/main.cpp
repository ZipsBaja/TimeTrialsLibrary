#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <display/SSD1306.h>
#include <interactive-ui/components/PaddingComponent.h>
#include <interactive-ui/ScreenManager.h>
#include <interactive-ui/Screen.h>

SSD1306 oled_display(20, 21, i2c0);
ScreenManager manager(&oled_display);
Screen main_screen(&manager, 128, 64);



#include <devices/TimeTrials.h>

int main()
{
    stdio_init_all();
    i2c_init(i2c0, 400000);
    if (!oled_display.Initialize())
    {
        printf("Failed to initialize SSD1306 display\n");
        return 1;
    }

    oled_display.Power(true);
    oled_display.ClearDisplay();

    
    main_screen.SortComponents();
    manager.PushScreen(&main_screen);
    manager.Update();

    devices::timetrials_begin();
    devices::timetrials_init();

    while (1)
    {
        Event::HandleEvents();
        devices::timetrials_update();
        manager.UpdateDeltaTime();
        manager.UpdateIfAnyComponentMoving();
        oled_display.UpdateDisplay(); 
    }
}
