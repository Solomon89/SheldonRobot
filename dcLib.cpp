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
extern "C"
{
	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <string.h>
	#include <ctype.h>

	#include "pigpio.h"
}
#include <stdio.h>
#include <iostream>
#include <map>
#include <math.h> 

using namespace std;

#define GPIORM1 18
#define GPIORM2 23
#define GPIOLM1 25
#define GPIOLM2 24

int maxim[] = {81,81};
int minim[] = {11,11};
   
void InitialiseDC()
{
 int  status;
 status = gpioInitialise();
 if (status < 0)
   {
      fprintf(stderr, "DC pigpio initialisation failed.\n");
   }
}
   
void MotorMotion(double leftMotorProsentage, double rightMotorPersentage)
{
  
  int i = 0;
  int leftMotorFrequency = ((maxim[i]-minim[i])/100.0) * (100.0 - abs(leftMotorProsentage));
  i++;
  int rightMotorFrequency = ((maxim[i]-minim[i])/100.0) * (100.0 - abs(rightMotorPersentage));
 
  cout << "L:" << leftMotorFrequency << "R:" << rightMotorFrequency << endl;
  
  if (leftMotorProsentage > 0)
  {
	  gpioWrite(GPIOLM1, PI_HIGH);
	  gpioPWM(GPIOLM2, leftMotorFrequency);
  }
  else
  {
	  gpioWrite(GPIOLM2, PI_HIGH);
	  gpioPWM(GPIOLM1, leftMotorFrequency);	
  }
  if (rightMotorPersentage > 0)
  {
		gpioWrite(GPIORM1, PI_HIGH);
		gpioPWM(GPIORM2, rightMotorFrequency);
  }
  else
  {
		gpioWrite(GPIORM2, PI_HIGH);
		gpioPWM(GPIORM1, rightMotorFrequency);
  }
  
	if(leftMotorProsentage == 0 && rightMotorPersentage == 0)
	{
	 gpioWrite(GPIOLM1, PI_LOW);
	 gpioWrite(GPIOLM2, PI_LOW);
	 gpioWrite(GPIORM1, PI_LOW);
	 gpioWrite(GPIORM2, PI_LOW);
	}
}



