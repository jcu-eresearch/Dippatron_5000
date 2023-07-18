#ifndef SETTINGS_H
#define SETTINGS_H

#include "stdint.h"
#include "EEPROM.h"

#define SETTINGS_MAGIC_NUMBER 0xABBADEAD5BEEF

struct DipperSettings_t
{
    uint64_t magic_number = SETTINGS_MAGIC_NUMBER;
    int32_t jog_pwm_off_time = 519;
    int32_t fine_jog_pwm_off_time = 5281;
    int32_t jog_steps = 28350;
    int32_t fine_jog_steps = 94;
    int32_t dip_pwm_off_time = 5281;
    int32_t dip_steps = 28350;
    
    int32_t pwm_on_time_us;

    bool invert_enable;
    int32_t dip_wait_time_S = 60;   

};

void init_settings(DipperSettings_t *settings);
void load_settings(DipperSettings_t *settings);
void save_settings(DipperSettings_t *settings);

#endif