#include<zephyr/kernel.h>
#include<zephyr/sys/printk.h>
#include<stdio.h>

#define THREAD1_STACK_SIZE 512
#define THREAD2_STACK_SIZE 512


K_THREAD_STACK_DEFINE(stack1, THREAD1_STACK_SIZE);
K_THREAD_STACK_DEFINE(stack2, THREAD2_STACK_SIZE);
K_MUTEX_DEFINE(my_mutex);
// Declare thread data structs
static struct k_thread thread1;
static struct k_thread thread2;

K_SEM_DEFINE(mes, 0, 1);

void thread1_process(void *arg_1, void *arg_2, void *arg_3){
    int counter1 =0;
    while(1){
        counter1 +=1;
        printk("Thread1 counter1: %d\n",counter1);
        if(counter1%4==0)k_sem_give(&mes); 
        k_msleep(1000);

    }
}

void thread2_process(){
    int counter2=0;
    while(1){
        counter2+=1;
        k_sem_take(&mes, K_FOREVER);
        printk("Thread2 counter2: %d\n",counter2);
    }
}

int main(void){

    k_tid_t tid1;
    k_tid_t tid2;
    tid1 = k_thread_create(&thread1,          // Thread struct
                              stack1,            // Stack
                              K_THREAD_STACK_SIZEOF(stack1),
                                thread1_process,     // Entry point
                                NULL,                   // arg_1
                                NULL,                   // arg_2
                                NULL,                   // arg_3
                                6,                      // Priority
                                0,                      // Options
                                K_NO_WAIT);             // Delay
                                
    
    tid2 = k_thread_create(&thread2,          // Thread struct
                              stack2,            // Stack
                              K_THREAD_STACK_SIZEOF(stack2),
                              thread2_process,     // Entry point
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