
#ifndef CTB_PRINTER_H
#define CTB_PRINTER_H

#include <math.h>

// Defined in 0.1mm, so 900 is 90mm
#define POSITION_START   90
#define POSITION_END 296.5
#define POSITION_STEPS 39000
#define POSITION_STEPS_PER_MM (( POSITION_END - POSITION_START ) / POSITION_STEPS)

int printer_steps_per_mm();

#endif