//################################################################################################
//
// Program     : Scheduling Assignment 1
// Source file : main.c
// Authors     : Nagarjun chinnari & nisarg trivedi
// Date        : 5 February 2018
//
//################################################################################################

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>
#include <sched.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <errno.h>
#include <linux/input.h>
#include <syscall.h>


#define MAXCHAR 1000   //size of buffer needded to read the text file

//################################################
//
// Global variables & functions declaration area
//
//################################################


const char *mDevice = "/dev/input/event17"; //Storing path of mouse event. Change this event number if your machine shows different number for mouse event
struct input_event ie;


int tret,cret,stp_flag=0;              //stop flag ,error flag
int pi_flag;                           //flag for priority inheritence

int trace_fd = -1;
int marker_fd = -1;


//structure decleration for reading text file using linked list
 struct node{
    int data;
    struct node* next;
};


struct node* start= NULL;
struct node* new_node;
struct node* temp;
struct node* current;




pthread_mutex_t M[10];                          //Mutex array
pthread_mutex_t startm,leftm,rightm;            //Mutex which work with conditional mutex
pthread_cond_t left,right,begin;                //conditional variable for starting the threads at the same time and left,right for handling 
pthread_mutexattr_t AT;                         //Mutex attribute


//###################
//
// Compute Function
//
//###################

void compute(int x)
{
    int i, j=0;
    for (i = 0; i < x; i++)
    {
        j = j + i;
    }
}

//#######################
//
// Thread Functions 
//
//########################

void *p_func(void *arg)                     //periodic thread function with a pointer argument which points to the period in the linked list
{   
    printf("P ID %ld\n",syscall(SYS_gettid));        //gets thread id  of pthread
    int a,b;
    struct node* curr;
    curr=(struct node*)arg;
    a=curr->data;
    struct timespec time,period;                        // adding the period to the structure
    period.tv_sec = a/1000;
   // printf("%ld\n",period.tv_sec);
    period.tv_nsec = (a%1000) * 1000000ul;
   // printf("%ld\n",period.tv_nsec);


    pthread_mutex_lock(&startm);                            // All threads waiting on start condition variable to start simultaneously
    cret = pthread_cond_wait(&begin,&startm);
    if(cret!=0)
        printf("periodic condn error %d\n",cret);
    pthread_mutex_unlock(&startm);

    //sem_wait(&start);
    clock_gettime(CLOCK_MONOTONIC,&time);    
    curr=curr->next;

    while(stp_flag == 0)                                // stop flag becomes 1 when the timer expires
    {
                if (marker_fd >= 0)
        write(marker_fd, "PERIODIC\n", 50);
        time.tv_sec+=period.tv_sec;
        time.tv_nsec+=period.tv_nsec;
        if(time.tv_nsec > 1000000000ul)
        {
            time.tv_sec++;
            time.tv_nsec=0;
        }

        while(curr!=NULL && curr->data !=-1 && curr->data !=-2)            // end of the line is known when -1 or -2 or null occurs
            {

            if( (curr->data/10)<= -48)                                  //   to recognize mutex or computation  
            {
                
                if((curr->data % 10) == -1)                                // -1 represents mutex lock
                {
                    b=(curr->data/-10)-48;
                   
                    pthread_mutex_lock(&M[b]);                              //lock mutex
                        if (marker_fd >= 0)
        write(marker_fd, "LOCKING MUTEX IN AER\n", 50);
                    printf("Locking mutex %d\n",b);
                }

                else if((curr->data% 10) == -2)                             // -2 represents mutex unlock
                {
                    b=(curr->data/-10)-48;
                        if (marker_fd >= 0)
        write(marker_fd, "UNLOCKING MUTEX IN PER\n", 50);
                    pthread_mutex_unlock(&M[b]);                             //unlock mutex
                    printf("Unlocking mutex %d\n",b);
                }
            }

            else
            {                                
                compute(curr->data);                                //calls the compute function for given iterations
                printf("Computing for %d\n",curr->data);
            }

            curr=curr->next;
        }
       clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&time,0); //Takes time provided in time object as absolue time and thread goes to sleep
        curr=(struct node*)arg;                                // pointing to the intial value again
        curr=curr->next;
    }
    printf("Exiting periodic\n");
    pthread_exit(NULL);
}

