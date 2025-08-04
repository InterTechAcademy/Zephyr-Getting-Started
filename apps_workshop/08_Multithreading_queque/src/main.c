#include<zephyr/kernel.h>
#include<zephyr/sys/printk.h>
#include<zephyr/random/random.h>
#include<stdio.h>

#define PRODUCER_THREAD_STACK_SIZE 512
#define CONSUMER_THREAD_STACK_SIZE 512

#define QUEUE_SIZE 20


K_THREAD_STACK_DEFINE(producer_stack, PRODUCER_THREAD_STACK_SIZE);
K_THREAD_STACK_DEFINE(consumer_stack, CONSUMER_THREAD_STACK_SIZE);
K_MUTEX_DEFINE(my_mutex);
// Declare thread data structs
static struct k_thread producer_thread;
static struct k_thread consumer_thread;

K_MSGQ_DEFINE(my_queue, sizeof(uint32_t), QUEUE_SIZE, 4);


void producer_process(void *arg_1, void *arg_2, void *arg_3){
    uint32_t rnd;
    int count=0;
    int ret;
    while(1){
        count+=1;
        rnd = sys_rand32_get();
        ret = k_msgq_put(&my_queue, &rnd, K_NO_WAIT);
		if (ret < 0) {
			printk("Queue full\n");
		}
        printk("Producer data %d\n",rnd);
        if(count%4==0) k_msleep(4000);
        
    }
}

void consumer_process(){
    uint32_t recived_data;
    while(1){
        while (k_msgq_num_used_get(&my_queue) > 0) {
			if (k_msgq_get(&my_queue, &recived_data, K_FOREVER) == 0) {
				printk("Consumer data %d\n",recived_data);
                k_msleep(500);
			}
		}
        
    }
}

int main(void){

    k_tid_t tid_producer;
    k_tid_t tid_consumer;
    tid_producer = k_thread_create(&producer_thread,          // Thread struct
                              producer_stack,            // Stack
                              K_THREAD_STACK_SIZEOF(producer_stack),
                                producer_process,     // Entry point
                                NULL,                   // arg_1
                                NULL,                   // arg_2
                                NULL,                   // arg_3
                                6,                      // Priority
                                0,                      // Options
                                K_NO_WAIT);             // Delay
                                
    
    tid_consumer = k_thread_create(&consumer_thread,          // Thread struct
                              consumer_stack,            // Stack
                              K_THREAD_STACK_SIZEOF(consumer_stack),
                              consumer_process,     // Entry point
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