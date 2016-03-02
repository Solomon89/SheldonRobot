#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "dcLib.h"
#include "SonarLib.h"

int main(int argc, char *argv[])
{
   initialiseSonars();
   InitialiseDC();
   
   int LFSValue = getCM(0);
   int RFSValue = getCM(1);
   while(LFSValue > 5 || RFSValue > 5)
   {
       LFSValue = getCM(0);
       RFSValue = getCM(1);
	   printf("LFS = %d RFS = %d \n",LFSValue,RFSValue);
	   MotorMotion(20,20);
   }
   MotorMotion(0,0);
}
