#include <ctype.h>
#include <stdint.h>
#include<zephyr/kernel.h>
#include<zephyr/device.h>
#include<zephyr/drivers/stepper.h>
#include<zephyr/logging/log.h>

LOG_MODULE_REGISTER(stepper, CONFIG_STEPPER_LOG_LEVEL);

#define STEPS 1000000
#define NUM_Test 5
#define STEPS_PER_REVOLUTION 200
#define STEPS_PER (STEPS_PER_REVOLUTION/NUM_Test)
#define DROP_TIME 3000

static const struct device *stepper_dev;
static struct stepper_params params;
static uint8_t current_tube = 0;
static int32_t current_position = 0;

static void move(struct k_work *work);

K_WORK_DELAYABLE_DEFINE(tube_work, move);

static void callback(const struct device *dev, struct stepper_callback_args *args){
  if(args->type == STEPPER_CALLBACK_POSITION_REACHED){
    stepper_get_position(stepper_dev,&current_position);
    printk("Position of tube is : %d (Tube %d)\n", current_position,current_tube);
    k_work_schedule(&tube_work,K_MSEC(DROP_TIME));
  }
}

static void move(struct k_work *work){
  current_tube = (current_tube+1)%NUM_Test;
  printk("Moving to tube %d\n",current_tube);
  int32_t next_pos=current_tube*STEPS_PER;
  params.mode = STEPPER_MODE_PING_PONG_ABSOLUTE;
  params.position=next_pos;
  stepper_move(stepper_dev,&params);
}

void main(void){
  stepper_dev=DEVICE_DT_GET(DT_ALIAS(stepper0));
  if(!device_is_ready(stepper_dev)){
    printk("stpper not ready\n");
    return;
  }

  stepper_set_microstep_interval(stepper_dev,STEPS);
  struct stepper_callback cb = {
    .handler = callback,
  };
  stepper_add_callback(stepper_dev,&cb);
  params.mode=STEPPER_MODE_PING_PONG_ABSOLUTE;
  params.position=0;
  stepper_enable(stepper_dev);
  printk("ready\n");
  params.position=0;
  stepper_move(stepper_dev,&params);
  k_work_schedule(&tube_work,K_MSEC(DROP_TIME));

  while(1){
    k_sleep(K_FOREVER);
  }
}
