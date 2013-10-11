#include<math.h>
#include<stdio.h>

/*
Failure rate is failures per hour
lambda = 1/SMTBF

Year	Cores	SMTBF(hrs)	System
2002	8192	40	ASCI White
2004	3016	9.7	Lemieux PSC
2007	6656	351	Seaborg NERSC
2002	8192	6.5	ASCI Q
2004	15000	1.2	Google
2006	131072	147.8	BlueGene/L
*/

#define PLOT 0

float exponential_dist(float, float);

/*in:lambda, nodes*/
float failure_rate(float,int);

int failures(int time){

FILE *fp;
char outputfile[255]="failure.dat";

fp=fopen(outputfile,"w");

/* use existing failure data*/
typedef struct
	{
	int cores;
	float mttf;
	char *name;
	} Stats;
	
Stats machine[6];

machine[0].cores=8192;
machine[0].mttf=40;
machine[0].name="ASCI White";

machine[1].cores=3016;
machine[1].mttf=9.7;
machine[1].name="Lemiuex";

machine[2].cores=15000;
machine[2].mttf=1.2;
machine[2].name="Google";

machine[3].cores=6656;
machine[3].mttf=351;
machine[3].name="Seaborg";

machine[4].cores=8192;
machine[4].mttf=6.5;
machine[4].name="ASCI Q";

machine[5].cores=131072;
machine[5].mttf=147.8;
machine[5].name="BlueGene/L";

float mttf[7][7]={{8192,40},{3016,9.7},{6656,351},{8192,6.5},{15000,1.2},{131072,147}};


double r;
float step;
int system;

for(step=0; step<=time; step+=0.1)
	{
	for (system=0; system<=5; system++)
		{	
		//r = mttf[system][1]; //get mtbf
		r = machine[system].mttf;
		fprintf(fp,"%g ", exponential_dist(r,step));
		}
	fprintf(fp,"\n");
	}
fclose(fp);

/*plot results*/
if(PLOT){
FILE *pipe = popen("gnuplot -persist > /dev/null", "w");
fprintf(pipe, "set terminal x11 enhanced\n");
fprintf(pipe, "set xlabel \"Time\"\n");
fprintf(pipe, "set ylabel \"Probability of Failure\"\n");
fprintf(pipe, "set grid \n");
fprintf(pipe, "set yrange[0:1]\n");
fprintf(pipe, "set xrange[0:%d]\n",time);
fprintf(pipe, "plot '%s' u 1 w lp t \"Machine A\",'' u 2 w lp t \"Machine B\",'' u 3 w lp t \"Machine C\",'' u 4 w lp t \"Machine D\",'' u 5 w lp t \"Machine E\"\n",outputfile);
fclose(pipe);
}



}//end main

float exponential_dist(float rate, float time)
        {
	float lambda=1/rate;
        return 1-expf((-lambda)*time);
        }

/*
The total number of failures within an item population, divided by the total time expended by that population, during a particular measurement interval under stated conditions. (MacDiarmid, et al.)
MacDiarmid, Preston; Morris, Seymour; et al., (no date), Reliability Toolkit: Commercial Practices Edition, pp 35–39, Reliability Analysis Center and Rome Laboratory, Rome, New York.

euqation from Wolstenholme pg.112
*/
float failure_rate(float lambda,int nodes)
        {
        float sum=0;
        int i;
        for (i=1; i<=nodes; i++)sum+=(1/i);
        return (1/lambda)*sum;
        }

