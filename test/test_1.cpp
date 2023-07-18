#include "Arduino.h"
#include <unity.h>

void test_flash_init(void)
{
    TEST_ASSERT_TRUE(true);
}

void setup()
{
    Serial.begin(115200);
    UNITY_BEGIN();

    RUN_TEST(test_flash_init);
    UNITY_END();
}

void loop()
{

}