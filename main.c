//################################################################################################
//
// Program     : Assignment 3
// Source file : main.c
// Authors     : Nagarjun chinnari & nisarg trivedi
// Date        : 30 March 2018
//
//################################################################################################

#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>
#include <pwm.h>
#include <pinmux.h>
#include <kernel.h>
#include <asm_inline_gcc.h>
#include <shell/shell.h>

//the corresponding zephyr pin number for GPIO 13 of IO2 
#define LED1  5

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

//Defining the stack size
#define STACKSIZE 1024

//Definig the priority of threads
#define PRIORITY1 7
#define PRIORITY2 8

//Defining the flag for rising edge
#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH)

//Initialising the variables and their data types
long long start_time;
long long stop_time;
long long cycles_spent;
long long int_lat[500];
long long int_latbg[500];
long long context[500];
int count;


//defining the data type for the message queue
struct data_item_type {
    char field[5];
};

struct data_item_type data;

char my_msgq_buffer[10 * sizeof(data)];
char msg[5] = "abcd";


//Initializing the mutex and semaphores
struct k_mutex my_mutex;
struct k_sem my_sem;
struct k_sem my_sem1;
struct k_sem my_sem2;
struct k_sem my_sem3;
struct k_msgq my_msgq;

//initializing the device
struct device *dev2;


//Shell function for the printing of the collected data
static int shell_cmd_params(int argc, char *argv[])
{
	int cnt;
	for (cnt = 0; cnt < 100; cnt++) {
		printk("%lld \n", context[cnt]);
	}
	return 0;
}

#define MY_SHELL_MODULE "print_module"

static struct shell_cmd commands[] = {
	{ "print", shell_cmd_params, "print argc" },
	{ NULL, NULL, NULL }
};


//GPIO callback function: interrupt occurs on this pin, it goes into the written function
void callback_gpio(struct device *dev2, struct gpio_callback *cb,
		    u32_t pins)
{
	stop_time=_tsc_read();  //Time stamp counter value saved in stop_time variable
	cycles_spent=stop_time-start_time;
	printk("%lld \n",cycles_spent); //in ticks
	count=count+1;
}

// Structure for callback
static struct gpio_callback dev2_cb;


//The higher priority thread which writes in the message queue
void blink1(void)
{
	k_sem_take(&my_sem3, K_FOREVER);  //Semaphore has been taken
	printk("\n Entering the writing thread \n");
	while(1)
	{
		   start_time=_tsc_read();
		  k_msgq_put(&my_msgq, msg, K_FOREVER);   // adding a value to the message queue 

		if(count==500)    // if the count equals 500, it exits the thread
		{
			break;
		}
		
	}
	gpio_pin_disable_callback(dev2,LED1);   //Disabling the callback function
	printk("\n Exiting the writing thread \n");
	
}


//The lower priority thread which reads from the message queue
void blink2(void)
{
	k_sem_take(&my_sem2, K_FOREVER);
	printk("\n Inside the reading thread \n");
	gpio_pin_enable_callback(dev2,LED1);

	while(1)
	{
		 start_time=_tsc_read();
		k_msgq_get(&my_msgq, msg , K_FOREVER);   //reading the value from the message queue
		 start_time=_tsc_read();
		if(count==500)   // if the count equals 500, it exits the thread
		{
			break;
		}
		
			
	}
	printk("\n Exiting the reading thread");
	k_sem_give(&my_sem2);

}


// higher priority thread used on context switch
void blink3(void)
{
	k_sem_take(&my_sem, K_FOREVER);

while(count<=100)	
{
k_sleep(3);	
k_mutex_lock(&my_mutex, K_FOREVER);  //locking the mutex
stop_time = _tsc_read();
cycles_spent = stop_time - start_time;  //calculating the overhead
context[count]=10*(cycles_spent/4); //converting the ticks to nanoseconds
k_mutex_unlock(&my_mutex);  //unlocking the mutex
printk("%lld\n",context[count]);
count=count+1;
k_sleep(5);
}
 k_sem_give(&my_sem);
}


