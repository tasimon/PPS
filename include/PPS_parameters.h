#include<time.h>
#include<string.h>

#define PPS_PARAMETERS_H

double get_parfrac(int, double);
double amd(double, int);

float pwr=0.5;
int frequency=1600;

/* Failure Info
 lambda = failures per hour
*/
float lambda=0.1; // Lemieux MTTF
int nodes=4;

/* Define processor type
Options: 
 NEHALEM
 WESTMERE 
*/
#define WESTMERE 


/*Westmere runs from 40-80*/
//2.00 GHz, 1.86 GHz, 1.73 GHz, 1.60 
