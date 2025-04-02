#include <zephyr.h>
#include <device.h>
#include <drivers/stepper.h>
#include "stepper_control.h"

#define STEPPER_NODE DT_ALIAS(stepper0)
#define STEPPER_LABEL DT_LABEL(STEPPER_NODE)

const struct device *stepper_dev;

void stepper_init(void) {
    stepper_dev = device_get_binding(STEPPER_LABEL);
    if (!stepper_dev) {
        printk("Stepper device not found!\n");
        return;
    }
}
