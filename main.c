/* Basic test program for double-doubles
   Written by Peter Miller 1-12-2025
   This version for ya_double_double 18-9-2026
   
   Tests check fma (which is assumed present and working by the code), then check double-double multiply and subtract, finally divide.
   These check the "lower level" double-double functions as well.

Compile with:

 gcc main.c -lm -lquadmath

 expected output:
sizeof float=4 double=8 long double=16
__SIZEOF_FLOAT__ is defined and set to 4
__SIZEOF_DOUBLE__ is defined and set to 8
__SIZEOF_LONG_DOUBLE__ is defined and is set to 16
__SIZEOF_FLOAT128__ is defined and set to 16
__SIZEOF_INT128__ is defined and set to 16
number of bits in mantissa of float is FLT_MANT_DIG=24  and max exponent is 2^FLT_MAX_EXP=128 max exponent (decimal)=FLT_MAX_10_EXP=38
number of bits in mantissa of double is DBL_MANT_DIG=53  and max exponent is 2^DBL_MAX_EXP=1024 max exponent (decimal)=DBL_MAX_10_EXP=308
number of bits in mantissa of long double is LDBL_MANT_DIG=64  and max exponent is 2^LDBL_MAX_EXP=16384 max exponent (decimal)=LDBL_MAX_10_EXP=4932
 max difference between 1 and next larger representable number for doubles is  DBL_EPSILON=((double)2.22044604925031308084726333618164062e-16L)
number of bits in mantissa of float128 is __FLT128_MANT_DIG__=113  and max exponent is 2^__FLT128_MAX_EXP__=16384 max exponent (decimal)=__FLT128_MAX_10_EXP__=4932
__GNUC__ defined, __GNUC__=13 __GNUC_MINOR__=3
__MINGW32__ is not defined
__x86_64 defined and is set to 1
__SSE4_2__ defined and is set to 1
__AVX2__ defined and is set to 1
__FMA__ defined and is set to 1
Checking fma functions:
double (DBL_EPSILON = 2.22044604925031308084726333618164062e-16 so e^2=4.9303806576313237838233035330172e-32 :
1+epsilon=1.00000000000000022e+00 1+2*epsilon=1.00000000000000044e+00
   direct=0.00000000000000000e+00
      fma=4.93038065763132378e-32
     f128=4.930380657631323783823303533017414e-32
 all f128=4.930380657631323783823303533017414e-32
  - double check passed

Checking double-double functions:
double-double (DBL_EPSILON = (2.2204460492503130808e-16,2.2204460492503130808e-16) 2.22044604925031308084726333618164062e-16 so e^2=4.9303806576313237838233035330172e-32 :
Using e=3.3881317890172013563e-21
{1+e}:
   double-double=1.00000000000000000e+00 3.38813178901720136e-21
            f128=1.000000000000000000003388131789017e+00
   error=0.000000000000000000000000000000000e+00
1+e as a double is 1
fast2sum(1,e):
   double-double=1.00000000000000000e+00 3.38813178901720136e-21
            f128=1.000000000000000000003388131789017e+00
   error=0.000000000000000000000000000000000e+00
{1+e}+{1+e}:
   double-double=2.00000000000000000e+00 6.77626357803440271e-21
            f128=2.000000000000000000006776263578034e+00
   error=0.000000000000000000000000000000000e+00
{1+e}+{1+2e}:
   double-double=2.00000000000000000e+00 1.01643953670516041e-20
            f128=2.000000000000000000010164395367052e+00
   error=0.000000000000000000000000000000000e+00
{1+e}+{1-e}:
   double-double=2.00000000000000000e+00 0.00000000000000000e+00
            f128=2.000000000000000000000000000000000e+00
   error=0.000000000000000000000000000000000e+00
 (1+e)*(1+e) - (1+2e) gives:
   double-double=0.00000000000000000e+00 0.00000000000000000e+00
            f128=0.000000000000000000000000000000000e+00
   error=0.000000000000000000000000000000000e+00
 (1+e)*(1-e) gives (should be 1-e^2):
   double-double=1.00000000000000000e+00 0.00000000000000000e+00
            f128=1.000000000000000000000000000000000e+00
   error=0.000000000000000000000000000000000e+00
  - double-double all tests passed


Checking double double divide for accuracy
 355/113 (approximation to pi):
 as dd =3.14159292035398208e+00,2.24009601428792650e-16
 dd's combined to f128's=3.141592920353982300884955752212392e+00
               as f128's=3.141592920353982300884955752212389e+00
              Difference=3.081487911019577364889564708135884e-33
      Test passed
 (355+e)/(113) (e=double epsilon):
 as dd =3.14159292035398208e+00,2.25974597932553992e-16
 dd's combined to f128's=3.141592920353982302849952255973734e+00
               as f128's=3.141592920353982302849952255973728e+00
              Difference=6.162975822039154729779129416271767e-33
      Test passed
 (355+e)/(113+e) (e=double epsilon):
 as dd =3.14159292035398208e+00,2.19801378827817027e-16
 dd's combined to f128's=3.141592920353982296676733151236769e+00
               as f128's=3.141592920353982296676733151236778e+00
              Difference=-9.629649721936179265279889712924637e-33
      Test passed
 (355+e/1e+14)/(113+e/1e+14) (e=double epsilon):
 as dd =3.14159292035398208e+00,2.24009601428792601e-16
 dd's combined to f128's=3.141592920353982300884955752212343e+00
               as f128's=3.141592920353982300884955752212347e+00
              Difference=-4.237045877651918876723151473686840e-33
      Test passed
All tests passed

  
*/
/*----------------------------------------------------------------------------
 * MIT License:
 *
 * Copyright (c) 2025,2026 Peter Miller
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <quadmath.h> /* see https://gcc.gnu.org/onlinedocs/libquadmath/quadmath_005fsnprintf.html#quadmath_005fsnprintf - also needs quadmath library linking in */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdarg.h>
#include "../ya_double_double/ya_double_double.h"


