#include<zephyr/kernel.h>
#include<zephyr/sys/printk.h>
#include<stdio.h>



int main(void){
    while(1){
        printk("ON\n");
        k_msleep(500);
        printk("OFF\n");
        k_msleep(500);
    }    
    return 0;
}