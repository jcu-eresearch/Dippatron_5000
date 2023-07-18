
#ifndef TB67S109_H
#define TB67S109_H

#include "Arduino.h"

enum TB67S109Direction_e
{
    TB67S109_Undefined = 0,
    TB67S109_Foward = 1,
    TB67S109_Reverse = 2
};

class TB67S109Driver
{
    protected:
        int pulse_pin;
        int direction_pin;
        int enable_pin;
        bool invert_enable = false;
        bool enable_state = false;
        
        
        
    public:
        TB67S109Driver(int pulse_pin, int direction_pin, int enable_pin);
        void setEnable(bool enable);
        void setDirection(TB67S109Direction_e direction);        
        bool getInvertEnable();
        void setInvertEnable(bool invert);

        virtual void step(TB67S109Direction_e direction, int32_t speed, int32_t steps, bool toggle_enable = true) = 0;
    

};

class TB67S109Driver_GPIO: public TB67S109Driver
{
    private:
        
        int32_t pwm_on_time_us;

    public:
        TB67S109Driver_GPIO(int pulse_pin, int direction_pin, int enable_pin, int32_t pwm_on_time_us);
        
        void setPWMOnTime(int32_t pwm_on_time_us);
        int32_t getPWMOnTime();

     
        virtual void step(TB67S109Direction_e direction, int32_t speed, int32_t steps, bool toggle_enable = true);
        

};

#endif