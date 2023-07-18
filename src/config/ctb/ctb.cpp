#include "ctb.h"


int printer_steps_per_mm()
{
    double end = POSITION_END;
    double start = POSITION_START;
    double steps = POSITION_STEPS;
    return round(steps / (end - start));
}