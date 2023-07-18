#include "TB67S109.h"


TB67S109Driver::TB67S109Driver(int pulse_pin, int direction_pin, int enable_pin)
{
    this->pulse_pin = pulse_pin;
    this->direction_pin = direction_pin;
    this->enable_pin = enable_pin;

    pinMode(direction_pin, OUTPUT);
    pinMode(enable_pin, OUTPUT);   

    digitalWrite(direction_pin, HIGH);     

    setEnable(false);              

    setInvertEnable(false);
}

void TB67S109Driver::setEnable(bool enable)
{
    enable_state = enable;
    if(!invert_enable)
    {
        digitalWrite(this->enable_pin, enable?HIGH:LOW);
    }else{
        digitalWrite(this->enable_pin, enable?LOW:HIGH);
    }
}

void TB67S109Driver::setDirection(TB67S109Direction_e direction)
{
    bool foward = HIGH;
    switch(direction)
    {
        case TB67S109_Foward:
        {
            digitalWrite(direction_pin, foward);
        }break;
        case TB67S109_Reverse:
        {
            digitalWrite(direction_pin, !foward);
        }break;
        default:
        {
            Serial.println("ERROR! Invalid Direction.");
        }
    }
}

bool TB67S109Driver::getInvertEnable()
{
    return this->invert_enable;
}

void TB67S109Driver::setInvertEnable(bool invert)
{
    this->invert_enable = invert;
    setEnable(enable_state);
}

TB67S109Driver_GPIO::TB67S109Driver_GPIO(int pulse_pin, int direction_pin, int enable_pin, int32_t pwm_on_time_us)
: TB67S109Driver(pulse_pin, direction_pin, enable_pin)
{
    this->pwm_on_time_us = pwm_on_time_us;
    pinMode(pulse_pin, OUTPUT);

    digitalWrite(pulse_pin, LOW);
    
}

void TB67S109Driver_GPIO::setPWMOnTime(int32_t pwm_on_time_us)
{
    this->pwm_on_time_us = pwm_on_time_us;
}

int32_t TB67S109Driver_GPIO::getPWMOnTime()
{
    return this->pwm_on_time_us;
}

void TB67S109Driver_GPIO::step(TB67S109Direction_e direction, int32_t speed, int32_t steps, bool toggle_enable)
{
    if(toggle_enable){setEnable(true);}
    setDirection(direction);
    noInterrupts();
    for(int i = 0; i < steps; i++)
    {
        digitalWrite(pulse_pin, HIGH);
        delayMicroseconds(pwm_on_time_us);
        digitalWrite(pulse_pin, LOW);
        delayMicroseconds(speed);
    }
    interrupts();
    if(toggle_enable){setEnable(false);}
}
