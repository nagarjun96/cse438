User space program schedulability

   Following project is used to check the scheddulability of the given tasksets

Getting Started

    These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 
    See deployment for notes on how to deploy the project on a live system.

Prerequisites

  Linux kernel (preferably linux 2.6.19 and above)
  GNU (preferably gcc 4.5.0 and above)

Installing

Download below files in user directory on your machine running linux distribution.

   1)rtes2.c
   2)Makefile
   3)report.pdf
   4)README.md
   5)file.txt
	


Deployment

   TO RUN ON THE HOST
   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 
   Give Permission to file.xtx using the command "chmod 777 file.txt"
   Use below commands to compile :

   1] Run “make” command to compile rtes2.c 

   2] If there is no error then a object file under the name rtes2 will be created. 

   3] Now run the object file using the command “./rtes2”
   
   

Expected results

   Based upon the  tasksets in file.txt the program tells us whether a given task can be scheduled using earliest deadline first algorithm,rate-monotonic algorithm and deadline monotonic algorithm. 
   And if a tasket is not schedulable then it gives us various information like at which it misses deadline, worst case response time etc.
  

Built With

  Linux 4.10.0-28-generic
  x86_64 GNU/Linux
  64 bit x86 machine

Authors

Nisarg trivedi
Nagarjun chinnari 

License

This project is licensed under the ASU License

