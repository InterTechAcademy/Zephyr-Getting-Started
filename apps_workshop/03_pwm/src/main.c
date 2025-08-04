#include<zephyr/kernel.h>
#include<zephyr/drivers/pwm.h>
#include<zephyr/sys/printk.h>
#include<stdio.h>

#define PWM_LED DT_ALIAS(myled)

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(PWM_LED);

int main(void){
    uint32_t pulse_ns=0;
    int ret;
    if (!pwm_is_ready_dt(&pwm_led)) {
		printk("PWM is not ready\n");
		return 0;
	}
    while(1){
        pulse_ns+=1;
        if(pulse_ns>pwm_led.period)pulse_ns=0;
        ret = pwm_set_dt(&pwm_led, pwm_led.period, pulse_ns);
        k_msleep(1);
        
    }
    return 0;
}