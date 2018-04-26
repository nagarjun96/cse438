//################################################################################################
//
// Program     : Assignment 2
// Source file : rtes2.c
// Authors     : Nagarjun chinnari & nisarg trivedi
// Date        : 3 March 2018
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
//structure decleration for reading text file using linked list

 struct node{
    float data;
    struct node* next;
};

//nodes which are used to traverse,sort,create
struct node* start= NULL;
struct node* new_node;
struct node* temp;
struct node* current;
struct node* load;
struct node* sort4;
struct node* sort5;
struct node* sort6;
struct node* sort1;
struct node* sort2;
struct node* sort3;

int num_tid,i,j,k,l,n,z,num[100];
float sum_u=0,sum_l=0,s_l=0,sum_c=0,a,b,c=0,u,t,sum_ana=0,prev_ana=0;
float deadline=0,d,e,order[100],ef_ut[100];
int flag=0,flag1,flag2=0;
    

void edf_test(void);
void sort_per(void);
void sort_dead(void);
void d_p(void);
void rm_u(int n);
void rm(int n);
int rt_analysis(int n);
void dens_order();
void rm1(void);
void dm1(void);
void dm(int n);

//#############################
//
// Main
//
//#############################
int main(void)
{

    FILE *fp;               
    char str[MAXCHAR];
    char* filename = "file.txt";
    char *line,*record;
     
   

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

            new_node->data = atof(record); 
            new_node->next=NULL;

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

		edf_test();                        /// function which is used to test taskset earliest deadline algorithm
		printf("\n");
		rm1();								//function used to test taskset using rate monotonic algorithm
		printf("\n");
		dm1();								//function used to test taskset using deadline monotonic algorithm
		
	/*	current=start;
while(current->next!=NULL)
{
	current=current->next;
}*/


}


//Rate Monotonic Algorithm for the all the tasks given

void rm1(void)
{
current=start;
j=0;    
sort_per();   // Sorts all the task sets based on their periods
current=start;   // pointing the "current" node to the "start" of the linked list
while(current->next!=NULL)
{
	j++;    // It is the index of the current taskset
	current=current->next;
	n=(int)(current->data);  // n is the size of the taskset
	rm(n);    // finds whether a particular taskset can be scheduled using rate monotonic algorithm
	printf("\n");
		
	for ( i = 0; i < n; i++)
	{
		current=current->next;
		current=current->next;
		current=current->next;
	}
	
}
return;
}

// Deadline Monotonic for all the tasks given

void dm1(void)
{
current=start;
j=0;    // It is the index of the current taskset
sort_dead(); // Sorts all the tasksets based on their deadlines
current=start;		// pointing the "current" node to the "start" of the linked list
while(current->next!=NULL)
{
	j++;
	current=current->next;
	n=(int)(current->data); 	// n is the size of the taskset 
	dm(n);		// finds whether a particular taskset can be scheduled using deadline monotonic algorithm
	printf("\n");   

	for ( i = 0; i < n; i++)
	{
		current=current->next;
		current=current->next;
		current=current->next;
	}
	
}
return;
}

//To find whether the taskset can be scheduled using the dealine monotonic algorithm

