#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "pigpio.h"
#include <wiringPi.h>

#define USERDATA 18249013

#define TRIG 6
#define ECHO 13
 enum Sensors{LFS = 0 ,RFS = 1,FRS = 2,BRS = 3};
 int Trigs[] = {16,21,26,6};
 int Echos[] = {12,20,19,13};
 


int getCM(int Sensor) {
	int trig = Trigs[Sensor];
    int echo = Echos[Sensor];
     //Send Trig pulse
    gpioWrite(trig, PI_HIGH);
    time_sleep(0.00001);
    gpioWrite(trig, PI_LOW);
    //Wait for Echo start
    while(gpioRead(echo) == PI_LOW);
    //Wait for Echos end
    long startTime = micros();
    while(gpioRead(echo) == PI_HIGH && micros() - startTime < 2900 );
    long travelTime = micros() - startTime;
 
        //Get distance in cm
    int distance = travelTime / 58;
 
    return distance;
}

void initialiseSonars()
{
int  status;
 status = gpioInitialise();
 if (status < 0)
   {
      fprintf(stderr, "Sonar pigpio initialisation failed.\n");
   }
   int i = 0;
   for (i=0;i<4;i++)
   {
    gpioSetMode(Echos[i], PI_INPUT);
    gpioSetMode(Trigs[i], PI_OUTPUT);
   }
}