//lower priority thread used for context switching
void blink4(void)
{
	k_sem_take(&my_sem1, K_FOREVER);
	
while(count<=100)	
{
    k_mutex_lock(&my_mutex, K_FOREVER); //locking the mutex
    k_sleep(10);
	start_time = _tsc_read(); 
	k_mutex_unlock(&my_mutex); // unlocking the mutex
	k_sleep(10);
}
	
}

	
//#############################
//
// Main
//
//#############################

void main(void)
{
	int c,d,e;  // error variables
	struct device *dev3;  // defining the device pointers
	struct device *dev4;


	k_msgq_init(&my_msgq, my_msgq_buffer,sizeof(data), 10);  // message queue is initialized
    	

//Semaphores and mutex are initialized

    k_sem_init(&my_sem, 0, 1);
	k_sem_init(&my_sem1, 0, 1);
    k_sem_init(&my_sem2, 0, 1);
    k_sem_init(&my_sem3, 0, 1);
	k_mutex_init(&my_mutex);

	k_msgq_init(&my_msgq, my_msgq_buffer,5, 10);

	dev2=device_get_binding("GPIO_0"); //Device binding
	dev3=device_get_binding("EXP1");
	dev4=device_get_binding("PWM0");

	//configuring the GPIO pins

	gpio_pin_configure(dev4,13 , GPIO_DIR_IN | GPIO_INT | EDGE);

    gpio_pin_configure(dev3,2 , GPIO_DIR_IN | GPIO_INT | EDGE);

	c=gpio_pin_configure(dev2, LED1, GPIO_DIR_IN | GPIO_INT | EDGE);
	

	if(c<0)
	{
		printk("error \n");
	}

	//Initializing the callback

	gpio_init_callback(&dev2_cb, callback_gpio, BIT(LED1));
	
	d=gpio_add_callback(dev2, &dev2_cb);
    
    if(d<0)
    {
	printk("error\n");
    }

	//Enabling callback
	e=gpio_pin_enable_callback(dev2, LED1);
	
	if(e<0)
	{
		printk("error\n");
	}
	
	k_sem_give(&my_sem);
    k_sem_give(&my_sem1);
    k_sem_take(&my_sem, K_FOREVER);
    
	int a,b;
	struct device *pwm_dev;  //defining PWM and Pinmux device pointers
	struct device *pinmux;

	pinmux=device_get_binding(CONFIG_PINMUX_NAME);

	a=pinmux_pin_set(pinmux, 5, PINMUX_FUNC_C);  // pinmux setting

	if(a<0)
	{
		printk("error");
	}

	pwm_dev=device_get_binding(CONFIG_PWM_PCA9685_0_DEV_NAME);

	
	b=pwm_pin_set_cycles(pwm_dev,3,4095,2000); //PWM on IO5 is generated
	
	if(b<0)
	{
		printk("error\n");
	}
		
	count=0;
	while (count<=500)  // reading for 500 values 
	{
		start_time=_tsc_read();
	}
	
	gpio_pin_disable_callback(dev2, LED1); // disabling callback
	k_sleep(1000);
	k_sem_give(&my_sem2);
	k_sem_give(&my_sem3);
	count=0;
	k_sem_take(&my_sem2, K_FOREVER);

	gpio_pin_disable_callback(dev2, LED1); //disabling callback
    SHELL_REGISTER(MY_SHELL_MODULE, commands);

}

//defining K_thread for calculating context switch and interrupt latency with background task
K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL,
		PRIORITY1, 0, K_NO_WAIT);
K_THREAD_DEFINE(blink_id, STACKSIZE, blink2, NULL, NULL, NULL,
		PRIORITY2, 0, K_NO_WAIT);

K_THREAD_DEFINE(blink3_id, STACKSIZE, blink3, NULL, NULL, NULL,
		PRIORITY1, 0, K_NO_WAIT);
K_THREAD_DEFINE(blink4_id, STACKSIZE, blink4, NULL, NULL, NULL,
		PRIORITY2, 0, K_NO_WAIT);