void dm(int n)
{
	flag=0; 	
	flag2=0;
	d_p(); 	// Checks whether the deadline is equal to period
	if(flag==0) 
	{
	temp=current;
	rm_u(n); // Calculates the utilization factor and stores it in sum_u and also calculates the value of "c=n(2^(1/n)-1)" 
	if(sum_u<=c)  // checks whether the utilisation factor is less than or equal to 'c' 
	{
		printf("In Tasket %d till task %d are Schedulable by Deadline monotonic and is found by utilization test\n",j,n);
		return ;
	}
	else if(sum_u > 1) 
          {
            printf("In Taskset %d  task %d is not schedulable by Deadline monotonic as utilization exceeds 1\n",j,n);
            if(n>1)
            {
            dm(n-1);  // if nth task is not schedulable it calls the (n-1) task
        	}
        	return;
          }

    // Doing the RT Analysis as sum_u lies between 'c' and 1
    else
    {
    	printf("In DM: rt_analysis of taskset %d\n",j);			
		temp=current;
		flag1=rt_analysis(n);
		if(flag1==1)
		{
			dm(n-1);  	// if nth task is not schedulable it calls the (n-1) task
			return;
		}
		return;
    }

   }


   	// This is the deadline less than period case 
     else 
    {
    	dens_order();  //the function finds if the density order is same as the deadline order
    	if(flag2==1)   // Density order =! deadline order
    	{
	    		temp=current;  // doing the RT analysis
	    		printf("In DM: rt_analysis of Taskset %d\n",j);			
	    		flag1=rt_analysis(n);
	    		if(flag1==1)
				{
					dm(n-1);   // if nth task is not schedulable it calls the (n-1) task
					return;
				}
				return;
    			}
  
    	else  // Density order = deadline order
    	{
			temp=current;
			rm_u(n);  // finding the utilisation factor and value of 'c'
			if(sum_u<=c)   // checks whether the utilisation factor is less than or equal to 'c' 
			{
				printf("In Tasket %d till task %d are Schedulable in Deadline monotonic and is found by utilization test\n",j,n);
				return ;
			}
			else if(sum_u > 1)   // if it is greater than 1
		          {
		            printf("In Taskset %d  task %d is not  schedulable by Deadline monotonic as utilization exceeds 1!!\n",j,n);
		            if(n>1)
		            {
		            dm(n-1);  // if nth task is not schedulable it calls the (n-1) task  
		            return;
					}
		          }
		        
			else   // If it lies between 'c' and 1
			{
				printf("In DM: rt_analysis of Taskset %d\n",j);			
				//RT analysis 
				temp=current;
				flag1=rt_analysis(n);
				if(flag1==1)
				{
					dm(n-1);   // if nth task is not schedulable it calls the (n-1) task  
				}
			}

    	}
   }
    
	}


// For the specific taskset checks whether it is Scedulable with Rate monotonic Algorithm

void rm(int n)
{
	flag=0;
	flag2=0;
	d_p();  // Checks whether the deadline is equal to period 
	if(flag==0)
	{
	temp=current;
	rm_u(n); // Finds sum_u and 'c'
	if(sum_u<=c)
	{
		printf("In Tasket %d till task %d is Schedulable by Rate monotonic and found by utilization test\n",j,n);
		return ;
	}
	else if(sum_u > 1)
          {
            printf("In Taskset %d  task %d is not Rate Monotonic schedulable  as utilization exceeds 1!!\n",j,n);
            if(n>1)
            {
            rm(n-1);  // if nth task is not schedulable it calls the (n-1) task  
         	}
          }
        
	else
	{
		printf("In RM: rt_analysis of Taskset %d\n",j);			
		///RT analysis
		temp=current;
		flag1=rt_analysis(n);
		if(flag1==1)
		{
			rm(n-1);  // if nth task is not schedulable it calls the (n-1) task  
			return;
		}

		return;
	}

    }
    else                // This is the deadline less than period case
    {
    	
    	dens_order();   //checks if density order is equal to period order 
    	if(flag2==1) 
    	{
    			// Calculating the effective Utilization
	    		temp=current;
	    		for(i=0;i<n;i++)
	    		{
	    			temp=temp->next;
	    			a=temp->data;      //execution time 
	    			temp=temp->next;
	    			b=temp->data;     //deadline 
	    			ef_ut[i] = 0;    
	    			temp=temp->next;
	    			num[i]=0;
	    			temp=current;
           			 

           			 for(int k =0;k<=i;k++)
            		{
            			temp=temp->next;
            			a=temp->data;     //execution time 
            			temp=temp->next;
            			c=temp->data;   //deadline
            			temp=temp->next;
            			d=temp->data;   //period
              		

              		if(d<b)        //comparing period of the current task with the task whoes effective utilizatiion is being calculated
                	{
                  		ef_ut[i]+= (a/c);
                  		num[i]++;
                	}
              		else
                	{

                  		ef_ut[i]+= (a/b);

                  		}

                	

            	}
          		    if(ef_ut[i]>1)   //checking if effective utilization is greater than 1 and is the taskset schedulable or not
		    		{
		    			printf("Taskset %d is not schedulable by rm \n",j);
		    			return ;
					}

	    		}


	    		for (i = 0; i < n;i++)     //checking for any value if effective utilization is between n(2^1/n-1)
	    		{
	    			if(ef_ut[i] > (num[i]+1)*(pow(2.0,(1.0/(num[i]+1)))-1))
		    		{
		    			temp=current;
		    			printf("In RM: rt_analysis of Taskset %d\n",j);
		    			flag1=rt_analysis(n);
		    			if(flag1==1)
						{
							rm(n-1);   // if nth task is not schedulable it calls the (n-1) task  
							return;
						}
		    			return;
		    			
					}
				
	    		}
	    		 	printf("Taskset %d is  schedulable by rm  and found using effective utilization test\n",j);
    			}
    	else  // Density order = period order
    	{
			temp=current;
			rm_u(n);  // rate monotonic algorithm for taskset
			if(sum_u<=c)
			{
				printf("In Tasket %d till task %d is Schedulable by Rate monotonic and found by utilization test\n",j,n);
			}
			else if(sum_u > 1)
		          {
		          	printf("In Taskset %d  task %d is not Rate Monotonic schedulable  as utilization exceeds 1!!\n",j,n);
		            if(n>1)
		            {
		            rm(n-1);    // if nth task is not schedulable it calls the (n-1) task  
		            return;
					}
		          }
		        
			else
			{
				printf("In RM: rt_analysis of Taskset %d\n",j);			
				temp=current;
				flag1=rt_analysis(n);
				if(flag1==1)
				{
					rm(n-1);   // if nth task is not schedulable it calls the (n-1) task  
				}
			}

    	}
    }
	
}

