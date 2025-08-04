#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>

#define RGB_NODE	DT_ALIAS(led_rgb)



static const struct device *const rgb = DEVICE_DT_GET(RGB_NODE);



int main(void)
{
	struct led_rgb color = {0, 0, 0};
	int ret;

	if (device_is_ready(rgb)) {
		printk("Found LED strip device %s", rgb->name);
	} else {
		printk("LED strip device %s is not ready", rgb->name);
		return 0;
	}
	while (1) {
		color.g=0;
		color.r=50;
		ret = led_strip_update_rgb(rgb, &color, 1);

		printk("RED");
		k_msleep(500);
		color.r=0;
		color.b=50;
		ret = led_strip_update_rgb(rgb, &color, 1);
		printk("BLUE");
		k_msleep(500);
		
		color.b=0;
		color.g=50;
		ret = led_strip_update_rgb(rgb, &color, 1);
		printk("GREEN");
		k_msleep(500);
			
		
	}

	return 0;
}
