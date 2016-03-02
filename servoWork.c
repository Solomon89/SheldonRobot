

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

#define GPIO 5
#define GPIO2 22 
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



double convertDegreeToFrequency(int degree)
{
  
   double min = 500.0;
   double max = 2500.0;
   double maxDegree = 180.0;
    if(degree > maxDegree) {
    degree = maxDegree;
   }
   double _return = (max-min)/maxDegree;
   _return = (degree * _return) + min;
   
   return _return;
   
}

void mouveTower(int comand ,int degree)
{
  double frequency = convertDegreeToFrequency(degree);
  switch (comand)
  {
    case 0:
	{
	 gpioServo(GPIO, frequency);
	 break;
	}
	
	case 1:
	{
	 gpioServo(GPIO2, frequency);
	 break;
	}
	default : {gpioServo(GPIO2, frequency); break;}
	
  }
 time_sleep(1);
}




int main(int argc, char *argv[])
{
   int  status;
   int degree =0;
   int comand = 0;
   if (argc > 2)
   {
  
           comand = atoi(argv[1]); 
		   degree = atoi(argv[2]);
           printf("%d\n", degree);
   }
   else {
      comand = 1;
      degree = 90;
   }


   status = gpioInitialise();

   if (status < 0)
   {
      fprintf(stderr, "pigpio initialisation failed.\n");
      return 1;
   }

    mouveTower(comand , degree);
    //t3();


   gpioTerminate();

   return 0;
}

