//######################################################
//
// Project Title : Event Handling & Signaling in Linux
//
// Task : 3
// Part : 1
//
// Description : To check arbitrary capture of process directed signal SIGIO
//               for multiple threads that have signal unblocked
//
// Created by : Sarvesh Patil & Nagarjun Chinnari
//
// Date : 1 December 2017
// File : task3_1.c
// 
//######################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

//==========================================
// Global variables & functions declaration
//==========================================

sigset_t main_mask;
sigset_t thread_mask;

// Thread IDs array
pthread_t task_tid[5] = {0};

int flag=0;



//##############################################################
//=========================
// Utility functions Area
//=========================

// Signal check function
//======================
void sig_chk_func(int sig_num)
{
	if (sig_num == SIGIO)
	{
		printf("=========================================================\n");

		if (pthread_equal(pthread_self(),task_tid[0]))
		{
			printf("Thread 1 signal handler was called by the SIGIO event\n");
		}
		else if (pthread_equal(pthread_self(),task_tid[1]))
		{
			printf("Thread 2 signal handler was called by the SIGIO event\n");
		}
		else if (pthread_equal(pthread_self(),task_tid[2]))
		{
			printf("Thread 3 signal handler was called by the SIGIO event\n");
		}
		else if (pthread_equal(pthread_self(),task_tid[3]))
		{
			printf("Thread 4 signal handler was called by the SIGIO event\n");
		}
		else if (pthread_equal(pthread_self(),task_tid[4]))
		{
			printf("Thread 5 signal handler was called by the SIGIO event\n");
		}
		else
		{
			printf("Error signal handling\n");
		}
	}

	return;
}

//##############################################################
//=========================
// Thread functions Area
//=========================

//=====================
// Thread function 1
//=====================
void* thread_func1(void* arg)
{
	printf("Waiting for signal in Thread 1\n");
	sleep(5);
	sigsuspend(&thread_mask);
	printf("Received signal in Thread 1\n");
	printf("Thread 1 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}


//=====================
// Thread function 2
//=====================
void* thread_func2(void* arg)
{
	printf("Waiting for signal in Thread 2\n");
	sleep(5);
	sigsuspend(&thread_mask);
	printf("Received signal in Thread 2\n");
	printf("Thread 2 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}

//=====================
// Thread function 3
//=====================
void* thread_func3(void* arg)
{
	printf("Waiting for signal in Thread 3\n");
	sleep(5);
	sigsuspend(&thread_mask);
	printf("Received signal in Thread 3\n");
	printf("Thread 3 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}

//=====================
// Thread function 4
//=====================
void* thread_func4(void* arg)
{
	printf("Waiting for signal in Thread 4\n");
	sleep(5);
	sigsuspend(&thread_mask);
	printf("Received signal in Thread 4\n");
	printf("Thread 4 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}

//=====================
// Thread function 5
//=====================
void* thread_func5(void* arg)
{
	printf("Waiting for signal in Thread 5\n");
	sleep(5);
	sigsuspend(&thread_mask);
	printf("Received signal in Thread 5\n");
	printf("Thread 5 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}

//#######################################################################################
//================
// Main
//================

int main()
{
	//=========================================
    // Thread Initializations
    //=========================================
    printf("\nMain waiting for 5 seconds..\n\n");
	pthread_create(&task_tid[0],NULL,&thread_func1,NULL);
	usleep(1);
	pthread_create(&task_tid[1],NULL,&thread_func2,NULL);
	usleep(1);
	pthread_create(&task_tid[2],NULL,&thread_func3,NULL);
	usleep(1);
	pthread_create(&task_tid[3],NULL,&thread_func4,NULL);
	usleep(1);
	pthread_create(&task_tid[4],NULL,&thread_func5,NULL);
	usleep(1);
	sleep(5);

	//=========================================
    // Signal Initializations
    //=========================================
	struct sigaction my_sig_act;

	my_sig_act.sa_handler = &sig_chk_func;

	sigemptyset(&my_sig_act.sa_mask);

	sigaction(SIGIO, &my_sig_act, NULL);

	sigemptyset(&main_mask);
	sigaddset(&main_mask, SIGIO);
	
	sigfillset(&thread_mask);
	sigdelset(&thread_mask, SIGIO);

	// Mask SIGIO for Main
	//=====================
	if(pthread_sigmask(SIG_BLOCK, &main_mask, NULL) != 0)
	{
		printf("Error while masking signal in Main\n");
	}
	

	// Send SIGIO signal
	//==================
	kill(0,SIGIO);
	
	while(flag == 0);

	printf("---------------\n");
	printf("Terminated.\n");
	
	return 0;
}


//=====================
// End of task3_1.c
//=====================