void *ap_func(void *arg)                                //aperiodic thread function with a pointer argument which points to the event in the linked list
{   

    printf("A ID %ld\n",syscall(SYS_gettid));         //gets thread id  of pthread
    int a,b;
    struct node* curr;
    curr=(struct node*)arg;
    a=curr->data;
    curr=curr->next;
    pthread_mutex_lock(&startm);                            // All threads waiting on start condition variable to start simultaneously
    cret = pthread_cond_wait(&begin,&startm);
    if(cret!=0)
        printf("aperiodic condn error %d\n",cret);
    pthread_mutex_unlock(&startm);
  
    while(stp_flag == 0)                                        // stop flag becomes 1 when the timer expires
    {

            if (marker_fd >= 0)
        write(marker_fd, "APERIDIC\n", 50);

        if(a == 0)                                               //a=0 then left click and a=1 for right click
        {                                            
            pthread_mutex_lock(&leftm);                         // Waits on left click condition variable
            cret = pthread_cond_wait(&left,&leftm);
            if(cret!=0)
                printf("Left Aperiodic condn error %d\n",cret);
            pthread_mutex_unlock(&leftm);

            if(stp_flag==1)
                break;
        }

        else if(a == 1)
        {
            pthread_mutex_lock(&rightm);                             // waits on right click condition variable
            cret = pthread_cond_wait(&right,&rightm);
            if(cret!=0)
                printf("Right Aperiodic condn error %d\n",cret);
            
            pthread_mutex_unlock(&rightm);
    
            if(stp_flag==1)
                break;
       
        }

         while(curr!=NULL && curr->data !=-1 && curr->data !=-2)             // end of the line is known when -1 or -2 or null occurs
            {

            if( (curr->data/10)<= -48)                                  //   to recognize mutex or computation
            {
                if((curr->data % 10) == -1)                             // -1 represents mutex lock
                {
                    b=(curr->data/-10)-48;
                       if (marker_fd >= 0)
                    write(marker_fd, "LOCKING MUTEX\n", 50);
                    pthread_mutex_lock(&M[b]);                          //mutex lock
                   printf("Locking mutex %d\n",b);
                }

                else if((curr->data% 10) == -2)                         // -2 represents mutex unlock
                {
                    b=(curr->data/-10)-48;
                        if (marker_fd >= 0)
        write(marker_fd, "UNLOCKING MUTEX\n", 50);
                    pthread_mutex_unlock(&M[b]);                        //mutex unlock
                    printf("Unlocking mutex %d\n",b);
                }
            }

            else
            {
                compute(curr->data);                                            //calls the compute function for given iterations 
                printf("Computing  for aperiodic Thread  %d \n ",curr->data);       
            }
            
            curr=curr->next;
        }
    curr=(struct node*)arg;                                                         // pointing to the intial value again                                           
        curr=curr->next;
       

       }
    printf("Exiting aperiodic\n");
    
    pthread_exit(NULL);
}
//##############
//
// Mouse thread
//
//##############

void *m_func()
{

    int fd,bytes;
    fd = open(mDevice,O_RDWR);           // Opens mice device file located in /dev/input/mice
    if(fd == -1)                        //Because open returns -1 on occurence of error
    {
        printf("ERROR Opening %s\n", mDevice);
    }

    while(stp_flag == 0)
    {
        bytes = read(fd, &ie, sizeof(struct input_event)); 

            if(bytes > 0)
            {
                 
           
                if((ie.code == BTN_LEFT) && (ie.value == 0))                //brodcasting based on left or right click
                {
                        pthread_cond_broadcast(&left);
                

            
                }    

                else if((ie.code == BTN_RIGHT) && (ie.value == 0))
            {
                      pthread_cond_broadcast(&right);
           
                    }
                          
    }

    }

  
  return 0;
}
//#############################
//
// Main
//
//#############################
int main(void)
{

        char *debugfs;
        char path[256];
        debugfs = "/sys/kernel/debug";
        if (debugfs) {
            strcpy(path, debugfs);  /* BEWARE buffer overflow */
            strcat(path,"/tracing/tracing_on");
            trace_fd = open(path, O_WRONLY);
            if (trace_fd >= 0)
                write(trace_fd, "1", 1);

            strcpy(path, debugfs);
            strcat(path,"/tracing/trace_marker");
            marker_fd = open(path, O_WRONLY);
        }
    
    FILE *fp;               
    char str[MAXCHAR];
    char* filename = "file.txt";
    char *line,*record;
    int num_tid,i;
    struct timespec stop;               // timespec variable for calculating time for terminating main program

     
    printf("Enter 0 to enable Priority Inheritence in mutex and 1 to disable priority inheritence in mutex : \n");
    scanf("%d",&pi_flag);


    fp = fopen(filename, "r");          // opening input file
    if (fp == NULL)
    {
        printf("Could not open file %s",filename);
        return -1;
    }
    while(!feof(fp))                               //traversing till end of the file
    {
   line = fgets(str, MAXCHAR, fp);              // Reading line and storing string in line variable
   record = strtok(line," ");                   //space is delimeter


       while(record != NULL)     
        { 
            new_node = (struct node*) malloc(sizeof(struct node));     //creating a new node of linked list
            if (new_node == NULL)
            printf("nFailed to Allocate Memory");

            new_node->data = atoi(record); 
            new_node->next=NULL;
              
        if(record[0] == 'P')                        // Places -1  if periodic thread
        {
         new_node->data= -1;
            
        }

        else if(record[0] == 'A')                // Places -1  if aperiodic thread
        {
         new_node->data= -2;
            
        }
        if(record[0] == 'L')                       
        {
         new_node->data= -10*record[1]-1;               // 10s digit tells which mutex to lock/unlock and 1s digit tells whether to lock/unlock
            
        }

        else if(record[0] == 'U')
        {
         new_node->data= -10*record[1]-2;
            
        }

             if (start==NULL)                           //creating and storing the values in linked list
            {
                   start=new_node;
                   
                 }
                 else
                 {
                   temp = start;
                     while(temp->next!=NULL)
                     {
                     temp = temp->next;
                     }
                   temp->next = new_node;
                 }

            record = strtok(NULL," ");     
        }
}

num_tid=start->data;


     //==================
    // Thread Creation
    //==================
    

    pthread_t tid[num_tid];         // thread id array
    pthread_t m_tid;                // thread id of mouse click detection thread

    pthread_attr_t att;             // thread attributes
    pthread_attr_t m_att;

    struct sched_param param;

    cret = pthread_cond_init(&begin,NULL);                  // initialising condition variables
    if(cret!=0)
         printf("begin condn init error %d\n",cret);

    cret = pthread_cond_init(&left,NULL);
    if(cret!=0)
         printf("Left condn init error %d\n",cret);

    cret = pthread_cond_init(&right,NULL);
    if(cret!=0)
         printf("Right condn init error %d\n",cret);
    
    pthread_mutex_init(&startm,NULL);                     // Initialising condition variable mutexes(thread mutex are different) // Setting pthread attributes
    //----------------------------
    pthread_mutex_init(&rightm,NULL);
    pthread_mutex_init(&leftm,NULL);
   

    //Setting thread affinity to core 0
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0,&cpuset);

    // Setting pthread attributes
    //----------------------------
    pthread_attr_init(&m_att);
    pthread_attr_setinheritsched(&m_att,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&m_att, SCHED_FIFO);                // setting scheduling priority
    param.sched_priority = 99;                                       // Setting highest priority for mouse detection thread
    pthread_attr_setschedparam(&m_att,&param); 
    int ret1,ret2;
     ret1=pthread_mutexattr_init(&AT);
