#ifndef DRIVER_H
#define DRIVER_H

#include "TB67S109.h"
#include "config/printer.h"
#include "stdint.h"
#include "menu/menu_utils.h"
#include "config/keys.h"
#include "config/ctb/ctb.h"

void setStepper(TB67S109Driver *stepper);
TB67S109Driver* getStepper();
void jog(int32_t jog_speed, int32_t jog_steps, int32_t fine_jog_speed, int32_t fine_jog_steps);
void run_dip(int32_t dip_raise_speed, int32_t dip_raise_steps, uint32_t dip_wait_time_S, int32_t fine_jog_speed, int32_t fine_jog_steps);
void caculate_printer_values(int32_t off_time_us, int32_t on_time_us, int32_t steps, double &distance_mm, double &speed_mm_s);
int32_t caculate_stepper_off_time(int32_t on_time_us, double speed_mm_s);

#endif