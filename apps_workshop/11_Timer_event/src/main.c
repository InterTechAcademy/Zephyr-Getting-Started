#include<stdio.h>
#include<zephyr/kernel.h>
#include<zephyr/drivers/gpio.h>


#define my_led DT_ALIAS(my_led)
#define TIMER_MS 500

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(my_led, gpios);

static struct k_timer my_timer;

void timer_callback(struct k_timer *timer)
{
    int ret;
    if (timer == &my_timer) {
        ret = gpio_pin_toggle_dt(&led); 
        printk("Timer interupt\n");
    }
}

int main(void)
{
    int ret;
    if(!gpio_is_ready_dt(&led)){
        return 0;
    }
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    k_timer_init(&my_timer, timer_callback, NULL);

    k_timer_start(&my_timer, K_MSEC(TIMER_MS), K_MSEC(TIMER_MS));

    // Do nothing
    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}