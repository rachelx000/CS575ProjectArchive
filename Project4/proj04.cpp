/*
 *
 * Name: Rachel Xing
 * Email: xingru@oregonstate.edu
 *
 * Project #4 - Vectorized Array Multiplication and Multiplication/Reduction using SSE
 *
 * Test array multiplication and multiplication/reduction using usual C/C++ programming,
 * SIMD, usual C/C++ programming + multithreading, SIMD + multithreading
 *
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>
#include <omp.h>

// SSE stands for Streaming SIMD Extensions

#define SSE_WIDTH	4
#define ALIGNED		__attribute__((aligned(16)))


#define NUMTRIES	100

#ifndef ARRAYSIZE
#define ARRAYSIZE	1024*1024
#endif

// print debugging messages?
#ifndef DEBUG
#define DEBUG		true
#endif

// print CSV?
#define CSV			true

// Extra credit?
#define EC			false


ALIGNED float A[ARRAYSIZE];
ALIGNED float B[ARRAYSIZE];
ALIGNED float C[ARRAYSIZE];


void	SimdMul(    float *, float *,  float *, int );
void	NonSimdMul( float *, float *,  float *, int );
float	SimdMulSum(    float *, float *, int );
float	NonSimdMulSum( float *, float *, int );


int
main( int argc, char *argv[ ] )
{
	for( int i = 0; i < ARRAYSIZE; i++ )
	{
		A[i] = sqrtf( (float)(i+1) );
		B[i] = sqrtf( (float)(i+1) );
	}

	if ( CSV )
		fprintf( stderr, "%12d,", ARRAYSIZE );
	else
		fprintf( stderr, "%12d\t", ARRAYSIZE );

	double maxPerformance = 0.;
	for( int t = 0; t < NUMTRIES; t++ )
	{
		double time0 = omp_get_wtime( );
		NonSimdMul( A, B, C, ARRAYSIZE );
		double time1 = omp_get_wtime( );
		double perf = (double)ARRAYSIZE / (time1 - time0);
		if( perf > maxPerformance )
			maxPerformance = perf;
	}
	double megaMults = maxPerformance / 1000000.;
	if ( CSV )
		fprintf( stderr, "%10.2lf,", megaMults );
	else
		fprintf( stderr, "N %10.2lf\t", megaMults );
	double mmn = megaMults;
	if ( DEBUG )
		fprintf( stderr, "\nNon-SIMD SimdMul:\t[ %8.1f , %8.1f ]\n", C[0], C[ARRAYSIZE-1]);

	maxPerformance = 0.;
	for( int t = 0; t < NUMTRIES; t++ )
	{
		double time0 = omp_get_wtime( );
		SimdMul( A, B, C, ARRAYSIZE );
		double time1 = omp_get_wtime( );
		double perf = (double)ARRAYSIZE / (time1 - time0);
		if( perf > maxPerformance )
			maxPerformance = perf;
	}
	megaMults = maxPerformance / 1000000.;
	if ( CSV )
		fprintf( stderr, "%10.2lf,", megaMults );
	else
		fprintf( stderr, "S %10.2lf\t", megaMults );
	double mms = megaMults;
	double speedup = mms/mmn;
	if ( CSV )
		fprintf( stderr, "%6.2lf,", speedup );
	else
		fprintf( stderr, "(%6.2lf)\t", speedup );
	if ( DEBUG )
		fprintf( stderr, "\nSIMD SimdMul:\t\t[ %8.1f , %8.1f ]\n", C[0], C[ARRAYSIZE-1]);

	maxPerformance = 0.;
	float sumn, sums;
	for( int t = 0; t < NUMTRIES; t++ )
	{
		double time0 = omp_get_wtime( );
		sumn = NonSimdMulSum( A, B, ARRAYSIZE );
		double time1 = omp_get_wtime( );
		double perf = (double)ARRAYSIZE / (time1 - time0);
		if( perf > maxPerformance )
			maxPerformance = perf;
	}
	double megaMultAdds = maxPerformance / 1000000.;
	if ( CSV )
		fprintf( stderr, "%10.2lf,", megaMultAdds );
	else
		fprintf( stderr, "N %10.2lf\t", megaMultAdds );
	double mmrn = megaMultAdds;

	maxPerformance = 0.;
	for( int t = 0; t < NUMTRIES; t++ )
	{
		double time0 = omp_get_wtime( );
		sums = SimdMulSum( A, B, ARRAYSIZE );
		double time1 = omp_get_wtime( );
		double perf = (double)ARRAYSIZE / (time1 - time0);
		if( perf > maxPerformance )
			maxPerformance = perf;
	}
	megaMultAdds = maxPerformance / 1000000.;
	if ( CSV )
		fprintf( stderr, "%10.2lf,", megaMultAdds );
	else
		fprintf( stderr, "S %10.2lf\t", megaMultAdds );
	double mmrs = megaMultAdds;
	speedup = mmrs/mmrn;
	if ( CSV )
		fprintf( stderr, "%6.2lf,", speedup );
	else
		fprintf( stderr, "(%6.2lf)\n", speedup );
    if ( DEBUG )
		fprintf( stderr, "MulSum:\t\t\t[ %8.1f , %8.1f ]\n", sumn, sums );

    /* 		Extra Credit	*/
    int threads[] = {1, 2, 4};

    for( int i = 0; i < 3; i++) {
      	// set number of threads
    	int t = threads[i];
    	omp_set_num_threads( t );
        double num_elements_per_core = (double)ARRAYSIZE / double(t);

        double maxPerformance = 0.;
		for( int t = 0; t < NUMTRIES; t++ )
		{
			double time0 = omp_get_wtime( );
			#pragma omp parallel
			{
				int thisThread = omp_get_thread_num( );
				int first = thisThread * num_elements_per_core;
				NonSimdMul( &A[first], &B[first], &C[first], num_elements_per_core );
			}
			double time1 = omp_get_wtime( );
			double perf = (double)ARRAYSIZE / (time1 - time0);
			if( perf > maxPerformance )
				maxPerformance = perf;
		}
		double megaMults = maxPerformance / 1000000.;
		if ( CSV )
			fprintf( stderr, "%10.2lf,", megaMults );
		else
			fprintf( stderr, "N+%d %10.2lf\t", t, megaMults );
    	mms = megaMults;
    	speedup = mms/mmn;
    	if ( CSV )
    		fprintf( stderr, "%6.2lf,", speedup );
    	else
    		fprintf( stderr, "(%6.2lf)\t", speedup );
    	if ( DEBUG )
    		fprintf( stderr, "\nNon-SIMD+%d Cores SimdMul:\t[ %8.1f , %8.1f ]\n", t, C[0], C[ARRAYSIZE-1]);

		maxPerformance = 0.;
		for( int t = 0; t < NUMTRIES; t++ )
		{
			double time0 = omp_get_wtime( );
			#pragma omp parallel
			{
				int thisThread = omp_get_thread_num( );
				int first = thisThread * num_elements_per_core;
            	SimdMul( &A[first], &B[first], &C[first], num_elements_per_core );
            }
			double time1 = omp_get_wtime( );
			double perf = (double)ARRAYSIZE / (time1 - time0);
			if( perf > maxPerformance )
				maxPerformance = perf;
		}
		megaMults = maxPerformance / 1000000.;
		if ( CSV )
			fprintf( stderr, "%10.2lf,", megaMults );
		else
			fprintf( stderr, "S+%d %10.2lf\t", t, megaMults );
		mms = megaMults;
		speedup = mms/mmn;
		if ( CSV )
			fprintf( stderr, "%6.2lf,", speedup );
		else
			fprintf( stderr, "(%6.2lf)\t", speedup );
		if ( DEBUG )
			fprintf( stderr, "\nSIMD+%d Cores SimdMul:\t\t[ %8.1f , %8.1f ]\n", t, C[0], C[ARRAYSIZE-1]);

		maxPerformance = 0.;
		float sumn, sums;
		for( int t = 0; t < NUMTRIES; t++ )
		{
			double time0 = omp_get_wtime( );
			#pragma omp parallel reduction(+:sumn)
			{
				int thisThread = omp_get_thread_num( );
				int first = thisThread * num_elements_per_core;
                double sum = NonSimdMul( &A[first], &B[first], num_elements_per_core );
				sumn += sum
			}
			double time1 = omp_get_wtime( );
			double perf = (double)ARRAYSIZE / (time1 - time0);
			if( perf > maxPerformance )
				maxPerformance = perf;
		}
		double megaMultAdds = maxPerformance / 1000000.;
		if ( CSV )
			fprintf( stderr, "%10.2lf,", megaMultAdds );
		else
			fprintf( stderr, "N+%d %10.2lf\t", t, megaMultAdds );
    	mmrs = megaMultAdds;
    	speedup = mmrs/mmrn;
    	if ( CSV )
    		fprintf( stderr, "%6.2lf,", speedup );
    	else
    		fprintf( stderr, "(%6.2lf)\n", speedup );

		maxPerformance = 0.;
		for( int t = 0; t < NUMTRIES; t++ )
		{
			double time0 = omp_get_wtime( );
			sums = SimdMulSum( A, B, ARRAYSIZE );
			#pragma omp parallel reduction(+:sums)
			{
				int thisThread = omp_get_thread_num( );
				int first = thisThread * num_elements_per_core;
				double sum = NonSimdMul( &A[first], &B[first], num_elements_per_core );
				sums += sum
			}
			double time1 = omp_get_wtime( );
			double perf = (double)ARRAYSIZE / (time1 - time0);
			if( perf > maxPerformance )
				maxPerformance = perf;
		}
		megaMultAdds = maxPerformance / 1000000.;
		if ( CSV )
			fprintf( stderr, "%10.2lf,", megaMultAdds );
		else
			fprintf( stderr, "S+%d %10.2lf\t", t, megaMultAdds );
    	mmrs = megaMultAdds;
    	speedup = mmrs/mmrn;
    	if ( CSV )
    		fprintf( stderr, "%6.2lf,", speedup );
    	else
    		fprintf( stderr, "(%6.2lf)\n", speedup );
    	if ( DEBUG )
    		fprintf( stderr, "MulSum+%d Cores:\t\t\t[ %8.1f , %8.1f ]\n", t, sumn, sums );
    }

	return 0;
}


