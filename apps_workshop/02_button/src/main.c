#include<zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>
#include<zephyr/sys/printk.h>
#include<stdio.h>

#define led_node DT_ALIAS(myled)
#define btn_node DT_ALIAS(mybutton)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(led_node,gpios);
static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(btn_node,gpios);

int main(void){
    int ret;
    int btn_state;
    if(!gpio_is_ready_dt(&led)){
        return 0;
    }
    if(!gpio_is_ready_dt(&btn)){
        return 0;
    }
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    ret = gpio_pin_configure_dt(&btn, GPIO_INPUT);
    while(1){
        btn_state = gpio_pin_get_dt(&btn);
        printk("Button state: %d\n",btn_state);
        k_msleep(500);
        ret = gpio_pin_set_dt(&led,btn_state);
        }

    return 0;
}