if(ret1!=0)
{
    printf("attr init error\n");
}
    ret2=pthread_mutexattr_setprotocol(&AT,PTHREAD_PRIO_INHERIT);

if(ret2!=0)
{
    printf("set protocol  error\n");
}

    for(i=0;i<10;i++)
    {
    if(pi_flag == 0)
    {
    
    pthread_mutex_init(&M[i],&AT);

     } 
    else
     {              
     pthread_mutex_init(&M[i],NULL);
     }
     }

   tret=pthread_create(&m_tid,&m_att,m_func,NULL);     //creating mouse thread
    if(tret)
            {
                printf("Error..Retruned with code %d\n", tret );
                exit(-1);
            }
          
    pthread_setaffinity_np(m_tid,sizeof(cpu_set_t),&cpuset);
   
    current=start;
    i=0;


while(current->next!=NULL)
{
        pthread_attr_init(&att);
        pthread_attr_setinheritsched(&att,PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&att, SCHED_FIFO);
        temp=current->next;
        param.sched_priority = temp->data;                                  // Setting priority of rest of threads acc to input file specification
        pthread_attr_setschedparam(&att,&param); 


    temp=temp->next;
        

    if((current->data)==-1)                                                  // create periodic thread if 0th element is -1
    {
        
           tret=pthread_create(&tid[i],&att,p_func,(void *)(temp));
            pthread_setaffinity_np(tid[i],sizeof(cpu_set_t),&cpuset);
            if(tret)
            {
                printf("Error..Retruned with code %d\n", tret );
                exit(-1);
            }
            printf("Create periodic %d \n",i);
             i++; 
             
 
      
       
    }
   else if (current->data==-2)                                                   // create periodic thread if 0th element is -2
    {
        
            tret=pthread_create(&tid[i],&att,ap_func,(void *)(temp));
            pthread_setaffinity_np(tid[i],sizeof(cpu_set_t),&cpuset);
            if(tret)
            {
                printf("Error..Retruned with code %d\n", tret );
                exit(-1);
            }
            printf("Create aperiodic %d\n",i);
            i++;
           // printf("%d\n", temp->data);
        
       
       }
    
    
   
    current=current->next;
}

//Conditional variable & setting timer

sleep(1);                                         //waiting for the threads to get created
pthread_cond_broadcast(&begin);                   


 current=start;
 current=current->next; 

clock_gettime(CLOCK_MONOTONIC,&stop);

stop.tv_sec += current->data/1000;                    //adding time to sleep which is taken from text file
stop.tv_nsec += (current->data%1000)*1000000ul;

if(stop.tv_nsec > 1000000000ul)
{
    stop.tv_nsec = 0;
    stop.tv_sec++;
}

clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&stop,0);  // adds  a timer 

//termination sequence
stp_flag=1;

pthread_cond_broadcast(&right);
pthread_cond_broadcast(&left);
pthread_cond_destroy(&begin);
pthread_cond_destroy(&left);
pthread_cond_destroy(&right);


  for(i = 0; i< num_tid; i++)
   pthread_join(tid[i],NULL);                   // joining all the threads

   printf("Terminated..\n"); 

   write(trace_fd, "0", 1);
   

    return 0 ; 

    
    }


//##################
// End of rtes.c
//##################