// This function calculates the response time analysis and stores in sum_ana and checks if the task is schedulable
int rt_analysis(int n)
{
	sum_ana=0;
	prev_ana=-1;
	
	// Calculating sum_ana
	for ( i = 0; i < n; i++)
	{
		temp=temp->next;
		sum_ana+=temp->data;	
		temp=temp->next;
		d=temp->data;
		temp=temp->next;
		
	}
	while(sum_ana<=d && prev_ana!=sum_ana)
	{	
	prev_ana=sum_ana;
	sum_ana=0;
	temp=current;	
	for(i=0;i<n-1;i++)
	{
		temp=temp->next;
		a=temp->data;
		temp=temp->next;
		temp=temp->next;
		b=temp->data;
		sum_ana+=ceil(prev_ana/b)*a;
	}
	temp=temp->next;
	a=temp->data;
	sum_ana+=a;
    }
    if(sum_ana<=d)
    {
    	printf("In Taskset %d till Task %d are schedulable by RT analysis with a response time of %f\n\n",j,n,sum_ana);
    	return 0;
    }
     printf("Task %d of taskset %d is not schedulable by RT analysis with a response time of %f\n\n",n,j,sum_ana);
     return 1;
}



// checks if deadline = Period and sets the flag
void d_p(void)
{
    temp=current;
	for(i=0;i<n;i++)
	{
		temp=temp->next;
		temp=temp->next;
		a=temp->data;
		temp=temp->next;
		b=temp->data;
		if(a!=b)
		{
			flag=1;
		}

	}

}

//Checks if the density order is equal to period order or deadline order (based on how they are sorted) and sets the flag
void dens_order()
{
	temp=current;
	for(i=0;i<n;i++)
	{
		temp=temp->next;
		temp=temp->next;
		a=temp->data;
		temp=temp->next;
		b=temp->data;
		if(a<b)
		{
			order[i]=a;
		}
		else
		{
			order[i]=b;
		}
	}
	for (i = 0; i < n-1; i++)
	{
		if(order[i+1]<order[i])
		{
			flag2=1;
		}
	}

}

// Calculates the value of 'c' and sum_u

void rm_u(int n)	
{
sum_u=0;
	for(i=0;i<n;i++)
	{
		temp=temp->next;
		a=temp->data;
		temp=temp->next;
		b=temp->data;
		sum_u+=a/b;
		temp=temp->next;

	}
	c=(n*(pow(2.0,(1.0/n))-1));
}


//Function is used to find whether the given set of tasksets are schedulable or not

