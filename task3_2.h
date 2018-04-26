//######################################################
//
// Project Title : Event Handling & Signaling in Linux
// Task : 3
// Part : 2
// Created by : Sarvesh Patil & Nagarjun Chinnari
// Date : 1 December 2017
// File : task3_2.h
// 
//######################################################

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

//==========================================
// Global variables & functions declaration
//==========================================

int i=0,len=0;
pthread_t tid_arr[5]={0};

// Thread register function
//==========================
void register_tid(pthread_t new_tid,int t_num)
{
	tid_arr[t_num]=new_tid;
}

// Function to send thread directed kill
//========================================
void signal_tid(pthread_t sig_tid)
{
	printf("\n");
	for(int j=0; j<5; j++)
	{
		if(tid_arr[j]!=0 && tid_arr[j]!=sig_tid)
		{
			printf("Sending kill signal for : %d\n", j+1);
			pthread_kill(tid_arr[j],SIGIO);
		}
	}
}
//=====================
// End of task3_2.h
//=====================