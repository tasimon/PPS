/* PPS: Parallel performance summarizer 
** Tyler Simon 
** 10/11/2013
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "PPS_parameters.h"

/*Power is the cube of frequency*/
//#define power(x,y) (y*((x)*(x)*(x)))


float perfperwatt(int,float,float);
float hill(float,int,int);
float get_joules(int ,float,int,int);
float karp_flatt(int, float);

int main(int argc, char **argv)
{
int cores,nodes;
float T_n,memory_t, compute_t;
FILE *fp;
char inputfile[255];
char line[80];

float ppw=0.0;
double speedup_Amdahl=0.0;
double dynamic_speedup=0.0;
double  corewatts=0;
double joules=0;
double watts=0;
double speedup=0;
double par_frac;

if (argc > 1)strcpy(inputfile,argv[1]);
else {
	printf("Usage:%s <inputfile>\n", argv[0]);
	exit(0);
	}


/* Simulation Psudocode (from David Lilja "Measuring Computer Performance")

Initialize global state

initialize global time

obtain input event

while (events) and (time < time limit)
	{
	advance global time
	remove event from event list
	process event
		{
		perform event specific operations
		update global variables
		update sim statistics
		generate new event triggered by this event
		}
	}
	Print simulation results

*/

/* Determine Watts from Frequency*/
#ifdef NEHALEM 
int coresperchip=4;
int chipspernode=2;
int tmpwatts;
if ( frequency==1600 )tmpwatts=40;
else if(frequency==1730)tmpwatts=55;
else if(frequency==1860)tmpwatts=65;
else if(frequency==2000)tmpwatts=80;
char proctype[25]="NEHALEM";
watts=(tmpwatts/(coresperchip*chipspernode));
#endif

#ifdef WESTMERE
int coresperchip=6;
int chipspernode=2;
int tmpwatts;
if ( frequency==1600 )tmpwatts=40;
else if(frequency==1730)tmpwatts=55;
else if(frequency==1860)tmpwatts=65;
else if(frequency==2000)tmpwatts=80;
char proctype[25]="WESTMERE";
watts=(tmpwatts/(coresperchip*chipspernode));
#endif

int first=1;
float compute_t_serial;
float eff;
float t_overhead;
float K;
float gustafson;
float amdahl_max_s;
float k_flatt;


//printf(" %s: Results for %s\n", timestring(), proctype);

printf("\n\n#Runtime\tSpeedup\tPar_frac\tCores\tEff.\tperf_watt\tAmdahl(S)\tA_MAX\tGust(SS)\tK_F\tmult_S\tWatts\tJoules\tT_O\tK\n");
printf("#-------\t-------\t-----\t--------\t-----\t--------\t----------\t---------\t------\t---\t--------\t------\t------\t---\t-\n");

if( (fp=fopen(inputfile, "r")) != NULL)
	{
	while(fgets(line, 80, fp) != NULL)
		{
	 	/* get a line, up to 80 chars from fr.  done if NULL */
		if(first) 
			{
			sscanf(line,"%d %f %d", &cores,&compute_t_serial,&nodes);
			first=0;
		/* Make calculations for the serial (1 processor) case	*/
		speedup=1.00; //for the single processor case
		par_frac=get_parfrac(cores,speedup);
		ppw=perfperwatt(cores,pwr,par_frac);
		speedup_Amdahl=amd(par_frac,cores);
		dynamic_speedup=hill(par_frac,cores,(coresperchip*chipspernode)-cores);
		corewatts=(nodes*cores*powf(frequency,3)*1.0E-6);
		joules=get_joules(watts,compute_t_serial,cores,nodes);
		eff = speedup/cores;
		t_overhead=0;
		K=0;
		gustafson=(1-par_frac)+(par_frac*cores);
		k_flatt=0;
		

		printf("%.2f\t%.3f\t%.3f\t%d\t%.3f\t%.3f\t%.3f\t%.2f\t%.2f\t%.2f\t%.2f\t%g\t%.2f\t%.2f\t%.2f\n",
		compute_t_serial,
		speedup,
		par_frac,
		cores,
		eff,
		ppw,
		speedup_Amdahl,
		0.0,
		gustafson,
		k_flatt,
		dynamic_speedup,
		corewatts,
		joules, 
		t_overhead,
		K);
			}
		else {
			sscanf(line,"%d %f %d", &cores,&compute_t,&nodes);

			speedup=compute_t_serial*(1/compute_t);
	
			par_frac=get_parfrac(cores,speedup);
		
		//Simon & Ward Model
		//T_n=(compute_t/cores)+(cores*memory_t);
		//printf (" T(n) %2.2f = (%2.2f/%d)+(%d*%2.2f)\n", T_n, compute_t,cores,cores,memory_t);
		//printf ("(S)ideal Speedup = %2.2f\n", sqrt(compute_t/memory_t));


		/* Run Woo perf per watt*/
		ppw=perfperwatt(cores,pwr,par_frac);

		/* Get parallel Fraction*/

		/* Run Amdahl*/
		//speedup_Amdahl=amdahl(par_frac, cores);
		speedup_Amdahl=amd(par_frac,cores);

		/* Run Hill's Dynamic Speedup using r additional cores*/
		dynamic_speedup=hill(par_frac,cores,(coresperchip*chipspernode)-cores);

		/* watts from joules*/
		corewatts=(cores*powf(frequency,3)*1.0E-6);
		joules=get_joules(watts,compute_t,cores,nodes);

		eff = speedup/cores;
		K = eff/(1-eff);
		t_overhead = (cores*compute_t)-compute_t_serial;

		gustafson=(1-par_frac)+(par_frac*cores);
		amdahl_max_s=1/(1-par_frac);
		k_flatt=karp_flatt(cores,speedup);
	     
		/* Print Results*/

		printf("%.2f\t%.3f\t%.3f\t%d\t%.3f\t%.2f\t%.3f\t%.2f\t%.2f\t%.2f\t%.2f\t%g\t%.2f\t%.2f\t%.2f\n",
		compute_t,
		speedup,
		par_frac,
		cores,
		eff,
		ppw,
		speedup_Amdahl,
		amdahl_max_s,
		gustafson,
		k_flatt,
		dynamic_speedup,
		corewatts,
		joules,
		t_overhead,K);

			}
		}//reading file
	}
else 
	{
	perror("couldn't open file");
	exit(1);
	}
fclose(fp);

/*print failure data with 24 hour time*/
//failures(24);

return 0;
}//end main loop

