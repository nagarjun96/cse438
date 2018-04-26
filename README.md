User space program Real-time task models

   Following project is used to create real time task models(periodic,aperiodic threads).

Getting Started

    These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 
    See deployment for notes on how to deploy the project on a live system.

Prerequisites

  Linux kernel (preferably linux 2.6.19 and above)
  GNU (preferably gcc 4.5.0 and above)

Installing

Download below files in user directory on your machine running linux distribution.

   1)rtes1.c
   2)Makefile
   3)report.pdf
   4)README.md
	

Ensure that 666(rw- rw- rw-) file permissions exist for /dev/input/event2.

you can check by the following command 
ls -lrt /dev/input/event2. 

else change using
sudo chmod 666 /dev/input/event2

Deployment
   TO RUN ON THE HOST
   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

   Use below commands to compile :

   1] Run “make” command to compile rtes1.c 

   2] If there is no error then a object file under the name rtes1 will be created. 

   3] Now run the object file using the command “./rtes1”
   
   TO RUN ON GALILEO BOARD

   Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

   In the make file we gave the path as "/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux" for compiler

   if you have a different location then change it.
   
   Use below command to to compile 
 
   make TEST_TARGET=Galileo2

   Now send the object file to the galileo
   board using the follwing command (change IP & home
   accordingly)

   sudo scp rtes1 root@192.168.0.100:/home

   Connect to Galileo board with root login

   Mouse events are detected through /dev/input/eventX device
   file. Here header is written for /dev/input/event2 file
   Check on which file on your board & change in rtes1.c
   accordingly.

   On Galileo2 board, ensure that 666(rw- rw- rw-) file
   permissions exist for /dev/input/event2.

   You can check by the following command 
   ls -lrt /dev/input/event2. 

   Otherwise change using the following command 
   chmod 666 /dev/input/event2


   Once above completed then run the below command to execute
   the program code
   
   cd /home
   ./rtes1


Expected results

   It asks for whether priority inheritence should be enabled or disabled enter 0 for enabling and 1 vice-versa, now based upon the input text file it creates a number of periodic and aperiodic threads    	and runs them for the given execution time. 
  

Built With

  Linux 4.10.0-28-generic
  x86_64 GNU/Linux
  64 bit x86 machine

Authors

Nisarg trivedi
Nagarjun chinnari 

License

This project is licensed under the ASU License

