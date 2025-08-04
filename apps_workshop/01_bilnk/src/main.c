#include<zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>
#include<zephyr/sys/printk.h>
#include<stdio.h>

#define led_node DT_ALIAS(myled)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(led_node,gpios);

int main(void){
    int ret;
    if(!gpio_is_ready_dt(&led)){
        return 0;
    }
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    while(1){
        ret = gpio_pin_set_dt(&led,1);
        printk("ON\n");
        k_msleep(500);
        ret = gpio_pin_set_dt(&led,0);
        printk("OFF\n");
        k_msleep(500);
    }

    return 0;
}