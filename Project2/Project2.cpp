/*
 *
 * Name: Rachel Xing
 * Email: xingru@oregonstate.edu
 *
 * Project #2 - Functional Decomposition
 *
 * A month-by-month simulation of a grain-growing operations. The amount the grain grows is affected
 * by the temperature, amount of precipitation, and the number of deer around to eat it. The number
 * of deer depends on the amount of grain available to eat and the number of their predators, wolves.
 *
 */

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#ifndef DEBUG
#define DEBUG		false
#endif

unsigned int seed = 0;
int	    NowYear;		// 2025- 2030
int	    NowMonth;		// 0 - 11
int 	TotalMonth;

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	    NowNumDeer;		// number of deer in the current population
int 	NowNumWolf;		// number of wolves in the current population

const float GRAIN_GROWS_PER_MONTH     =	    50.0;
const float ONE_DEER_EATS_PER_MONTH   =		1.0;

const float AVG_PRECIP_PER_MONTH      =		15.0;	// average
const float AMP_PRECIP_PER_MONTH      =		6.0;	// plus or minus
const float RANDOM_PRECIP             =		2.0;	// plus or minus noise

const float AVG_TEMP                  =		60.0;	// average
const float AMP_TEMP                  =		20.0;	// plus or minus
const float RANDOM_TEMP               =		10.0;	// plus or minus noise

const float MIDTEMP                   =		40.0;
const float MIDPRECIP                 =	    10.0;

// Constants for Lotka-Volterra model that simulates the prey/predator relationship between deer and wolves:
const float ALPHA = 0.3;		// growth rate of the prey (deer)
const float BETA  = 0.015;		// death rate of the prey (deer)
const float DELTA =	0.01;		// growth rate of the predator (wolf)
const float GAMMA = 0.9;		// death rate of the predator (wolf)
const float DT	  = 1;			// time step - 1 month

// Variables for the custom barriers:
omp_lock_t		Lock;
volatile int	NumInThreadTeam;
volatile int	NumAtBarrier;
volatile int	NumGone;

// Functions for the custom barriers:
void InitBarrier( int n );
void WaitBarrier( );

// Functions for the tasks for grain-growing operations:
void Deer( );
void Grain( );
void Wolf( );
void Watcher( );

// Helper functions for mathematical operations:
float Ranf( float low, float high);
int   Ranf( int ilow, int ihigh);
float SQR( float x);

// Main program:
int main( int argc, char *argv[ ] ) {

	// starting date and time:
	NowMonth   = 0;
    TotalMonth = 0;
	NowYear    = 2025;

	// starting state (feel free to change this if you want):
	NowNumDeer = 20;
    NowNumWolf = 5;
	NowHeight  = 100.;

    // starting temperature and precipitation
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
		NowPrecip = 0.;

	omp_set_num_threads( 4 );	// same as # of sections
	InitBarrier( 4 );
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			Deer( );
		}

		#pragma omp section
		{
			Grain( );
		}

		#pragma omp section
		{
			Wolf( );
		}

		#pragma omp section
		{
			Watcher( );
		}
	}       // implied barrier -- all functions must return in order
			// to allow any of them to get past here
	return 0;
}

/*
 * Handle the task of updating the number of deers based on the current state
 */
void Deer( ) {
	int nextNumDeer, carryingCapacity;
	while ( NowYear < 2031 ) {

		nextNumDeer = NowNumDeer;
		carryingCapacity = (int)( NowHeight );

		// Lotka-Volterra equation for the prey population
		int deltaDeer =  ( ALPHA * (float)NowNumDeer - BETA * (float)NowNumDeer * (float)NowNumWolf) * (float)DT;
		nextNumDeer += deltaDeer;

		if( nextNumDeer < carryingCapacity )
			nextNumDeer++;
		else if( nextNumDeer > carryingCapacity )
			nextNumDeer--;

		if( nextNumDeer < 0 )
			nextNumDeer = 0;		// clamp nextNumDeer against zero

		WaitBarrier(); 				// DoneComputing barrier

		NowNumDeer = nextNumDeer;

		WaitBarrier();				// DoneAssigning barrier

		WaitBarrier();				// DonePrinting barrier


	}
}

/*
 * Handle the task of updating the number of wolves based on the current state
 */
