//######################################################
//
// Project Title : Event Handling & Signaling in Linux
// Task : 2
// Part : 1
// Description : 
// Created by : Sarvesh Patil & Nagarjun Chinnari
// Date : 1 December 2017
// File : task2_1.c
// 
//######################################################

#define _GNU_SOURCE
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<sched.h>


//===========================================
// Global variables & functions declaration
//===========================================

pthread_t task_tid; 
struct sched_param param;
int tchk = 0;

struct sigaction my_sig_act;

//=======================================
// Signal handler
//=======================================
void sighandler(int sig_num)																			
{
	if (sig_num == SIGUSR1)
   	{
   		tchk = 1;
   		printf("SIGUSR1 signal received..\n");
   		printf("Exiting Signal Handler..\n");
   	}
}


//=======================================
// Thread function for SIGUSR1 signal
//=======================================
void *signal_tfunc(void *args)
{	
	
	if(tchk == 1)
	printf("Exiting from thread\n");
	return 0;
}

//###########################################################################
//=======
// Main
//=======
int main()
{
	int ret;
	long i = 0;
	// Set all threads to run on core 1
	//===================================
	cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0,&cpuset);
    pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&cpuset);
    //==================================
    // Signal handler initialization
    //==================================
	memset(&my_sig_act,0,sizeof(my_sig_act));
	my_sig_act.sa_handler = &sighandler;

	if(sigaction(SIGUSR1,&my_sig_act,NULL) == -1){
		printf("Error in sigaction"); }
	

	//=========================================
    // Thread Initializations
    //=========================================
	

	pthread_create(&task_tid,NULL,&signal_tfunc,NULL);
	pthread_setaffinity_np(task_tid,sizeof(cpu_set_t),&cpuset);
	
	param.sched_priority = 60;
	ret =pthread_setschedparam(pthread_self(), SCHED_FIFO, &param); 
	if(ret != 0)
		printf("Error in Main priority\n"); 

    printf("Assigning priorities in main: Completed\n");

    param.sched_priority = 30;
	ret = pthread_setschedparam(task_tid, SCHED_FIFO, &param);
	if(ret != 0)
		printf("Error in task_tid priority\n"); 

	

	//printf("Main function is going to sleep..\n");
	//sleep(1);

	//printf("---------------------------------------\n");
	//printf("Main executing again preempting thread.\n");

	// Send kill signal to thread
	//============================
	pthread_kill(task_tid,SIGUSR1);
	printf("SIGUSR1 signal sending : Completed\n======================================\n");
	printf("working in main\n");
	while( i< 99999999)  
	{
		i=i+2;
	}                                     // Keeping main busy to see handler cannot function till main gives away control
		
	
	printf("Main is waiting for Thread to return..\n=====================================\n");
	pthread_join(task_tid, NULL);
	printf("Thread returned successfully.\n");
	printf("Program Terminated.\n");

	return 0;
}

//#######################
//
// End of task2_1.c
//
//#######################