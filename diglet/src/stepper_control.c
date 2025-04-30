#include "stepper_control.h"
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/stepper.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(stepper, CONFIG_STEPPER_LOG_LEVEL);

#define STEPS 1000000
#define NUM_Test 5
#define STEPS_PER_REVOLUTION 200
#define STEPS_PER (STEPS_PER_REVOLUTION/NUM_Test)
#define DROP_TIME 3000

static const struct device *stepper_dev;
static uint8_t current_tube = 0;
static int32_t current_position = 0;
static void move(struct k_work *work);
K_WORK_DELAYABLE_DEFINE(tube_work, move);

static void callback(const struct device *dev, const enum stepper_event event, void *user_data)
{
  if(event == STEPPER_EVENT_STEPS_COMPLETED) {
    stepper_get_actual_position(stepper_dev, &current_position);
    printk("Position of tube is : %d (Tube %d)\n", current_position, current_tube);
    k_work_schedule(&tube_work, K_MSEC(DROP_TIME));
  }
}

static void move(struct k_work *work)
{
  current_tube = (current_tube+1) % NUM_Test;
  printk("Moving to tube %d\n", current_tube);
  int32_t next_pos = current_tube * STEPS_PER;
  
  stepper_move_to(stepper_dev, next_pos);
}

void stepper_init(void)
{
  stepper_dev = DEVICE_DT_GET(DT_ALIAS(stepper0));
  if(!device_is_ready(stepper_dev)) {
    printk("stepper not ready\n");
    return;
  }
  
  stepper_set_microstep_interval(stepper_dev, STEPS);
  
  stepper_set_event_callback(stepper_dev, callback, NULL);
  
  stepper_set_reference_position(stepper_dev, 0);
  stepper_enable(stepper_dev);
  
  printk("ready\n");
  
  stepper_move_to(stepper_dev, 0);
  
  k_work_schedule(&tube_work, K_MSEC(DROP_TIME));
}
