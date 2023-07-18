//
// Created by nigelb on 13/10/21.
//
#include "Arduino.h"
#include "menu/menu.h"
#include "TB67S109.h"
#include "EEPROM.h"
#include "menu/settings.h"
#include <unity.h>


TB67S109Driver_GPIO myStepper(8, 9, 10, 10);
DipperSettings_t settings;

void setup()
{
    memset(&settings, 0, sizeof(settings));
    EEPROM.begin();
    Serial.begin(115200);

    setMenuStream(&Serial);
    setStepper(&myStepper);
    load_settings(&settings);

    printfln("EEPROM Size: %i", EEPROM.length());
    myStepper.setInvertEnable(true);
}

void loop()
{
    menu(&settings);
    delay(1000);
}


