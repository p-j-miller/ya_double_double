# ya_double_double
C header only library providing basic double-double operations in an easy to use way

These routines are more accurate than just using double variables, and in most cases are more accurate than using __float128's - see the Algorithms section below for references giving the error bounds of the algorithms used.

Note these routines ignore special cases (inf, nan arguments, overflows, divides by zero, etc). 
Code that does deal with these situations is in the repository "double-double" (but those functions have a different calling convention and are slower than the functions here).
 
Note that the order of hi & lo in a DoubleDouble is normally hi,lo, but can be changed to lo,hi by defining YA_DD_LO_FIRST before including this header file 
 
Recommended  Gcc setup  is  (minimum Windows 11 PC + avx2 for fma):
	 -msse4.2
	-mfpmath=sse
	-mavx2
	-mfma
	-O3
	-fno-math-errno
	-DNDEBUG

main.c is the test program.

# Types
DoubleDouble - the fundamental type which hold two doubles ".hi" and ".lo". 

By default the values are stored "hi,lo", but if YA_DD_LO_FIRST is defined before including ya_double_double.h" then the order is "lo,hi".
This only matters if a variable is initialized when its declared.
# Functions
~~~
__float128 dd_to_f128(DoubleDouble x); // convert a double-double to  a float128 (if the __float128 type is available)
DoubleDouble minus_dd(DoubleDouble x); // returns - x
DoubleDouble fast2sum(double a,double b);  // adds a and b to give double double "x". Requires |a| >= |b|
DoubleDouble exact_add(double a,double b); // general add a+b => double double
DoubleDouble add_dd_dd(DoubleDouble x,DoubleDouble y); // add two double-double variables
DoubleDouble add_dd_d(DoubleDouble a,double b); // add double-double and double variables
DoubleDouble exact_mult(double a,double b); // multiply double * double-double to give a double-double result
DoubleDouble mult_dd_dd(DoubleDouble a,DoubleDouble b); // multiply double-double * double-double to give a double-double result
DoubleDouble exact_mult_dd_dd(DoubleDouble a,DoubleDouble b);// This can give a more accurate result than mult_dd_dd (but is slower)
DoubleDouble FMA_dd_dd_dd(DoubleDouble a,DoubleDouble b,DoubleDouble c); // returns a*b+c with only 1 rounding
DoubleDouble div_dd_dd(DoubleDouble a,DoubleDouble b); // divide double-double / double-double to give a double-double result
~~~
# Algorithms
See https://github.com/p-j-miller/double-double which includes references to the core algorithms used and their error bounds.

FMA routine implements Algorithm 4 from
"Extended-Precision FMA under Parameterized Double-Word Overlap:
Tight Error Bounds and Examples by Claude-Pierre Jeannerod, Mioara Joldes,
Nicolas Louvet, Jean-Michel Muller, published in the proceedings of
Arith 2026, https://inria.hal.science/hal-05517451.

Division see  "High Precision Division and Square Root", Alan H. Karp and Peter Markstein, 
ACM Transactions on Mathematical Software Volume 23, Issue 4, Dec 1997 (Originally published as HP Labs Report 93-93-42 (R.1) Oct 1994).

Both FMA and Division are based on implementations in the CORE-MATH project - https://core-math.gitlabpages.inria.fr/
# Installation
It is recommended that the files from this repository are placed in a directory called ya_double-double

The test program can be compiled on Linux , or on Windows using for example WinLibs gcc from https://winlibs.com/ with 
~~~
gcc main.c -lm -lquadmath
~~~
When executed as "./a.out" on Linux or "a" on Windows the last line of the output should read "All tests passed"
# Example
~~~
/* Very simple example of the use of ya_double_double.h,  save as example.c */
#include <stdio.h>
#include "../ya_double_double/ya_double_double.h"
int main(int argc, char *argv[])
{ DoubleDouble x={1,0},y={3,0},z;// x=1, y=3
  z=div_dd_dd(x,y); // 1/3
  printf("1/3=%.32g,%.20g\n",z.hi,z.lo);// assumes %.32g will actually print 32 accurate digits - gcc is normally OK for this
  printf("As doubles 1/3=%.20g\n",(double)1.0/(double)3.0);
}
~~~
This can be compiled with gcc on Linux, or on Windows using for example WinLibs gcc from https://winlibs.com/ 
~~~
gcc -m64 example.c -lm
~~~
and then executed as "./a.out" on Linux or "a" on Windows to give:
~~~
1/3=0.33333333333333331482961625624739,1.8503717077085941313e-17
As doubles 1/3=0.33333333333333331483
~~~
Adding "z.hi" and "z.lo" will give a higher resolution result, in this case it gives 0.33333333333333333333333333333333 , which has 32 3's, twice as many as the double result (16).
Remember the exact result is an infinite number of 3's. 

See the algorithms section above for references giving the accuracy of the results ("hi"+"lo" is always significantly more accurate than just using a double, and in most cases is more accurate than using __float128's (and more portable as not all compilers support __float128's)).
