//######################################################
//
// Project Title : Event Handling & Signaling in Linux
// Task : 1
// Description : Usage of setjmp() & longjmp()
// Created by : Sarvesh Patil & Nagarjun Chinnari
// Date : 1 December 2017
// File : task1.c
// 
//######################################################

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>
#include <linux/input.h>

//====================
// Mouse declarations
//====================
int fd_m;
int t_flag;
const char *MOUSE_DEVICE = "/dev/input/event15"; // Decive file path to read mice events
struct input_event ie;


// setjmp buffer
//===============
jmp_buf buffer1;


//============================================
// Thread function for Mouse event detection
//============================================
void* mouse_tfunc(void* arg)
{
    struct timeval click1,click2;
    unsigned char m_flag = 0;
    long t_sec,t_usec,mtime;

    while((read(fd_m, &ie, sizeof(struct input_event))) && t_flag==0)
    {
      if((ie.type == EV_KEY) && (ie.code == BTN_RIGHT) && (ie.value == 0))
      {
        if (m_flag == 0)
        {
          gettimeofday(&click1,NULL);
          m_flag = 1;
        }
        else
        {
          gettimeofday(&click2,NULL);
          t_sec  = click2.tv_sec  - click1.tv_sec;
          t_usec = click2.tv_usec - click1.tv_usec;
          mtime = ((t_sec) * 1000 + t_usec/1000.0) + 0.5;
          if (mtime < 500)
          {
              printf("\nDouble Right\n");
              t_flag=1;
              kill(0,SIGUSR1);
          }
          else
          {
              click1 = click2;
          }
        }
      }
    }

  return NULL;
}


//=======================================
// Signal handler
//=======================================
void my_sig_handler(int sig_num)
{
   // longjmp point
   longjmp(buffer1,-1);  // Returns buffer value as -1
}


//###########################################################################
//=======
// Main
//=======
int main()
{
    //=========================================
    // Mouse Initialization
    //=========================================
    if((fd_m = open(MOUSE_DEVICE, O_RDONLY)) < 0)
    {
      perror("opening device");
    }

    //=========================================
    // Thread Initializations
    //=========================================
    int tret;
    pthread_t m_tid;

    tret=pthread_create(&m_tid,NULL,mouse_tfunc,NULL);
    if (tret)
    {
      printf("ERROR. Return code from pthread_create() is %d\n", tret);
      exit(-1);
    }
    
    //=========================================
    // Signal Initializations
    //=========================================
    int t_count=0;
    struct sigaction my_sig_act;

    my_sig_act.sa_handler = &my_sig_handler;

    sigaction(SIGUSR1, &my_sig_act, NULL);
    

    // setjump point
    //===============
    int buf_val=setjmp(buffer1);  // Initial buffer value is set as 0

    if(buf_val == 0)
    {
        // Loop to update & print time every second until signal arrives
        while(1)
        {
          t_count = t_count + 1;
          if(t_count == 60)
          {
            printf("One minute completed.\nNormal termination.\n");
            t_flag=1;
            break;
          }
          printf("Time : %d sec\n", t_count);
          sleep(1);
        }
    }
    else
    {
        printf("================================\n");
        printf("Program executed for %d seconds \nbefore termination due to kill signal.\n",t_count);
    }

    printf("-----------\n");
    printf("Terminated.\n");

    return 0;
}

//#######################
//
// End of task1.c
//
//#######################