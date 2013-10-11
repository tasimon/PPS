/* ------------------------------------------------------------------------
 * Tyler A. Simon
 * This program computes a linear least squares model for a set of given data.
 *
 * PROBLEM DESCRIPTION:
 *  The method of least squares is a standard technique used to find
 *  the equation of a straight line from a set of data. Equation for a
 *  straight line is given by 
 *	 y = mx + b
 *  where m is the slope of the line and b is the y-intercept.
 *
 *  Given a set of n points {(x1,y1), x2,y2),...,xn,yn)}, let
 *      SUMx = x1 + x2 + ... + xn
 *      SUMy = y1 + y2 + ... + yn
 *      SUMxy = x1*y1 + x2*y2 + ... + xn*yn
 *      SUMxx = x1*x1 + x2*x2 + ... + xn*xn
 *
 *  The slope and y-intercept for the least-squares line can be 
 *  calculated using the following equations:
 *        slope (m) = ( SUMx*SUMy - n*SUMxy ) / ( SUMx*SUMx - n*SUMxx ) 
 *  y-intercept (b) = ( SUMy - slope*SUMx ) / n
 *
 * ---------------------------------------------------------------------- */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

double comm_time(double,double, double, double);

int main(int argc, char **argv) {

  double *x, *y, *z;
  double SUMx, SUMy, SUMxy, SUMxx, SUMres, res, slope, SUMyy,
         y_intercept, y_estimate ;
  int i,n;
  FILE *infile;
  char filename[255];

double messagesize;
double bandwidth;
double nodes;

if (argc != 4)
        {
        printf("%s <data file> <message size> <nodes>\n", argv[0]);
        exit(0);
        }

 strcpy(filename,argv[1]);
 messagesize=atof(argv[2]);
 nodes=atof(argv[3]);
  

  infile = fopen(filename, "r");
  if (infile == NULL) printf("error opening file\n");    
  fscanf (infile, "%d", &n);
  x = (double *) malloc (n*sizeof(double));
  y = (double *) malloc (n*sizeof(double));
  z = (double *) malloc (n*sizeof(double));
  
  SUMx = 0; SUMy = 0; SUMxy = 0; SUMxx = 0; 
  for (i=0; i<n; i++) 
	{
   	fscanf (infile, "%lf %lf %lf", &x[i], &y[i], &z[i]);
    	SUMx = SUMx + x[i];
    	SUMy = SUMy + y[i];
    	SUMxy = SUMxy + x[i]*y[i];
    	SUMxx = SUMxx + x[i]*x[i];
    	SUMyy = SUMyy + y[i]*y[i];
  	}
  slope = ( SUMx*SUMy - n*SUMxy ) / ( SUMx*SUMx - n*SUMxx );
  y_intercept = ( SUMy - slope*SUMx ) / n;
  
  printf ("\n");
  printf ("The linear equation that best fits the given data for latency:\n");
  printf ("       y = %6.2lfx + %6.2lf\n", slope, y_intercept);
  printf ("--------------------------------------------------\n");
  printf ("   Original (x,y)     Estimated y     Residual\n");
  printf ("--------------------------------------------------\n");
      
  SUMres = 0;
  for (i=0; i<n; i++) 
	{
    	y_estimate = slope*x[i] + y_intercept;
    	res = y[i] - y_estimate;
    	SUMres = SUMres + res*res;
    	printf ("   (%6.2lf %6.2lf)      %6.2lf       %6.2lf\n", x[i], y[i], y_estimate, res);
  	}
  printf("--------------------------------------------------\n");
  printf("Residual sum = %6.2lf\n", SUMres);
  printf(" r^2 = %6.2lf\n", SUMxy/(sqrt(SUMxx*SUMyy)));
  printf("--------------------------------------------------\n");
  printf("latency(us) = %4.4f\n", (slope*messagesize+y_intercept));

  double time_startup=slope*messagesize+y_intercept;
  double commtime=0.0;

  for(i=0; i<n; i++)
	{
	if( messagesize>=x[i]) 
		{
		if( messagesize>x[n-1]) bandwidth=z[n-1];
		else
		bandwidth=z[i];
	        commtime=comm_time(time_startup,z[i],messagesize,nodes);
		}
	}
		printf("msgsize(bytes) = %2.1f\nBandwidth(MB/s) = %g\n", messagesize,bandwidth);
		printf("nodes = %g\n", nodes);
		printf("Expected Communication Time(s) = %4.4f\n", commtime);

}//end main

double comm_time(double ts, double tw, double m, double nodes)
{
return nodes*((ts*1.0E-6)+((1/tw)*(m/1048576)));
}


