#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "stepper_control.h"

void main(void) {
    printk("AgriProbe stepper\n");
    stepper_init();
    while (1) {
        k_sleep(K_FOREVER);
    }
}
