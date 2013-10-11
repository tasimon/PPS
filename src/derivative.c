/* Calculates first and second derivatives of x,y data 
Tyler Simon

data format:

numlines
x y 
...

4
1 2.2
3 4.5
7 2.2
9 11.2

*/


#include<math.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>

int main(int argc, char **argv)
{

double *X;
double *Y;
double *result;
FILE *infile;
char filename[255];
int n; //how many lines
int i;

if (argc==2)
strcpy(filename, argv[1]);
else {
	printf("%s <file with x,y format>\n",argv[0]);
	exit(1);
	}

infile = fopen(filename, "r");
  if (infile == NULL) 
		{
		printf("error opening file\n");
		exit(1);
		}
  fscanf (infile, "%d", &n);
  X = (double *) malloc (n*sizeof(double));
  Y = (double *) malloc (n*sizeof(double));
  result = (double *) malloc (n*sizeof(double));
  

for (i=0; i<n; i++) { fscanf (infile, "%lf\t%lf", &X[i], &Y[i]); }

/* step through and calculate first derivative */
printf("#[index] [xvalue] [y value] [f'(x)]\n");
for (i=0; i<(n-1); i++)
	{
	result[i]=(Y[i+1]-Y[i])/(X[i+1]-X[i]); //dy/dx
	
	printf("%d %g %g %g\n", i,X[i],Y[i],result[i]);
	}


return 0;
}//end main
