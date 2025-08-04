#include<zephyr/kernel.h>
#include<zephyr/drivers/pwm.h>
#include<zephyr/sys/printk.h>
#include<stdio.h>

#define PWM_LED DT_ALIAS(myled)
#define PWM_THREAD_STACK_SIZE 256


K_THREAD_STACK_DEFINE(pwm_stack, PWM_THREAD_STACK_SIZE);

// Declare thread data structs
static struct k_thread pwm_thread;


static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(PWM_LED);

void pwm_thread_process(void *arg_1, void *arg_2, void *arg_3){
        uint32_t pulse_ns=0;
    int ret;
    if (!pwm_is_ready_dt(&pwm_led)) {
		printk("PWM is not ready\n");
		return;
	}
    while(1){
        pulse_ns+=1;
        if(pulse_ns>pwm_led.period)pulse_ns=0;
        ret = pwm_set_dt(&pwm_led, pwm_led.period, pulse_ns);
        k_msleep(1); 
        
    }
    return;
}

int main(void){
    int tik=0;
    k_tid_t pwm_tid;
    pwm_tid = k_thread_create(&pwm_thread,          // Thread struct
                              pwm_stack,            // Stack
                              K_THREAD_STACK_SIZEOF(pwm_stack),
                                pwm_thread_process,     // Entry point
                                NULL,                   // arg_1
                                NULL,                   // arg_2
                                NULL,                   // arg_3
                                7,                      // Priority
                                0,                      // Options
                                K_NO_WAIT);             // Delay
    while (1) {
        tik+=1;
        printk("Hello %d\n",tik);
        k_msleep(1000);
    }
}