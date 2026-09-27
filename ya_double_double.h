/* ya-double-double.h
   Header only double-double library
    Note these routines ignore special cases (inf, nan arguments, overflows, divides by zero, etc) 
	Code that does deal with these situations is in "double_double" (but those functions have a different calling convention).
   This version by Peter Miller 18-9-2026
 
 Note that the order of hi & lo in a DoubleDouble is normally hi,lo, but can be changed to lo,hi by defining YA_DD_LO_FIRST before including this header file 
 
Recommended  Gcc setup  is  (minimum Windows 11 PC + avx2 for fma):
	 -msse4.2
	-mfpmath=sse
	-mavx2
	-mfma
	-O3
	-fno-math-errno
	-DNDEBUG
	
*/
/*
This is licensed under the following standard MIT license:

----------------------------------------------------------------------
Copyright © 2026 Peter Miller.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------
*/ 
#ifndef _ya_double_double_h
#define  _ya_double_double_h

 #ifdef __cplusplus
 extern "C" {
 #endif 
/* code below cannot be compiled with -Ofast as this makes the compiler break some C rules that we need, so make sure of this here */
/* we also need -msse2 and -mfpmath=sse to actually use the sse instructions for float and double maths, we also need -mfma or -mavx2 to get fma as a single instruction  */
/* there seems to be no way to duplicate "-fexcess-precision=standard" using a pragma - so that must be present on the command line [see comments at head of this file that suggest "-fexcess-precision=standard" is not required any more ] */

#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) || defined(__clang__)
 #if !defined(__BORLANDC__)
  #pragma GCC push_options
  #pragma GCC optimize ("-O3") /* cannot use Ofast, normally -O3 is OK. Note macro expansion does not work here ! */
 #ifdef  __x86_64  
   #pragma GCC target("avx2,fma") /*  -mavx2 [for fma] */
  #endif
 #endif
 // based on  https://jdebp.uk/FGA/predefined-macros-processor.html "__i386__" is set by GCC,Clang,Intel which is good enough as the outer #if limits us to gcc and clang
 #ifdef __i386__
   #pragma GCC target("sse2,fpmath=sse,avx2,fma") /* -msse2 and -mfpmath=sse, -mavx2 [the later for fma] */
 #endif 
#endif

// double-double routines here assume we have FMA ( __builtin_fma) - note this will be implemented in software if not available as an instruction in the processor (available for AVX2)

struct _DoubleDouble
{
#ifdef YA_DD_LO_FIRST	
	/* Note lo is first to match llvm - so smallest magnitude value is 1st in initialisers  */
	double lo;  
	double hi;
#else
	/*  hi is first - so largest magnitude value is 1st in initialisers  - this matches my original double-double library */
	double hi;  
	double lo;
#endif	
};
typedef struct _DoubleDouble DoubleDouble;/* 2 doubles */

#ifdef __SIZEOF_FLOAT128__
 static inline __float128 dd_to_f128(DoubleDouble x) // convert a double-double to  a float128
 {return (__float128)x.hi+(__float128)x.lo;
 }
 #endif

// returns -x
static inline DoubleDouble minus_dd(DoubleDouble x)
{DoubleDouble r;
 r.hi= -x.hi;
 r.lo= -x.lo;
 return r;
}

 // adds a and b to give double double "x". Requires |a| >= |b|
static inline DoubleDouble fast2sum(double a,double b)
{DoubleDouble r;
 r.hi=a+b;
 double t=r.hi-a;
 r.lo=b-t;
 return r;
}

// general add a+b => double double
static inline DoubleDouble exact_add(double a,double b)
{DoubleDouble r;
 double s=a+b;
 r.hi=s;
 double z=s-a;
 r.lo=(a-(s-z))+(b-z);
 return r;
}


// add two double-double variables
static inline DoubleDouble add_dd_dd(DoubleDouble x,DoubleDouble y)
{DoubleDouble s,t,v;
 double c,w;
 s=exact_add(x.hi,y.hi);
 t=exact_add(x.lo,y.lo);
 c=s.lo+t.hi;
 v=fast2sum(s.hi,c);
 w=t.lo+v.lo;
 return fast2sum(v.hi,w);
}

// add double-double and double variables
static inline DoubleDouble add_dd_d(DoubleDouble a,double b)
{DoubleDouble r=exact_add(a.hi,b);
 return fast2sum(r.hi,r.lo+a.lo);
}

// multiply two double variables to give a double-double result
static inline DoubleDouble exact_mult(double a,double b)
{DoubleDouble r;
 r.hi=a*b;
 r.lo= __builtin_fma(a,b,-r.hi);
 return r;
}

// multiply double * double-double to give a double-double result
static inline DoubleDouble mult_d_dd(double a,DoubleDouble b)
{DoubleDouble r=exact_mult(a,b.hi);
 r.lo= __builtin_fma(a,b.lo,r.lo);
 return r;
}

// multiply double-double * double-double to give a double-double result
// This ignores the lo*lo term of the result, which is OK where we consider only "normalised" DoubleDoubles
static inline DoubleDouble mult_dd_dd(DoubleDouble a,DoubleDouble b)
{DoubleDouble r=exact_mult(a.hi,b.hi);
 double t1= __builtin_fma(a.hi,b.lo,r.lo);
 double t2= __builtin_fma(a.lo,b.hi,t1);
 r.lo=t2;
 return r;
}

// multiply double-double * double-double to give a double-double result
// does include the lo*lo term.
static inline DoubleDouble exact_mult_dd_dd(DoubleDouble a,DoubleDouble b)
{DoubleDouble r=exact_mult(a.hi,b.hi);
 double t1= __builtin_fma(a.hi,b.lo,r.lo);
 double t2= __builtin_fma(a.lo,b.hi,t1);
 t2= __builtin_fma(a.lo,b.lo,t2); // two low's may change the result, for example (1+e)*(1-e) should give 1-e^2 but without this line gives 1
 r.lo=t2;
 return r;
}

/* This routine implements Algorithm 4 from
   "Extended-Precision FMA under Parameterized Double-Word Overlap:
   Tight Error Bounds and Examples by Claude-Pierre Jeannerod, Mioara Joldes,
   Nicolas Louvet, Jean-Michel Muller, published in the proceedings of
   Arith 2026, https://inria.hal.science/hal-05517451.

   This implementation is based on FastFMA_DW() from the CORE-MATH project
   (https://core-math.gitlabpages.inria.fr/).
*/
static inline DoubleDouble FMA_dd_dd_dd(DoubleDouble a,DoubleDouble b,DoubleDouble c)		
{DoubleDouble r;
  double dh = __builtin_fma (a.hi, b.hi, c.hi);
  double t = c.hi - dh;
  double e = __builtin_fma (a.hi, b.hi, t);
  double f = e + c.lo;
  // double g = __builtin_fma (ah, bl, f); // original algorithm
  double g = a.hi * b.lo + f;
  // *l = __builtin_fma (al, bh, g); // original algorithm
  double dl= a.lo * b.hi + g;
  r.hi=dh;
  r.lo=dl;
  return r;
}

// divide double-double / double-double to give a double-double result
// This algorithm is from COREMATH tan.c function fast_div()
// See  "High Precision Division and Square Root", Alan H. Karp and Peter Markstein, 
// ACM Transactions on Mathematical Software Volume 23, Issue 4, Dec 1997 (Originally published as HP Labs Report 93-93-42 (R.1) Oct 1994).
static inline DoubleDouble div_dd_dd(DoubleDouble a,DoubleDouble b)
{
 DoubleDouble r;
 double q=1.0/b.hi;
 r.hi=a.hi*q;
 double e_hi= __builtin_fma(b.hi,-r.hi,a.hi);
 double e_lo= __builtin_fma(b.lo,-r.hi,a.lo);
 r.lo=q*(e_hi+e_lo);
 return r;
}
 #ifdef __cplusplus
    }
 #endif
 

#endif // ifndef _ya_double_double_h