#define _mkstr(s) # s
#define mkstr(s) _mkstr(s)      /* creates "s" */

#if 0
/* the line below defines GCC_OPTIMIZE_AWARE when we can use # pragma GCC optimize ("-O2") */
#define GCC_OPTIMIZE_AWARE (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) || defined(__clang__)
/* code below cannot be compiled with -Ofast as this makes the compiler break some C rules that we need, so make sure of this here */
#if GCC_OPTIMIZE_AWARE
 #pragma GCC push_options
 #pragma GCC optimize ("-O3") /* cannot use Ofast, normally -O3 is OK. Note macro expansion does not work here ! */
 #if defined(_WIN32) && !defined(_WIN64)
  #pragma GCC target("sse2")
 #endif
#endif
#endif

unsigned int errs=0;

//#define DBL_EPSILON (double)0X1P-52

static void check_fma(void) // do some checks on fma(), fmal() amd fmaq()
{
 // start double :
 // DBL_DIG=15,DBL_DECIMAL_DIG__ = 17 , __DBL_EPSILON__ = ((double)2.22044604925031308084726333618164062e-16L)
 // use (1+e) * (1+e) = 1+2e+e^2
 // fma(a,b,c) = a*b-c with just a single rounding on the final result
 // we check fma(1+e,1+e,-(1+2e)) which should equal e^2
 char buf128[128],buf128_e[128];
 #define DBL_DECIMAL_DIG __DBL_DECIMAL_DIG__
 {
  double one_e=1.0+DBL_EPSILON;
  double one_2e=one_e+DBL_EPSILON;
  double r_direct=(one_e*one_e)-one_2e;
  double r_fma_d=fma(one_e,one_e,-one_2e);
  __float128 r_128=(__float128)(one_e)*(__float128)(one_e)-(__float128)(one_2e);// "fma" calculated exactly using f128's
  __float128 r_exact_128=(1.0f128+2.22044604925031308084726333618164062e-16f128)*(1.0f128+2.22044604925031308084726333618164062e-16f128)-(1.0f128+2.0f128*2.22044604925031308084726333618164062e-16f128);// all values f128 - so "exact"
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  quadmath_snprintf (buf128_e, sizeof buf128,"%.33Qe",r_exact_128); // 33 is full precision for full 128bit ieee value
  printf("double (DBL_EPSILON = 2.22044604925031308084726333618164062e-16 so e^2=4.9303806576313237838233035330172e-32 :\n");//  e^2 value calculated by "Windows calculator" using its maximum resolution
  printf("1+epsilon=%.*e 1+2*epsilon=%.*e\n",DBL_DECIMAL_DIG,one_e,DBL_DECIMAL_DIG,one_2e);
  printf("   direct=%.*e\n",DBL_DECIMAL_DIG,r_direct);
  printf("      fma=%.*e\n",DBL_DECIMAL_DIG,r_fma_d);
  printf("     f128=%s\n",buf128);
  printf(" all f128=%s\n",buf128_e); 
  if(r_direct==0 && r_fma_d==(double)r_128 && (double)r_128==(double)r_exact_128 && r_fma_d==4.9303806576313237838233035330172e-32) 
  	printf("  - double check passed\n");
  else
  	{printf("  - double check failed\n");  
  	 errs++;
  	}
  printf("\n");
 }
 
 
 // now look at double-double functions
 // start double :
 // DBL_DIG=15,DBL_DECIMAL_DIG__ = 17 , __DBL_EPSILON__ = ((double)2.22044604925031308084726333618164062e-16L)
 // use (1+e) * (1+e) = 1+2e+e^2
 // fma(a,b,c) = a*b-c with just a single rounding on the final result
 // we check fma(1+e,1+e,-(1+2e)) which should equal e^2
 // WARNING - when checking double-doubles arguments should be "normalised", which 1+DBL_EPSILON is NOT (as 1+DBL_EPSILON can be expressed as a double - by definition).
 // code below uses DBL_EPSILON/65536.0 for e
 
 printf("Checking double-double functions:\n");
 printf("double-double (DBL_EPSILON = (%.20g,%.20g) 2.22044604925031308084726333618164062e-16 so e^2=4.9303806576313237838233035330172e-32 :\n",DBL_EPSILON,0X1P-52);//  e^2 value calculated by "Windows calculator" using its maximum resolution 
 double e=DBL_EPSILON/65536.0;
 printf("Using e=%.20g\n",e);
 {DoubleDouble one_e={1.0,e};// 1+e
  DoubleDouble one_2e={1.0,e+e};// 1+2*e
  DoubleDouble one_me={1.0,-e};// 1-e
  DoubleDouble mone_2e={-1.0,-(e+e)};// -(1+2*e)
  DoubleDouble r;
  __float128 r_128;
  printf("{1+e}:\n");
  r_128=dd_to_f128(one_e);// calculated exactly using f128's
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   double-double=%.*e %.*e\n",DBL_DECIMAL_DIG,one_e.hi,DBL_DECIMAL_DIG,one_e.lo);
  printf("            f128=%s\n",buf128);
  r_128-=dd_to_f128(one_e); // error
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   error=%s\n",buf128); 
  if(r_128!=0) ++errs; 

  double d_1_e=one_e.hi+one_e.lo;
  printf("1+e as a double is %.20g\n",d_1_e);
 
  printf("fast2sum(1,e):\n");
  r=fast2sum(1,e);
  r_128=dd_to_f128(one_e);// calculated exactly using f128's
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   double-double=%.*e %.*e\n",DBL_DECIMAL_DIG,r.hi,DBL_DECIMAL_DIG,r.lo);
  printf("            f128=%s\n",buf128);
  r_128-=dd_to_f128(r); // error
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   error=%s\n",buf128); 
  if(r_128!=0) ++errs;     
  
  printf("{1+e}+{1+e}:\n");
  r=add_dd_dd(one_e,one_e);
  r_128=dd_to_f128(one_e)+dd_to_f128(one_e);// calculated exactly using f128's
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   double-double=%.*e %.*e\n",DBL_DECIMAL_DIG,r.hi,DBL_DECIMAL_DIG,r.lo);
  printf("            f128=%s\n",buf128);
  r_128-=dd_to_f128(r); // error
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   error=%s\n",buf128);
  if(r_128!=0) ++errs; 

  printf("{1+e}+{1+2e}:\n");
  r=add_dd_dd(one_e,one_2e);
  r_128=dd_to_f128(one_e)+dd_to_f128(one_2e);// calculated exactly using f128's
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   double-double=%.*e %.*e\n",DBL_DECIMAL_DIG,r.hi,DBL_DECIMAL_DIG,r.lo);
  printf("            f128=%s\n",buf128);
  r_128-=dd_to_f128(r); // error
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   error=%s\n",buf128);
  if(r_128!=0) ++errs; 
  
  printf("{1+e}+{1-e}:\n");
  r=add_dd_dd(one_e,one_me);
  r_128=dd_to_f128(one_e)+dd_to_f128(one_me);// calculated exactly using f128's
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   double-double=%.*e %.*e\n",DBL_DECIMAL_DIG,r.hi,DBL_DECIMAL_DIG,r.lo);
  printf("            f128=%s\n",buf128);
  r_128-=dd_to_f128(r); // error
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   error=%s\n",buf128); 
  if(r_128!=0) ++errs;    

  r_128=(dd_to_f128(one_e)*dd_to_f128(one_e))-dd_to_f128(one_2e);// calculated exactly using f128's
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  //printf("1+epsilon=%.*e 1+2*epsilon=%.*e\n",DBL_DECIMAL_DIG,one_e,DBL_DECIMAL_DIG,one_2e);
  printf(" (1+e)*(1+e) - (1+2e) gives:\n");
  DoubleDouble r1;
  r1=mult_dd_dd(one_e,one_e); //(1+e)*(1+e)
  // printf("  mult_dd_dd =%.*e %.*e\n",DBL_DECIMAL_DIG,r1h,DBL_DECIMAL_DIG,r1l);
  r1=add_dd_dd(r1,mone_2e);
  printf("   double-double=%.*e %.*e\n",DBL_DECIMAL_DIG,r1.hi,DBL_DECIMAL_DIG,r1.lo);
  printf("            f128=%s\n",buf128);
  r_128-=dd_to_f128(r1); // error
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   error=%s\n",buf128); 
  if(r_128!=0) ++errs;    
  
  // now do (1+e)*(1-e) = 1+e^2
  printf(" (1+e)*(1-e) gives (should be 1-e^2):\n");
  DoubleDouble r2;// 1-e
  r2=mult_dd_dd(one_e,one_me); //(1+e)*(1-e)
  printf("   double-double=%.*e %.*e\n",DBL_DECIMAL_DIG,r2.hi,DBL_DECIMAL_DIG,r2.lo);
  //r_128=((__float128)(one_e_h)+(__float128)(one_e_l))*((__float128)(one_e_h)-(__float128)(one_e_l));// calculated exactly using f128's
  r_128=(1.0f128+2.22044604925031308084726333618164062e-16f128/65536.0f128)*(1.0f128-2.22044604925031308084726333618164062e-16f128/65536.0f128);// calculated exactly using f128's
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("            f128=%s\n",buf128); 
  r_128-=dd_to_f128(r2); // error
  quadmath_snprintf (buf128, sizeof buf128,"%.33Qe",r_128); // 33 is full precision for full 128bit ieee value
  printf("   error=%s\n",buf128); 
  if(r_128!=0) ++errs;         

  if(errs==0) 
  	printf("  - double-double all tests passed\n");
  else
  	{
  	 printf("  - double-double tests failed - %d errors found\n",errs);   	
  	}  	 
  printf("\n");
 } 

}
 
 

 /* __DBL_DENORM_MIN__ ((double)4.94065645841246544176568792868221372e-324L) while__DBL_MIN__ ((double)2.22507385850720138309023271733240406e-308L) so exponents below -308 start to denormalise the mantissa 
   __DBL_MAX__ ((double)1.79769313486231570814527423731704357e+308L) so 308 is max positive exponent before overflow 
   __DBL_EPSILON__ ((double)2.22044604925031308084726333618164062e-16L)
 */