void Wolf( ) {
	int nextNumWolf, carryingCapacity;
	while ( NowYear < 2031 ) {

		nextNumWolf = NowNumWolf;
		carryingCapacity = NowNumDeer;

        // Lotka-Volterra equation for the predator population
        int deltaWolf =  (DELTA * (float)NowNumDeer * (float)NowNumWolf - GAMMA * (float)NowNumWolf) * (float)DT;
		nextNumWolf += deltaWolf;

		if( nextNumWolf > carryingCapacity )
			nextNumWolf--;
        else if ( nextNumWolf < carryingCapacity )
          	nextNumWolf++;

		if( nextNumWolf < 0 )
			nextNumWolf = 0;		// clamp nextNumWolf against zero

		WaitBarrier(); 				// DoneComputing barrier

		NowNumWolf = nextNumWolf;

		WaitBarrier();				// DoneAssigning barrier

		WaitBarrier();				// DonePrinting barrier


	}
}

/*
 * Handle the task of updating the height of the grain based on the current state
 */
void Grain( ) {
	float tempFactor, precipFactor, nextHeight;
	while ( NowYear < 2031 ) {

		float tempFactor = exp( -SQR(  ( NowTemp - MIDTEMP ) / 10. ) );
		float precipFactor = exp( -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  ) );

		nextHeight = NowHeight;
		nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        if ( nextHeight < 0. )
        	nextHeight = 0.;		// clamp nextHeight against zero

		WaitBarrier();				// DoneComputing barrier

        NowHeight = nextHeight;

		WaitBarrier();				// DoneAssigning barrier

		WaitBarrier();				// DonePrinting barrier
	}
}

/*
 * Handle the task of printing the updated current state and updating
 * environment parameters accordingly
 */

void Watcher( ) {
	while ( NowYear < 2031 ) {

		WaitBarrier();				// DoneComputing barrier

		WaitBarrier();				// DoneAssigning barrier

        // Once the update of all the current state have been completed
        // print the updated current state:
// #define CSV
#ifdef  CSV
		fprintf(stderr, "%4d, %6.2lf, %6.2lf, %6.2lf, %4d, %4d\n",
            TotalMonth, NowPrecip, NowTemp, NowHeight, NowNumDeer, NowNumWolf);
#else
        if ( DEBUG )
			fprintf(stderr, "Year: %4d, Month: %2d, Total Month: %2d, Precip (inches): %6.2lf, Temp (°F): %6.2lf, Height: %6.2lf, Deer #: %4d, Wolf #: %4d\n",
				NowYear, NowMonth+1, TotalMonth, NowPrecip, NowTemp, NowHeight, NowNumDeer, NowNumWolf);
        else
        	fprintf(stderr, "%4d, %2d, %2d, %6.2lf, %6.2lf, %6.2lf, %4d, %4d\n",
			  	NowYear, NowMonth+1, TotalMonth, NowPrecip, NowTemp, NowHeight, NowNumDeer, NowNumWolf);
#endif
       	// increment time accordingly:
        NowMonth++;
        if ( NowMonth > 11 ) {
        	NowYear++;
            NowMonth = 0;
        }
        TotalMonth++;

        // calculate new environment variables and update them:
		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

		float temp = AVG_TEMP - AMP_TEMP * cos( ang );
		NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
		NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
		if( NowPrecip < 0. )
			NowPrecip = 0.;

		WaitBarrier();
	}

}

/*
 * For Custom Barriers:
 * Specify how many threads will be in the barrier and initialize the lock
 */
void InitBarrier( int n )
{
	NumInThreadTeam = n;
	NumAtBarrier = 0;
	omp_init_lock( &Lock );
}

/*
 * For Custom Barriers:
 * Have the calling thread wait here until all the other threads catch up
 */
void WaitBarrier( ) {
	omp_set_lock( &Lock );
	{
		NumAtBarrier++;
		if( NumAtBarrier == NumInThreadTeam )
		{
			NumGone = 0;
			NumAtBarrier = 0;
			// let all other threads get back to what they were doing
			// before this one unlocks, knowing that they might immediately
			// call WaitBarrier( ) again:
			while( NumGone != NumInThreadTeam-1 );
			omp_unset_lock( &Lock );
			return;
		}
	}
	omp_unset_lock( &Lock );

	while( NumAtBarrier != 0 );	// this waits for the nth thread to arrive
	#pragma omp atomic
		NumGone++;			// this flags how many threads have returned
}

/*
 * Helper function to choose a random number between two given floats
 */
float Ranf( float low, float high )
{
	float r = (float) rand();               // 0 - RAND_MAX
	float t = r  /  (float) RAND_MAX;       // 0. - 1.

	return   low  +  t * ( high - low );
}

/*
 * Helper function to choose a random number between two given integers
 */
int Ranf( int ilow, int ihigh )
{
	float low = (float)ilow;
	float high = ceil( (float)ihigh );

	return (int) Ranf(low,high);
}

/*
 * Helper function to get the square of a given number x
 */
float SQR( float x )
{
	return x*x;
}
