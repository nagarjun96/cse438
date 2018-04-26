//######################################################
//
// Project Title : Event Handling & Signaling in Linux
// Task : 2
// Part : 2
// Description : 
// Created by : Sarvesh Patil & Nagarjun Chinnari
// Date : 1 December 2017
// File : task2_2.c
// 
//######################################################

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<semaphore.h>
#include<syscall.h>
//===========================================
// Global variables & functions declaration
//===========================================


sem_t sem;
pthread_t task_tid; 

struct sigaction my_sig_act;

//=======================================
// Signal handler
//=======================================

void sighandler(int signo){
	if (signo == SIGUSR1)
   		{
   			sem_post(&sem);
   			printf("received SIGUSR1\n===============\nExiting Signal Handler\n===========\n");
   		}
}
//=======================================
// Thread function 
//=======================================

void *tfunc(void *args)
{	
	printf("\n*************\nWaiting for semaphore\n===========\n");
	printf("%ld is func id ",syscall(SYS_gettid));
	sem_wait(&sem);
	printf("Exiting thread\n==========\n");
	return 0;
}

//###########################################################################
//=======
// Main
//=======

int main()
{
	//==================================
    // Semaphore initialization
    //==================================
	
	sem_init(&sem,0,1);
	sem_wait(&sem);

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

	pthread_create(&task_tid,NULL,&tfunc,NULL);
	printf("%ld is thread id ",syscall(SYS_gettid));
	sleep(1);

	// Send kill signal to thread
	//============================
	
	pthread_kill(task_tid,SIGUSR1);
	printf("SIGUSR1 signal sent\n=============\n");
	pthread_join(task_tid, NULL);
	printf("Exiting program\n**************\n");
	return 0;
}


//#######################
//
// End of task2_2.c
//
//#######################