#if 1
void test_div(void)
 {// start with a simple division test use 355/113 which is a good approximation for pi, then add small constants to top & bottom to validate the resolution.
  DoubleDouble d,top={355.0,0.0},bot={113.0,0.0};
  char buf[128]; 
  __float128 a128,e128;// a128 is approximation, e128 is exact result
  printf(" 355/113 (approximation to pi):\n");
  d=div_dd_dd(top,bot);
  printf(" as dd =%.*e,%.*e\n",DBL_DECIMAL_DIG,d.hi,DBL_DECIMAL_DIG,d.lo);
  a128=dd_to_f128(d); 
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,a128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf(" dd's combined to f128's=%s\n",buf);
  e128=355.0f128/113.0f128;
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,e128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf("               as f128's=%s\n",buf);
  e128=a128-e128; // difference
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,e128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf("              Difference=%s\n",buf);
  if(fabsq(e128)<3.0815e-33f128)
  	printf("      Test passed\n");
  else
  	{printf("      Test failed\n");
  	 errs++;
  	}  	
  	
  printf(" (355+e)/(113) (e=double epsilon):\n");
  top.lo=DBL_EPSILON;
  d=div_dd_dd(top,bot);
  printf(" as dd =%.*e,%.*e\n",DBL_DECIMAL_DIG,d.hi,DBL_DECIMAL_DIG,d.lo);
  a128=dd_to_f128(d); 
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,a128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf(" dd's combined to f128's=%s\n",buf);
  e128=(355.0f128 + (__float128)DBL_EPSILON)/(113.0f128);
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,e128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf("               as f128's=%s\n",buf);    
  e128=a128-e128; // difference
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,e128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf("              Difference=%s\n",buf); 
  if(fabsq(e128)<6.163e-33f128)
  	printf("      Test passed\n");
  else
  	{printf("      Test failed\n");
  	 errs++;
  	}  	
  	  
  printf(" (355+e)/(113+e) (e=double epsilon):\n");
  bot.lo=DBL_EPSILON;
  d=div_dd_dd(top,bot);
  printf(" as dd =%.*e,%.*e\n",DBL_DECIMAL_DIG,d.hi,DBL_DECIMAL_DIG,d.lo);
  a128=dd_to_f128(d);  
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,a128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf(" dd's combined to f128's=%s\n",buf);
  e128=(355.0f128 + (__float128)DBL_EPSILON)/(113.0f128+(__float128)DBL_EPSILON);
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,e128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf("               as f128's=%s\n",buf);  
  e128=a128-e128; // difference
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,e128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf("              Difference=%s\n",buf); 
  if(fabsq(e128)<9.62965e-33f128)
  	printf("      Test passed\n");
  else
  	{printf("      Test failed\n");
  	 errs++;
  	}  	
  	  
  
  const double c=1e14; // 1e14 is the largest power of 10 that produces a change in the results for double-doubles 
  printf(" (355+e/%g)/(113+e/%g) (e=double epsilon):\n",c,c);
  top.lo=DBL_EPSILON/c;
  bot.lo=DBL_EPSILON/c;
  d=div_dd_dd(top,bot);
  printf(" as dd =%.*e,%.*e\n",DBL_DECIMAL_DIG,d.hi,DBL_DECIMAL_DIG,d.lo);
  a128=dd_to_f128(d);  
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,a128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf(" dd's combined to f128's=%s\n",buf);
  e128=(355.0f128 + (__float128)DBL_EPSILON/(__float128)c)/(113.0f128+(__float128)DBL_EPSILON/(__float128)c);
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,e128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf("               as f128's=%s\n",buf);    
  e128=a128-e128; // difference
  quadmath_snprintf(buf, sizeof buf,"%.*Qe",__FLT128_DIG__,e128);	 // use __FLT128_DIG__ here (33) as all these should be exact
  printf("              Difference=%s\n",buf);  
  if(fabsq(e128)<4.2371e-33f128)
  	printf("      Test passed\n");
  else
  	{printf("      Test failed\n");
  	 errs++;
  	}  	
  	  
 }
