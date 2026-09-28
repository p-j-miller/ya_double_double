/* Very simple example of the use of ya_double_double.h,  save as example.c */
#include <stdio.h>
#include "../ya_double_double/ya_double_double.h"
int main(int argc, char *argv[])
{ DoubleDouble x={1,0},y={3,0},z;// x=1, y=3
  z=div_dd_dd(x,y); // 1/3
  printf("1/3=%.32g,%.20g\n",z.hi,z.lo);// assumes %.32g will actually print 32 accurate digits - gcc is normally OK for this
  printf("As doubles 1/3=%.20g\n",(double)1.0/(double)3.0);
}
