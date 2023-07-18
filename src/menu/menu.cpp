#include "menu.h"


void menu(DipperSettings_t *settings)
{
    int steps_per_mm = printer_steps_per_mm();

    uint16_t menu_selection = 0;
    bool run = true;

    // int32_t jog_pwm_off_time = 519;
    // int32_t fine_jog_pwm_off_time = 5281;
    // int32_t jog_steps = 28350;
    // int32_t fine_jog_steps = 94;
    // int32_t dip_pwm_off_time = 5281;
    // int32_t dip_steps = 28350;
    
    int32_t pwm_on_time_us;

    // bool invert_enable;
    // int32_t dip_wait_time_S = 60;
    
    TB67S109Driver_GPIO* myStepper = (TB67S109Driver_GPIO*)getStepper();

    while(run)
    {
        // jog_distance = double(jog_steps) / steps_per_mm;
        pwm_on_time_us = myStepper->getPWMOnTime();
        settings->invert_enable = myStepper->getInvertEnable();

        double jog_distance_mm, jog_speed_mm_s;
        caculate_printer_values(settings->jog_pwm_off_time, pwm_on_time_us, settings->jog_steps, jog_distance_mm, jog_speed_mm_s);

        double fine_jog_distance_mm, fine_jog_speed_mm_s;
        caculate_printer_values(settings->fine_jog_pwm_off_time, pwm_on_time_us, settings->fine_jog_steps, fine_jog_distance_mm, fine_jog_speed_mm_s);
        
        double dip_distance_mm, dip_speed_mm_s;
        caculate_printer_values(settings->dip_pwm_off_time, pwm_on_time_us, settings->dip_steps, dip_distance_mm, dip_speed_mm_s);


        Serial.println();
        printfln("Stepper Dipper, Steps per mm: %i", steps_per_mm);
        Serial.println();
        Serial.print("\t 1. Stepper PWM On Time uS [");Serial.print(pwm_on_time_us);Serial.println("]");
        Serial.print("\t 2. Invert Enable [");Serial.print(settings->invert_enable?"true":"false");Serial.println("]");Serial.println();
        
        Serial.print("\t10. Jog Off Time (uS)[");Serial.print(settings->jog_pwm_off_time);Serial.println("]");
        Serial.print("\t11. Jog Speed (mm/S) [");Serial.print(jog_speed_mm_s);Serial.println("]");
        Serial.print("\t12. Jog Steps        [");Serial.print(settings->jog_steps);Serial.println("]");
        Serial.print("\t13. Jog Distance (mm)[");Serial.print(jog_distance_mm);Serial.println("]");Serial.println();

        Serial.print("\t20. Fine Jog Off Time (uS)[");Serial.print(settings->fine_jog_pwm_off_time);Serial.println("]");
        Serial.print("\t21. Fine Jog Speed (mm/S) [");Serial.print(fine_jog_speed_mm_s);Serial.println("]");
        Serial.print("\t22. Fine Jog Steps        [");Serial.print(settings->fine_jog_steps);Serial.println("]");
        Serial.print("\t23. Fine Jog Distance (mm)[");Serial.print(fine_jog_distance_mm);Serial.println("]");Serial.println();

        Serial.print("\t30. Dip Off Time (uS)[");Serial.print(settings->dip_pwm_off_time);Serial.println("]");
        Serial.print("\t31. Dip Speed (mm/S) [");Serial.print(dip_speed_mm_s);Serial.println("]");
        Serial.print("\t32. Dip Steps        [");Serial.print(settings->dip_steps);Serial.println("]");
        Serial.print("\t33. Dip Distance (mm)[");Serial.print(dip_distance_mm);Serial.println("]");Serial.println();
        Serial.print("\t34. Dip Wait Time (S)[");Serial.print(settings->dip_wait_time_S);Serial.println("]");Serial.println();
        
        
        Serial.println();
        Serial.println("\t40. Jog");
        Serial.println("\t41. Dip Raise");        
        Serial.println();
        Serial.print("Enter Selection: ");
        if(!read_uint16_t(Serial, &menu_selection)){
            Serial.println("No Selection Entered.");
            continue;
        }
        clearBuf();
        Serial.println();

        switch(menu_selection) {
            case 1:
            {
                Serial.print("Enter PWM On Time in uS [");Serial.print(pwm_on_time_us);Serial.print("]: ");
                if(!read_int32_t(Serial, &pwm_on_time_us))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    myStepper->setPWMOnTime(pwm_on_time_us);
                }

            }break;
            case 2:
            {
                Serial.print("Invert Enable Pin [");Serial.print(settings->invert_enable?"true":"false");Serial.print("]: ");
                bool valid = false;
                read_bool_t(Serial, &settings->invert_enable, &valid);
                if(!valid)
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    myStepper->setInvertEnable(settings->invert_enable);
                    save_settings(settings);
                }                
            }break;     


            case 10:
            {
                Serial.print("Enter Jog Off Time (uS) [");Serial.print(settings->jog_pwm_off_time);Serial.print("]: ");
                if(!read_int32_t(Serial, &settings->jog_pwm_off_time))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }                
            }break;
            case 11:
            {
                Serial.print("Enter Jog Speed (mm/S) [");Serial.print(settings->jog_pwm_off_time);Serial.print("]: ");
                int32_t speed_mm_s;
                if(!read_int32_t(Serial, &speed_mm_s))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    settings->jog_pwm_off_time = caculate_stepper_off_time(pwm_on_time_us, speed_mm_s);
                    save_settings(settings);
                }
            }break;
            case 12:
            {
                Serial.print("Enter Jog Stepps [");Serial.print(settings->jog_steps);Serial.print("]: ");
                if(!read_int32_t(Serial, &settings->jog_steps))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    save_settings(settings);
                }
            }break;
            case 13:
            {
                Serial.print("Enter Jog Distance(mm) [");Serial.print(jog_distance_mm);Serial.print("]:");
                double distance = 0;
                if(!read_double_t(Serial, &distance))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    settings->jog_steps = distance * printer_steps_per_mm();
                    save_settings(settings);
                }
            }break;


            case 20:
            {
                Serial.print("Enter Fine Jog Off Time (uS) [");Serial.print(settings->fine_jog_pwm_off_time);Serial.print("]: ");
                if(!read_int32_t(Serial, &settings->fine_jog_pwm_off_time))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    save_settings(settings);
                }                
            }break;
            case 21:
            {
                Serial.print("Enter Fine Jog Speed (mm/S) [");Serial.print(settings->fine_jog_pwm_off_time);Serial.print("]: ");
                int32_t speed_mm_s;
                if(!read_int32_t(Serial, &speed_mm_s))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    settings->fine_jog_pwm_off_time = caculate_stepper_off_time(pwm_on_time_us, speed_mm_s);
                    save_settings(settings);
                }
            }break;
            case 22:
            {
                Serial.print("Enter Fine Jog Stepps [");Serial.print(settings->fine_jog_steps);Serial.print("]: ");
                if(!read_int32_t(Serial, &settings->fine_jog_steps))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    save_settings(settings);
                }
            }break;
            case 23:
            {
                Serial.print("Enter Fine Jog Distance(mm) [");Serial.print(fine_jog_distance_mm);Serial.print("]:");
                double distance = 0;
                if(!read_double_t(Serial, &distance))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    settings->fine_jog_steps = distance * printer_steps_per_mm();
                    save_settings(settings);
                }
            }break;


            case 30:
            {
                Serial.print("Enter Dip Off Time (uS) [");Serial.print(settings->dip_pwm_off_time);Serial.print("]: ");
                if(!read_int32_t(Serial, &settings->dip_pwm_off_time))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    save_settings(settings);
                }

            }break;
            case 31:
            {
                Serial.print("Enter Dip Speed (mm/S) [");Serial.print(settings->jog_pwm_off_time);Serial.print("]: ");
                int32_t speed_mm_s;
                if(!read_int32_t(Serial, &speed_mm_s))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    settings->dip_pwm_off_time = caculate_stepper_off_time(pwm_on_time_us, speed_mm_s);
                    save_settings(settings);
                }
            }break;         
            case 32:
            {
                Serial.print("Enter Dip Steps [");Serial.print(settings->dip_steps);Serial.print("]: ");
                if(!read_int32_t(Serial, &settings->dip_steps))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    save_settings(settings);
                }
            }break;
            case 33:
            {
                Serial.print("Enter Dip Distance(mm) [");Serial.print(dip_distance_mm);Serial.print("]:");
                double distance = 0;
                if(!read_double_t(Serial, &distance))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    settings->dip_steps = distance * printer_steps_per_mm();
                    save_settings(settings);
                }

            }break;
            case 34:
            {
                Serial.print("Enter Dip Wait Time [");Serial.print(settings->dip_wait_time_S);Serial.print("] S: ");
                if(!read_int32_t(Serial, &settings->dip_wait_time_S))
                {
                    Serial.print("\r\n\r\nNo Selection Entered\r\n");
                }else{
                    save_settings(settings);
                }
            }break;            
         
            case 40:
            {
                jog(settings->jog_pwm_off_time, settings->jog_steps, settings->fine_jog_pwm_off_time, settings->fine_jog_steps);
            }break;
            case 41:
            {
                run_dip(settings->dip_pwm_off_time, settings->dip_steps, settings->dip_wait_time_S, settings->fine_jog_pwm_off_time, settings->fine_jog_steps);
            }break;
            case 0:
                run = false;
                break;
            default:
                Serial.print("Unknown selection: ");
                Serial.println(menu_selection);
        }


    }
}