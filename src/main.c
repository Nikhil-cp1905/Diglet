#include <zephyr.h>
#include <sys/printk.h>
#include "dc_motor_control.h"
#include "stepper_control.h"

void main(void) {
    printk("AgriProbe BioBox: Initializing...\n");
    dc_motor_init();
    stepper_init();
}

