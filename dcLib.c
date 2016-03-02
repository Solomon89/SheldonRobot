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

#define GPIORM1 18
#define GPIORM2 23
#define GPIOLM1 25
#define GPIOLM2 24

int max[] = {81,75};
int min[] = {11,5};
   
void MotorMotion(double leftMotorProsentage, double rightMotorPersentage)
{
  int i = 0;
  int leftMotorFrequency = ((max[i]-min[i])/100) * (100 - leftMotorProsentage);
  i++;
  int rightMotorFrequency = ((max[i]-min[i])/100) * (100 - rightMotorPersentage);

  gpioWrite(GPIOLM1, PI_HIGH);
  gpioPWM(GPIOLM2, leftMotorFrequency);
  gpioWrite(GPIORM1, PI_HIGH);
  gpioPWM(GPIORM2, rightMotorFrequency);
   
  
if(leftMotorProsentage == 0 && rightMotorPersentage == 0)
{
 gpioWrite(GPIOLM1, PI_LOW);
 gpioWrite(GPIOLM2, PI_LOW);
 gpioWrite(GPIORM1, PI_LOW);
 gpioWrite(GPIORM2, PI_LOW);
  }
}
void InitialiseDC()
{
 int  status;
 status = gpioInitialise();
 if (status < 0)
   {
      fprintf(stderr, "DC pigpio initialisation failed.\n");
   }
}


