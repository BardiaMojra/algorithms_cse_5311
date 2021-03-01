/**
 * Lab01 P01 Coupon Collection
 *  How many boxes of cereal needs to be opened under a general discrete
 *  distribution P for m coupons. General in this case means we will have
 *  different probabilities for different coupons.
 * nested summation - power set
 *  -- here we need to work with subsets of q=0 to m-1
 *  -- (-1)^(m-1-q) dont use a power function, just sign
 * ----> based on current value of q, produce the set of all subsets for the
 * set of probabilities. Then we need it in terms of an index set.
 * ----> Then for all sets of size q, add up all the probabilities, subtract from
 * 1 and all those together. See the example.
 */

/*!
Part 01
  Implement a C program and evaluate (14b) for the SPECIAL case.

  Must be C.

  Compile and execute UTA Omega.

  Submit C code only.

  m > 2 and m <= 50

  input (multiple lines, it will be a text file):
  1st line: 6 3 ---  m (number of coupon), k (number of the first type)
  2nd line: then we have p, the probability of the first type of coupon.
  3rd  number random sim
  4th seed, for random number generator (e.g. srandom()) to start the
  simulation


  The simulation and estimator must produce results within few percent of each
  other.


  implementation should take \theta(m^2) time, powerset, instead of the \theta
  (2^m) time, exponential, needed by the original version of the expression
  (eq 14b). See elementary combinator.

  Binomial coefficient

  Count how many times the innermost loop is executed.

  Input:
  Enter total number of coupons and number of coupons of first type [m k]:
  50 10
  Enter probability for each coupon of first type [p]:
  0.099

  There are 40 coupons of second type with probability 0.000250

  Enter # of iterations for empirical simulation [t]:
  1000
  Enter seed [s]:
  5555

  Output:
  Theoretical 17114.172170 Count 450 ------> innermost loop
  Empirical boxes 17205.949000



  Links:
  orig paper: http://algo.inria.fr/flajolet/Publications/FlGaTh92.pdf
  https://www.youtube.com/watch?v=3mu47FWEuqA
  https://web.cs.wpi.edu/~hofri/CCP.pdf
  http://ranger.uta.edu/~weems/NOTES5311/cse5311.html

  Useful: https://en.wikipedia.org/wiki/Pascal%27s_triangle
*/



// Solves subset sum using dynamic programming

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// S[1] . . . S[n] are the input values
// A[k] will store the smallest subscript value j s.t. there is some
//      subset of S[1] . . . S[j-1], along with S[j], that sums to k
//int *S,*A,C,n;

/**

S=(int*) malloc((n+1)*sizeof(int));
A=(int*) malloc((C+1)*sizeof(int));
if (!S || !A)
{
  printf("malloc failed %d\n",__LINE__);
  exit(0);
}

printf("Enter set of %d positive numbers\n",n);
S[0]=0;
for (i=1;i<=n;i++)
  scanf("%d",&S[i]);

for (i=0;i<=C;i++)
  A[i]=(-1);  // initialize to "impossible"

A[0]=0;  // This is a cute way to force the initialization in Notes 18.
for (i=0;i<C;i++)
  // Derive other cases from optimal way to get A[i]
  if (A[i]!=(-1))
    // The S[] values increase the sum in increasing subscript order.
    for (j=A[i]+1;j<=n;j++)
    {
      k=i+S[j];
      if (k>C)
        continue;
      // See if A[k] can be improved
      if (A[k]==(-1) || A[k]>j)
      {
        printf("Changed A[%d] to %d\n",k,j);
        A[k]=j;
      }
    }

if (C<=50)
{
  printf(" i     S[i]\n");
  printf("--- ----------\n");
  for (i=1;i<=n;i++)
    printf("%3d     %4d\n",i,S[i]);
  printf(" i     A[i]\n");
  printf("--- ----------\n");
  for (i=1;i<=C;i++)
  {
    printf("%3d ",i);
    if (A[i]==(-1))
      printf("IMPOSSIBLE\n");
    else
      printf("%5d (%d)\n",A[i],S[A[i]]);
  }
}

if (A[C]==(-1))
  printf("no solution\n");
else
{
  // traceback to get subset
  printf("Solution is:\n");
  sum=C;
  while (sum>0)
  {
    printf("S[%d]=%d\n",A[sum],S[A[sum]]);
    sum-=S[A[sum]];  // Remove last S[] value included in sum.
  }
}
free(S);
free(A);
*/


typedef uint64_t marker;

void comb(int, int, marker, int, int);


marker one = 1;
double term;
int *P,*M;

int main()
{
 /**
  * m - number of coupons
  * k - number of coupons of the first type
  * h - number of coupons of the second type
  * t - number of iterations for empirical simulation
  * p - probability of the first type of coupon (float)
  * q - probability of the second type of coupon (float)
  * s - seed for random natural number generator
  */
  int m,k,h,t,s;
  double p,q;

  printf("Enter total number of coupons and number of coupons of first type [m k]:\n");
  scanf("%d %d",&m,&k);

  printf("Enter probability for coupons of first type [p]:\n");
  scanf("%lf",&p);

  printf("Enter the number of iterations for empirical simulation [t]:\n");
  scanf("%d", &t);

  printf("Enter seed [s]:\n");
  scanf("%d", &s);

  h = m-k; // coupons of the second type
  q = (1-(k*p)) / h; // probability of coupons of the second type

  printf("Total coupons - m: %d\n", m);
  printf("Total coupons of the first type - k: %d\n", k);
  printf("The probability of coupon of the first type - p: %.2lf\n", p);
  printf("Total coupons of the second type - h: %d\n", h);
  printf("The probability of coupon of the first type - q: %.2lf\n", q);
  printf("\n\n");


  if( (k>m) || (p<0) || (p>1.0) || (m>50) || (m<2))
  {
    printf("Out of bound entries. %d\n", __LINE__);
    exit(0);
  }

  int sign, c;

  for(c=0; c<=m-1; c++)
  {
    if ( (m-1-c) % 2 == 0 )
    { // even
      sign = 1;
    }
    else
    {
      sign = -1;
    }

    comb(m,c,1,1,sign);
  }

  return 0;
}



void comb(int pool, int need, marker chosen, int at, int sign)
{
	if (pool < need + at) return; /* not enough bits left */

	if (!need)
  {
		/* got all we needed; print the thing.  if other actions are
		 * desired, we could have passed in a callback function. */
		for (at = 1; at < pool; at++)
    {
			if (chosen & (one << at))
      {
        term = at;
        term = sign*term;
        printf("%.2lf ", term);

      }
    }

    printf("\n");
		return;
	}
	/* if we choose the current item, "or" (|) the bit to mark it so. */
	comb(pool, need - 1, chosen | (one << at), at + 1, sign);
	comb(pool, need, chosen, at + 1, sign);  /* or don't choose it, go to next */
}
