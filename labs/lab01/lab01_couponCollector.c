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


#define NBUG
#define SIGFIG 100

typedef uint64_t marker;

void print_Pj(int);
void getPj(int, int, double, double);
void comb(int, int, marker, int);
void print_prbs(int);
double rnd(double);
int min(int k, int q);
void pascal_triangle(int n);


marker one = 1;
double term;
double *P;
int *P2;

int cnt;
char BS = 0x08;
int main()
{
 /**
  * m - number of coupons
  * k - number of coupons of the first type
  * h - number of coupons of the second type
  * t - number of iterations for empirical simulation
  * p - probability of the first type of coupon (double)
  * q - probability of the second type of coupon (double)
  * s - seed for random natural number generator
  */
  int m,k,h,t,s,i;
  double p,q_;
  double expected_draws;

  printf("Enter total number of coupons and number of coupons of first type [m k]:\n");
  #ifdef NBUG
    m = 15;
    k = 5;
  #else
    scanf("%d %d",&m,&k);
  #endif

  printf("Enter probability for coupons of first type [p]:\n");
  #ifdef NBUG
    p = .1;
  #else
    scanf("%lf",&p);
  #endif

  printf("Enter the number of iterations for empirical simulation [t]:\n");
  #ifdef NBUG
    t = 23;
  #else
    scanf("%d", &t);
  #endif

  printf("Enter seed [s]:\n");
  #ifdef NBUG
    s = 23;
  #else
    scanf("%d", &s);
  #endif

  h = m-k; // coupons of the second type
  if(k>h) // keep k always smaller or equal to m/2
  {
    k = h;
    h = m-k;
    p = 1-p;
  }

  q_ = (1-(k*p)) / h; // probability of coupons of the second type

  printf("Total coupons - m: %d\n", m);
  printf("Total coupons of the first type - k: %d\n", k);
  printf("The probability of coupon of the first type - p: %.2lf\n", p);
  printf("Total coupons of the second type - h: %d\n", h);
  printf("The probability of coupon of the first type - q: %.2lf\n", q_);
  printf("The number of iterations for empirical simulation - t: %d\n", t);
  printf("The seed number for random number generator - s: %d \n", s);
  printf("\n");

  if( (k>m) || (p<0) || (p>1.0) || (m>50) || (m<2))
  {
    printf("Out of bound entries. %d\n", __LINE__);
    exit(0);
  }


  P = (double*) malloc((k+1)*sizeof(double));
  P2 = (int*) malloc((k+1)*sizeof(int));

  if(!P)
  {
    printf("Malloc failed! %d\n", __LINE__);
    exit(0);
  }

  // print probabilities
  double sum;
  printf("P_i = {");
  for(i=1; i<=m; i++)
  {
    if(i<=k)
    {
      printf("%.2lf, ", p);
      sum += p;
    }
    else
    {
      printf("%.2lf, ", q_);
      sum += q_;
    }
  }
  printf("%c%c} \n", BS, BS);
  printf("P_sum: %.2lf \n\n", sum);


  int sign, q;
  cnt = 0; // reset counter

  double term;


  for(q=0; q<m; q++)
  {
    int kqmin2 = min(k,q);
    double kChoose_j, tmp;
    int j;

    term = 0.0;

    printf("\n\n");
    printf("q:%d\n", q);

    if ( (m-1-q) % 2 == 0 ) {sign = 1;}
    else {sign = -1;}

    getPj(q,k,p,q_);
    pascal_triangle(m-kqmin2);

    for(j=0; j<=kqmin2; j++)
    {
      printf("j:%d\n", j);
      if(j==0)
      {
        kChoose_j = 1;
      }
      else
      {
        kChoose_j = rnd(kChoose_j * ((k+1-j) / j));
      }

      //printf("%dChoose_%d: %.2f \n", k,j, kChoose_j);
      //printf("%dChoose_%d: %d \n",m-kqmin2,q-j,*(P2+(q-j)));

      tmp = *(P+j) * kChoose_j;
      tmp = tmp * (*(P2+(q-j)));
      printf(" (P_%d:%.2lf)(%dChoose_%d:%.2lf)(%dChoose_%d:%d): %.2lf\n", \
      j,*(P+j),k,j,kChoose_j, m-kqmin2,q-j,*(P2+(q-j)),tmp);
      term += tmp;
    }
    printf("----->> term:(%d) %.2lf \n", sign, term);
    expected_draws += (term*sign);
  }

  printf("Theoretical expected draws: %.2lf (count %d)\n", expected_draws, cnt);




  free(P);
  free(P2);

  return 0;
}

void pascal_triangle(int n)
{
  unsigned int i,v;

    v = 1;
    for (i=0; i<=n; i++ )
    {
      printf("%3d ", v);
      *(P2+i) = v;
      v = v * (n-i)/(i+1);
    }
    printf("\n");
    return;
}

// round to sigfig decimal places
double rnd(double var)
{
  double value = (int)(var * SIGFIG + .5);
  return (double)(value/SIGFIG);
}

void getPj(int q, int k, double p, double q_)
{
  printf("getPj for q:%d\n", q);

  int i;
  double Tj, Pj;
  int kqmin = min(k,q);
  for(i=0; i<=kqmin; i++)
  {
    printf("   1/( 1-(%.2lf)*%d-(%.2lf)*(%d-%d))\n", p, i, q_, q, i);
    Pj= p*i + q_*(q-i);
    //printf("Pj: %.2lf\n", Pj);
    Tj = 1 - Pj;
    Tj = 1 / Tj;
    //printf("Tj: %.2lf\n", Tj);
    *(P+i) = rnd(Tj);
    cnt++;
  }
  printf("\n");
  print_Pj(k);
  return;
}

void print_Pj(int k)
{
  for (int i = 0; i<=k; i++)
  {
    printf("*(P+%d): %.2lf\n", i, *(P+i));
  }
  printf("\n");
  return;
}


int min(int k, int q)
{
  if(k<q)
    return k;
  else
    return q;
}