void edf_test(void)
{

current=start; 
j=0;
while(current->next!=NULL)
{

	current=current->next;
	n=(int)(current->data);   // value of the number of tasks in the task set
	temp=current;
	j++;
	s_l=0;
	sum_l=0;
	sum_u=0;
	flag=0;
	for(i=0;i<n;i++)  // Calculating the utilization factor
	{
		temp=temp->next;
		a=temp->data;
		temp=temp->next;
		b=temp->data;
		sum_u+=a/b;
		temp=temp->next;

	}

	// based on the values of sum_u we see whether the givem task set is schedulable or not 
	if(sum_u<=1)
	{
		printf("tasket %d is Schedulable in edf by utilization test\n",j);
	}
	else
	{

		temp=current;

		//as the utilization factor is inconclusive, we use loading factor analysis below to find whether the set is schedulable or not
		for (i = 0; i < n; i++)
		{
			temp=temp->next;
			sum_l+=temp->data;
			temp=temp->next;
			temp=temp->next;
		}
		
		for(k=0;k<n+1;k++)
		{   
			temp=current;
			for (i = 0; i < n; i++)
		   {
		
			temp=temp->next;
			a=temp->data;
			temp=temp->next;
			temp=temp->next;
			b=temp->data;
			s_l+=ceil(sum_l/b)*a;
	        }
	        sum_l=s_l;
	        s_l=0;
		}
		temp=current;
		deadline=sum_l;
		for ( k = 0; k < n; k++)
		{
			temp=temp->next;
			temp=temp->next;
			t=temp->data;
			while(t<=sum_l)
			{
				u=0;
				load=current;
				for ( i = 0; i < n; i++)
				{
					load=load->next;
					a=load->data;
					load=load->next;
					b=load->data;
					load=load->next;
					c=1;
					if(b<=t)
					{
						c+=(int)((t-b)/load->data);
						u+=c*a;
					}
				}

				u=u/t;
				if(u>1)
				{
					flag=1;
					if(t<deadline)
					{
					deadline=t;
				    }
				}
				load=temp->next;
				t+=load->data;
			}
			temp=temp->next;
			
		}
		if(flag==1)
		{
		printf("Tasket %d is not Schedulable in edf and was found using ld factor test , misses the deadline at %1.1f \n",j,deadline);
		}
		else if (flag==0)
		{
			printf("Tasket %d is  Schedulable in edf and was found using ld factor test\n",j);
		}
	}

	current=temp;

}

}


// Sorting based on periods

void sort_per()
{
	
current=start;

while(current->next!=NULL)
{	
	current=current->next;
	n=(int)(current->data);
	temp=current;
	load=current;		
		// Basic swapping for sorting the tasks

		for (i = 0; i < n-1; i++)
		{
			for (k = 0; k < n-i-1; k++)
			{
				temp=temp->next;
				sort1=temp;
			    temp=temp->next;
			    sort2=temp;
			    temp=temp->next;		
				sort3=temp;
			    a=sort3->data;
				temp=temp->next;
				sort4=temp;
				temp=temp->next;
				sort5=temp;
				temp=temp->next;
				sort6=temp;
				b=temp->data;
				
			if(a>b)
			{
				c=sort4->data;
				d=sort5->data;
				e=sort6->data;
				sort4->data=sort1->data;
				sort5->data=sort2->data;
				sort6->data=sort3->data;
				sort1->data=c;
				sort2->data=d;
				sort3->data=e;
				
				    
			}
			temp=sort3;
			}

			temp=current;
		}

for(i=0;i<n;i++)
{
	load=load->next;
	load=load->next;
	load=load->next;
}
current=load;


}		
}		



// Sorting based on deadlines
void sort_dead()
{
	
current=start;

while(current->next!=NULL)
{	
	current=current->next;
	n=(int)(current->data);
	temp=current;
	load=current;		
		
		// Basic swapping for sorting the tasks according to deadlines
		for (i = 0; i < n-1; i++)
		{
			for (k = 0; k < n-i-1; k++)
			{
				temp=temp->next;
				sort1=temp;
			    temp=temp->next;
			    sort2=temp;
			    a=temp->data;
			    temp=temp->next;		
				sort3=temp;
				temp=temp->next;
				sort4=temp;
				temp=temp->next;
				sort5=temp;
				b=temp->data;
				temp=temp->next;
				sort6=temp;
				
				
			if(a>b)
			{
				c=sort4->data;
				d=sort5->data;
				e=sort6->data;
				sort4->data=sort1->data;
				sort5->data=sort2->data;
				sort6->data=sort3->data;
				sort1->data=c;
				sort2->data=d;
				sort3->data=e;
				
				    
			}
			temp=sort3;
			}

			temp=current;
		}

for(i=0;i<n;i++)
{
	load=load->next;
	load=load->next;
	load=load->next;
}
current=load;
}		
}		