#endif 
 


int main(int argc, char *argv[])
{
 printf("sizeof float=%zd double=%zd long double=%zd\n",sizeof(float),sizeof(double),sizeof(long double));

#if defined(__SIZEOF_FLOAT__)
 printf("__SIZEOF_FLOAT__ is defined and set to %d\n",__SIZEOF_FLOAT__);
#endif
#if defined(__SIZEOF_DOUBLE__)
 printf("__SIZEOF_DOUBLE__ is defined and set to %d\n",__SIZEOF_DOUBLE__);
#endif
#if defined(__SIZEOF_LONG_DOUBLE__)
 printf("__SIZEOF_LONG_DOUBLE__ is defined and is set to %d\n",__SIZEOF_LONG_DOUBLE__);
#endif
#if defined(__SIZEOF_FLOAT128__)
 printf("__SIZEOF_FLOAT128__ is defined and set to %d\n",__SIZEOF_FLOAT128__);
#endif
#if defined(__SIZEOF_INT128__)
 printf("__SIZEOF_INT128__ is defined and set to %d\n",__SIZEOF_INT128__);
#endif
 printf("number of bits in mantissa of float is FLT_MANT_DIG=%d  and max exponent is 2^FLT_MAX_EXP=%d max exponent (decimal)=FLT_MAX_10_EXP=%d\n",FLT_MANT_DIG, FLT_MAX_EXP,FLT_MAX_10_EXP );
 printf("number of bits in mantissa of double is DBL_MANT_DIG=%d  and max exponent is 2^DBL_MAX_EXP=%d max exponent (decimal)=DBL_MAX_10_EXP=%d\n",DBL_MANT_DIG, DBL_MAX_EXP,DBL_MAX_10_EXP );
 printf("number of bits in mantissa of long double is LDBL_MANT_DIG=%d  and max exponent is 2^LDBL_MAX_EXP=%d max exponent (decimal)=LDBL_MAX_10_EXP=%d\n",LDBL_MANT_DIG, LDBL_MAX_EXP,LDBL_MAX_10_EXP );
 printf(" max difference between 1 and next larger representable number for doubles is  DBL_EPSILON=%s\n",mkstr(DBL_EPSILON));
#ifdef __FLT128_MANT_DIG__
 printf("number of bits in mantissa of float128 is __FLT128_MANT_DIG__=%d  and max exponent is 2^__FLT128_MAX_EXP__=%d max exponent (decimal)=__FLT128_MAX_10_EXP__=%d\n",__FLT128_MANT_DIG__, __FLT128_MAX_EXP__,__FLT128_MAX_10_EXP__ );
#endif
#ifdef  __BORLANDC__
  printf("__BORLANDC__ is defined as\"%s\"\n",mkstr(__BORLANDC__));
#endif
#ifdef __clang__
 printf("__clang__ defined and is set to %s\n",mkstr(__clang__));
#endif
#ifdef __GNUC__
 printf("__GNUC__ defined, __GNUC__=%u __GNUC_MINOR__=%u\n",__GNUC__ ,__GNUC_MINOR__);
#endif
#ifdef __MINGW32__ 
 printf("__MINGW32__ defined and is set to %s\n",mkstr(__MINGW32__));
 #ifdef __USE_MINGW_ANSI_STDIO
  printf("__USE_MINGW_ANSI_STDIO is defined as %d\n",(int)(__USE_MINGW_ANSI_STDIO));
 #else
  printf("__USE_MINGW_ANSI_STDIO is NOT defined\n");
 #endif
 #if defined(__USE_MINGW_ANSI_STDIO) && __USE_MINGW_ANSI_STDIO==1
  printf("Using %s() for sprintf() & snprintf()\n","__mingw_...");
 #elif defined(_UCRT)
  printf("Using UCRT for sprintf() &  snprintf()\n");
 #else
  printf("Using %s() for sprintf() &  snprintf()\n","__ms_...");
 #endif
#else
 printf("__MINGW32__ is not defined\n");
#endif 
#ifdef __MINGW64__
 printf("__MINGW64__ defined and is set to %s\n",mkstr(__MINGW64__));
#endif
#ifdef _CRTBLD
 printf("_CRTBLD defined and is set to %s\n",mkstr(_CRTBLD));
#endif
#ifdef _UCRT
 printf("_UCRT defined and is set to %s\n",mkstr(_UCRT));
#endif
#ifdef __MSVCRT__
 printf("__MSVCRT__ defined and is set to %s\n",mkstr(__MSVCRT__));
#endif
#ifdef  _WIN32 /* true even on w64 */
 printf("_WIN32 defined and is set to %s\n",mkstr(_WIN32));
#endif
#ifdef  _WIN64 /* true even on w64 */
 printf("_WIN64 defined and is set to %s\n",mkstr(_WIN64));
#endif

#ifdef  __x86_64 /* true even on w64 */
 printf("__x86_64 defined and is set to %s\n",mkstr(__x86_64));
#endif

#ifdef  __SSE4_2__ 
 printf("__SSE4_2__ defined and is set to %s\n",mkstr(__SSE4_2__));
#endif

#ifdef  __AVX2__ 
 printf("__AVX2__ defined and is set to %s\n",mkstr(__AVX2__));
#endif

#ifdef  __FMA__ 
 printf("__FMA__ defined and is set to %s\n",mkstr(__FMA__));
#endif


#if 1
 printf("Checking fma functions:\n");
 check_fma(); // do some checks on fma()
 printf("\n");
#endif

 printf("Checking double double divide for accuracy\n");
 test_div();

 if(errs==0)
 	printf("All tests passed\n");
 else
    printf("%u tests failed\n",errs); 
#if defined(__BORLANDC__)
 fprintf(stderr,"Finished - press return to exit:");
 getchar();
#endif
 return 0;
}



/* now restore gcc options to those set by the user */
#if GCC_OPTIMIZE_AWARE
#pragma GCC pop_options
#endif