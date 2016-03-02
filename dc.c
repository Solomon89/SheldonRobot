/*
gcc -o x_pigpio x_pigpio.c -lpigpio -lrt -lpthread
sudo ./x_pigpio

*** WARNING ************************************************
*                                                          *
* All the tests make extensive use of gpio 25 (pin 22).    *
* Ensure that either nothing or just a LED is connected to *
* gpio 25 before running any of the tests.                 *
*                                                          *
* Some tests are statistical in nature and so may on       *
* occasion fail.  Repeated failures on the same test or    *
* many failures in a group of tests indicate a problem.    *
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "pigpio.h"

#define USERDATA 18249013
#define GPIO 18
#define GPIORM1 18
#define GPIORM2 23
#define GPIOLM1 25
#define GPIOLM2 24

void CHECK(int t, int st, int got, int expect, int pc, char *desc)
{
   if ((got >= (((1E2-pc)*expect)/1E2)) && (got <= (((1E2+pc)*expect)/1E2)))
   {
      printf("TEST %2d.%-2d PASS (%s: %d)\n", t, st, desc, expect);
   }
   else
   {
      fprintf(stderr,
              "TEST %2d.%-2d FAILED got %d (%s: %d)\n",
              t, st, got, desc, expect);
   }
}

void t3(int comand)
{

   int dc[5]={11,21, 41, 61, 81};
   int dc1[5]={5,15, 35, 55, 75};
   
  switch (comand) {
  case 0: //forward
  {
      gpioWrite(GPIOLM1, PI_HIGH);
      gpioPWM(GPIOLM2, dc1[0]);
	  gpioWrite(GPIORM1, PI_HIGH);
      gpioPWM(GPIORM2, dc[0]);
     break;
  }
  case 1: // Right
  {
      gpioWrite(GPIOLM1, PI_HIGH);
      gpioPWM(GPIOLM2, dc1[0]);
	  gpioWrite(GPIORM1, PI_HIGH);
      gpioPWM(GPIORM2, dc[4]);
  break;
  }
  case 2: // Left
  {
      gpioWrite(GPIOLM1, PI_HIGH);
      gpioPWM(GPIOLM2, dc1[4]);
	  gpioWrite(GPIORM1, PI_HIGH);
      gpioPWM(GPIORM2, dc[0]);
  break;
  }
  case 3: // Back
  {
      gpioWrite(GPIOLM2, PI_HIGH);
      gpioPWM(GPIOLM1, dc1[2]);
	  gpioWrite(GPIORM2, PI_HIGH);
      gpioPWM(GPIORM1, dc[2]);
  break;
  }
  default : break;
  }
time_sleep(1);

 gpioWrite(GPIOLM1, PI_LOW);
 gpioWrite(GPIOLM2, PI_LOW);
 gpioWrite(GPIORM1, PI_LOW);
 gpioWrite(GPIORM2, PI_LOW);
  
}


int main(int argc, char *argv[])
{
   int  status;

   int comand = 0;
   if (argc > 1)
   {
  
           comand = atoi(argv[1]); 
           printf("%d\n", comand);
   }
   else {
   }

   status = gpioInitialise();

   if (status < 0)
   {
      fprintf(stderr, "pigpio initialisation failed.\n");
      return 1;
   }
    
   t3(comand);
  

   gpioTerminate();

   return 0;
}