float get_joules(int watts,float compute_t,int chipspernode, int nodes)
	{
	// watts = joules / second
	// joules = watts * second
	return (nodes*watts*compute_t*chipspernode);
	}

float perfperwatt(int n, float k,float par_frac)
	{
	float ppw=0.0;
	ppw=1/(1+(n-1)*k*(1-par_frac));
	return ppw;
	}

float hill(float par_frac, int n, int r)
	{ 
	float dynamic_speedup, k;
	/*k = performance on r addition cores*/
	
	//k = sqrt(r);
	//best speedup possible from amdahl 
	k = 1/(1-par_frac);
	//k = amd(par_frac,n);

	dynamic_speedup=1/(((1-par_frac)/k)+(par_frac/n));
	return dynamic_speedup;
	}
/*pg 168 Quinn*/
float karp_flatt(int cores,float speedup)
	{
	return (double)((1/speedup)-(1/(float)cores))/(1-(1/(float)cores));
	}


double getCurrentTime(void)
 /* It returns the current time as a real number: the whole part
   * is the number of seconds, the fractional part is the number of
   * microseconds since the epoch (1 jan 1970 at 00:00:00)
   */
{
struct timeval tval;
gettimeofday(&tval, NULL);
return (tval.tv_sec + tval.tv_usec/1000000.0);
}

