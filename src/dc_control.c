#include <zephyr.h>
#include <device.h>
#include <drivers/pwm.h>
#include "dc_control.h"

#define MOTOR_PWM_NODE DT_ALIAS(pwm0)
#define MOTOR_PWM_LABEL DT_LABEL(MOTOR_PWM_NODE)

const struct device *pwm_dev;

void dc_motor_init(void) {
    pwm_dev = device_get_binding(MOTOR_PWM_LABEL);
}
