User space program to control event handling and signaling

   Following project is used to control event handling and signaling.

Getting Started

    These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 
    See deployment for notes on how to deploy the project on a live system.

Prerequisites

  Linux kernel (preferably linux 2.6.19 and above)
  GNU (preferably gcc 4.5.0 and above)

Installing

Download zip file cse438-team20-assgn04.zip and unzip in user directory on your machine running linux distribution and you will get the following folders.



1)Task_1
2)Task_2
3)Task_3


Deployment for Task_1

   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

   Use below command to to compile 
 
   make

   Mouse events are detected through /dev/input/eventX device
   file. Here header is written for /dev/input/event4 file
   Check on which file on your board & change in task1.c
   accordingly.

   On Host, ensure that 666(rw- rw- rw-) file
   permissions exist for /dev/input/event2.

   You can check by the following command 
   ls -lrt /dev/input/event2. 

   Otherwise change using the following command 
   chmod 666 /dev/input/event2

   Once above completed then run the below command to execute
   the program code
   
   ./task1

Expected results

   prints the time after for every one second and terminates after 60 seconds if no mouse event is detected
   and if a double click occurs under 60 seconds it terminates the program


Deployment for Task2_1

   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

   Use below command to to compile 
 
   make

   Once above completed then run the below command to execute
   the program code
   
  sudo ./task2_1

Expected results

    main thread's priority , scheduling policy is set and a thread is created 
    whose priority is set lower than that of the main thread now a signal is generated in 
    main thread which will happen only after the main thread completes its process and then kills the runnable 
    thread 
     
   
Deployment for Task2_2

   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

   Use below command to to compile 
 
   make

   
   Once above completed then run the below command to execute
   the program code
   
   ./task2_2

Expected results
   
   A thread is a created and a semaphore is intialized, now when the  signal is sent the thread it is killed and program exits.


Deployment for Task2_3

   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

   Use below command to to compile 
 
   make

   Once above completed then run the below command to execute
   the program code
   
   ./task2_3

Expected results

  A thread is created and nanaosleep is given , now when the signal is sent to the thread it is killed and program exits.

   
Deployment for Task3_1

   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

   Use below command to to compile 
 
   make

   Once above completed then run the below command to execute
   the program code
   
   ./task3_1

Expected results

  Intially five threads are created , Now a signal is generated which is sent  one of the five threads
  randomly and kills it.

	
 
Deployment for Task3_2

   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

   Use below command to to compile 
 
   make

   Once above completed then run the below command to execute
   the program code
   
   ./task3_2

Expected results

  
  Intially five threads are created out of which three are unblocked and two are blocked, 
  also the main thread is blocked, Now on arrival of the signal one of the unblocked thread
  is picked up randomly and killed after which remaining two unblocked threads are killed

Built With

  Linux 4.10.0-28-generic
  x86_64 GNU/Linux
  64 bit x86 machine

Authors

Sarvesh Patil 
Nagarjun chinnari 

License

This project is licensed under the ASU License

