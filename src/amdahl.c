#include<stdlib.h>
#include<stdio.h>

double amd(double par, int n)
        {
        double speedup;
        speedup=1/((1-par)+(par/n));
        return speedup;
        }

double get_parfrac(int n,double speedup_in)
{
double i;
float epsilon=0.001;
double speedup=0.0;
double pfrac=0.0;

for (i=1.0; i>=0.00; i-=epsilon)
	{
	speedup=amd(i,n);
	if (speedup_in>= n){pfrac=1.0;break;}

	if(speedup_in <= (speedup+epsilon) && speedup >= (speedup-epsilon))
		{
		pfrac=i;
		}
	}
return (pfrac);
}
