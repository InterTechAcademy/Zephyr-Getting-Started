#include<zephyr/kernel.h>
#include<zephyr/sys/printk.h>
#include<zephyr/drivers/gpio.h>
#include<zephyr/drivers/counter.h>

#define my_led DT_ALIAS(my_led)
#define my_tim DT_ALIAS(my_timer)

#define COUNTER_TIME 1000000

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(my_led, gpios);



void counter_callback(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data)
{
    int ret;
    struct counter_alarm_cfg *alarm_cfg = user_data;

    alarm_cfg->ticks = counter_us_to_ticks(dev, COUNTER_TIME);
    counter_set_channel_alarm(dev, 0, alarm_cfg);

 
    ret = gpio_pin_toggle_dt(&led); 
    printk("Counter interupt\n");
}

int main(void){
    int ret;
    const struct device *counter_dev = DEVICE_DT_GET(my_tim);

    if (!device_is_ready(counter_dev)) {
        printk("Error: Timer device not ready\r\n");
        return 0;
    }
    if(!gpio_is_ready_dt(&led)){
        return 0;
    }
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    // Configure the alarm
    struct counter_alarm_cfg alarm_cfg = {
        .callback = counter_callback,
        .ticks = counter_us_to_ticks(counter_dev, COUNTER_TIME),
        .user_data = &alarm_cfg,
        .flags = 0
    };

    ret = counter_start(counter_dev);

    ret = counter_set_channel_alarm(counter_dev, 0, &alarm_cfg);
 
    
    while (1) {
        k_msleep(1000);
    }
    return 0;
}