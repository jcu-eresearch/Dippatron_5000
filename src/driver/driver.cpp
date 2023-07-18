#include "driver.h"

TB67S109Driver *_stepper = NULL;

void setStepper(TB67S109Driver *stepper)
{
    _stepper = stepper;
}

TB67S109Driver* getStepper()
{
    return _stepper;
}

void jog(int32_t jog_speed, int32_t jog_steps, int32_t fine_jog_speed, int32_t fine_jog_steps)
{
    
    clearBuf();
    while(true)
    {
        if(Serial.available())
        {
            int v = Serial.read();
            if(v == 'q')
            {
                Serial.print("Jogging UP");delayMicroseconds(10);
                _stepper->step(TB67S109_Reverse, jog_speed, jog_steps);
                Serial.println(".");
            }
            else if(v == 'a')
            {
                Serial.print("Jogging Down");delayMicroseconds(10);
                _stepper->step(TB67S109_Foward, jog_speed, jog_steps);
                Serial.println(".");
            }else if(v == 'w')
            {
                Serial.print("Fine Jogging UP");delayMicroseconds(10);
                _stepper->step(TB67S109_Reverse, fine_jog_speed, fine_jog_steps);
                Serial.println(".");
            }else if(v == 's')
            {
                Serial.print("Fine Jogging Down");delayMicroseconds(10);
                _stepper->step(TB67S109_Foward, fine_jog_speed, fine_jog_steps);
                Serial.println(".");
            }else if(v == 27)
            {
                break;
            }
        }
    }
    clearBuf();
}

void run_dip(int32_t dip_raise_speed, int32_t dip_raise_steps, uint32_t dip_wait_time_S, int32_t fine_jog_speed, int32_t fine_jog_steps)
{
    Serial.print("Dipping, Speed: ");Serial.println(dip_raise_speed);;
    // _stepper->setSpeed(dip_raise_speed);
    clearBuf();
    int start = dip_raise_steps;
    Serial.println();
    Serial.println("Dipping: ");
    delay(200);
    unsigned long dip_start = millis();
    _stepper->step(TB67S109_Foward, dip_raise_speed, dip_raise_steps);
    unsigned long dip_end = millis();
    Serial.println();
    Serial.println("You may carry out a fine adjustment while in the wait time.");
    clearBuf();
    while(((millis() - dip_end)/1000) < dip_wait_time_S)
    {
        if(Serial.available())
        {
            switch(Serial.read())
            {
                case FINE_ADJUST_UP:
                {
                    Serial.print("Fine Jogging UP");
                    _stepper->step(TB67S109_Reverse, fine_jog_speed, fine_jog_steps);
                    Serial.println(".");                    
                }break;
                case FINE_ADJUST_DOWN:
                {
                    Serial.print("Fine Jogging Down");
                    _stepper->step(TB67S109_Foward, fine_jog_speed, fine_jog_steps);
                    Serial.println(".");                    
                }break;


            }
        }

    }
    Serial.println("Rasing: ");
    delay(200);
    _stepper->step(TB67S109_Reverse, dip_raise_speed, dip_raise_steps);
    clearBuf();
}

void caculate_printer_values(int32_t off_time_us, int32_t on_time_us, int32_t steps, double &distance_mm, double &speed_mm_s)
{
    int steps_per_mm = printer_steps_per_mm();
    int32_t total_pwm_period = off_time_us + on_time_us;
    double pwm_periods_per_second = double(1000000)/total_pwm_period;
    double jog_seconds = double(steps) / pwm_periods_per_second;
    distance_mm = double(steps)/steps_per_mm;
    speed_mm_s = double(distance_mm)/jog_seconds;
}

int32_t caculate_stepper_off_time(int32_t on_time_us, double speed_mm_s)
{
    int pulses_per_mm = printer_steps_per_mm();
    
    double pulses_per_second = pulses_per_mm * speed_mm_s;
    double usecond_per_pulse = double(1000000)/pulses_per_second;
    return usecond_per_pulse - on_time_us;
}