void
NonSimdMul( float *A, float *B, float *C, int n )
{
	for( int i = 0; i < n; i++ ) {
  		C[i] = A[i] * B[i];
    }
}

float
NonSimdMulSum( float *A, float *B, int n )
{
  	float sum = 0.;
	for( int i = 0; i < n; i++ ) {
		sum += A[i] * B[i];
	}
    return sum;
}


void
SimdMul( float *a, float *b,   float *c,   int len )
{
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	__asm
	(
		".att_syntax\n\t"
		"movq    -24(%rbp), %r8\n\t"		// a
		"movq    -32(%rbp), %rcx\n\t"		// b
		"movq    -40(%rbp), %rdx\n\t"		// c
	);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups	(%r8), %xmm0\n\t"	// load the first sse register
			"movups	(%rcx), %xmm1\n\t"	// load the second sse register
			"mulps	%xmm1, %xmm0\n\t"	// do the multiply
			"movups	%xmm0, (%rdx)\n\t"	// store the result
			"addq $16, %r8\n\t"
			"addq $16, %rcx\n\t"
			"addq $16, %rdx\n\t"
		);
	}

	for( int i = limit; i < len; i++ )
	{
		c[i] = a[i] * b[i];
	}
}



float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;

	__asm
	(
		".att_syntax\n\t"
		"movq    -40(%rbp), %r8\n\t"		// a
		"movq    -48(%rbp), %rcx\n\t"		// b
		"leaq    -32(%rbp), %rdx\n\t"		// &sum[0]
		"movups	 (%rdx), %xmm2\n\t"		// 4 copies of 0. in xmm2
	);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups	(%r8), %xmm0\n\t"	// load the first sse register
			"movups	(%rcx), %xmm1\n\t"	// load the second sse register
			"mulps	%xmm1, %xmm0\n\t"	// do the multiply
			"addps	%xmm0, %xmm2\n\t"	// do the add
			"addq $16, %r8\n\t"
			"addq $16, %rcx\n\t"
		);
	}

	__asm
	(
		".att_syntax\n\t"
		"movups	 %xmm2, (%rdx)\n\t"	// copy the sums back to sum[ ]
	);

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}