#include<zephyr/kernel.h>
#include<zephyr/sys/printk.h>
#include<zephyr/drivers/gpio.h>

#define my_bnt DT_ALIAS(my_button)
#define my_led DT_ALIAS(my_led)

static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(my_bnt, gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(my_led, gpios);

static struct gpio_callback btn_callback_data;


void button_callback(const struct device *dev, struct gpio_callback *cb,uint32_t pins)
{
    int ret;
    ret = gpio_pin_toggle_dt(&led);    
    printk("Button pressed!"); 
}

int main(void){
    int ret;
    if(!gpio_is_ready_dt(&btn)){
        return 0;
    }
    if(!gpio_is_ready_dt(&led)){
        return 0;
    }
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    ret = gpio_pin_configure_dt(&btn, GPIO_INPUT);
    ret = gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&btn_callback_data, button_callback, BIT(btn.pin));
    gpio_add_callback(btn.port, &btn_callback_data);
    while (1) {
        k_msleep(1000);
    }
    return 0;
}