//######################################################
//
// Project Title : Event Handling & Signaling in Linux
// Task : 2
// Part : 3
// Description : 
// Created by : Sarvesh Patil & Nagarjun Chinnari
// Date : 1 December 2017
// File : task2_3.c
// 
//######################################################

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>


//=======================================
// Signal handler
//=======================================

struct sigaction my_sig_act;
void sighandler(int signo){
	if (signo == SIGUSR1)
   		printf("received SIGUSR1\n============\nExiting Signal Handler\n============\n");
}
//=======================================
// Thread function 
//=======================================

void *tfunc(void *args)
{	
	struct timespec delaytime;
	clock_gettime(CLOCK_MONOTONIC, &delaytime);
	delaytime.tv_sec++;
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&delaytime,0); //Takes time provided in time object as absolue time and thread goes to sleep
	printf("Exiting thread\n============\n");
	return 0;
}
int main(){


	//==================================
    // Signal handler initialization
    //==================================
	
	memset(&my_sig_act,0,sizeof(my_sig_act));
	my_sig_act.sa_handler = &sighandler;
	if(sigaction(SIGUSR1,&my_sig_act,NULL) == -1){
		printf("error occured"); }
	//=========================================
    // Thread Initializations
    //=========================================
	
	pthread_t task_tid; 
	pthread_create(&task_tid,NULL,&tfunc,NULL);

	// Send kill signal to thread
	//============================
	
	pthread_kill(task_tid,SIGUSR1);
	printf("*********\nSIGUSR1 signal sent\n============\n");
	pthread_join(task_tid, NULL);
	printf("Exiting program\n**********\n");
	return 0;
}


//#######################
//
// End of task2_3.c
//
//#######################