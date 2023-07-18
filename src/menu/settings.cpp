#include "settings.h"
#include "menu_utils.h"

void init_settings(DipperSettings_t *settings)
{
    printfln("Loading Default Settings");
    settings->magic_number = SETTINGS_MAGIC_NUMBER;
    settings->jog_pwm_off_time = 519;
    settings->fine_jog_pwm_off_time = 5281;
    settings->jog_steps = 28350;
    settings->fine_jog_steps = 94;
    settings->dip_pwm_off_time = 5281;
    settings->dip_steps = 28350;
    
    int32_t pwm_on_time_us;

    settings->invert_enable = true;
    settings->dip_wait_time_S = 60;  
}

void load_settings(DipperSettings_t *settings)
{
    printfln("Loading Settings from EEPROM");
    uint8_t *buf = (uint8_t*)settings;
    for(size_t i = 0; i < sizeof(DipperSettings_t); i++)
    {
        buf[i] = EEPROM.read(i);
    }

    if(settings->magic_number != SETTINGS_MAGIC_NUMBER)
    {
        init_settings(settings);
        save_settings(settings);
    }
    
}

void save_settings(DipperSettings_t *settings)
{
    
    bool wrote = false;
    uint8_t *buf = (uint8_t*)settings;
    for(size_t i = 0; i < sizeof(DipperSettings_t); i++)
    {
        if(EEPROM.read(i) != buf[i])
        {
            EEPROM.write(i, buf[i]);
            wrote = true;
        }
    }
    printfln("Saving Settings to EEPROM: %s", wrote?"True":"No changes detected");
}