#include<zephyr/kernel.h>
#include<zephyr/sys/printk.h>
#include<stdio.h>

int main(void){
    double counter_ms=0;
    while(1){
        printk("Czas:%f\n",counter_ms);
        k_msleep(100);
        counter_ms+=0.1;
        
    }    
    return 0;
}
