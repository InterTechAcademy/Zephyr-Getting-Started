#include<zephyr/kernel.h>
#include<zephyr/drivers/pwm.h>
#include<zephyr/sys/printk.h>
#include <zephyr/console/console.h>
#include<stdio.h>

#define PWM_LED DT_ALIAS(myled)
#define PWM_THREAD_STACK_SIZE 512
#define UART_THREAD_STACK_SIZE 512


K_THREAD_STACK_DEFINE(pwm_stack, PWM_THREAD_STACK_SIZE);
K_THREAD_STACK_DEFINE(uart_stack, UART_THREAD_STACK_SIZE);
K_MUTEX_DEFINE(my_mutex);
// Declare thread data structs
static struct k_thread pwm_thread;
static struct k_thread uart_thread;

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(PWM_LED);
static int32_t sleep_us = 500;
uint32_t pwm_period=0;

void pwm_thread_process(void *arg_1, void *arg_2, void *arg_3){
    uint32_t pulse_ns=0;
    int32_t sleep_us_local;
    int ret;
    pwm_period = pwm_led.period;

    printk("PWM thread started\n");
    while(1){
        k_mutex_lock(&my_mutex, K_FOREVER);
        sleep_us_local = sleep_us;
        k_mutex_unlock(&my_mutex);
        pulse_ns+=1;
        if(pulse_ns>pwm_led.period)pulse_ns=0;
        ret = pwm_set_dt(&pwm_led, pwm_led.period, pulse_ns);
        k_usleep(sleep_us_local);
        
    }
    return;
}

void uart_thread_process(){
    int8_t inc;
    printk("UART thread started\n");
    while (1) {
        const char *line = console_getline();
        if (line[0] == '+') {
            inc = 1;
        } else if (line[0] == '-') {
            inc = -1;
        } else {
            continue;
        }
        k_mutex_lock(&my_mutex, K_FOREVER);
        sleep_us += inc*100;
        if(sleep_us<100)sleep_us=100;
        else if (sleep_us>1000)sleep_us=1000;
        k_mutex_unlock(&my_mutex);
        printk("Changed sleep_us to: %d\r\n", sleep_us);
    }
}

int main(void){
    if (!pwm_is_ready_dt(&pwm_led)) {
		printk("PWM is not ready");
		return 0;
	}
    console_getline_init();

    
    k_tid_t pwm_tid;
    k_tid_t uart_tid;
    pwm_tid = k_thread_create(&pwm_thread,          // Thread struct
                              pwm_stack,            // Stack
                              K_THREAD_STACK_SIZEOF(pwm_stack),
                                pwm_thread_process,     // Entry point
                                NULL,                   // arg_1
                                NULL,                   // arg_2
                                NULL,                   // arg_3
                                6,                      // Priority
                                0,                      // Options
                                K_NO_WAIT);             // Delay
                                
    
    uart_tid = k_thread_create(&uart_thread,          // Thread struct
                              uart_stack,            // Stack
                              K_THREAD_STACK_SIZEOF(uart_stack),
                              uart_thread_process,     // Entry point
                                NULL,                   // arg_1
                                NULL,                   // arg_2
                                NULL,                   // arg_3
                                7,                      // Priority
                                0,                      // Options
                                K_NO_WAIT);             // Delay
    
    while (1) {
        k_msleep(1000);
    }
}