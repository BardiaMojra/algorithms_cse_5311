// compile with: gcc -Wall -g lab01_couponCollector.c -lm or gcc lab01_couponCollector.c -lm


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


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>


//#define NBUG
//#define NBUG2
//#define NBUG_SIM_ONLY

#define SIGFIG 100

typedef uint64_t marker;

void print_Pj(int);
void getPj(int, int, double, double);
void comb(int, int, marker, int);
void print_prbs(int);
double rnd(double);
int min(int k, int q);
void pascal_triangle(int n);
int coupon_collection_simulation(int, int, double, double, int);
double gcd(double, double);


double term;
double *P;
int *P2;
int *C;
int *M;


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
    m = 7;
    k = 3;
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

#ifdef NBUG_SIM_ONLY
  coupon_collection_simulation(m, k, p, q_, s);

  free(P);
  free(P2);
  return 0;
#endif // end of #ifdef NBUGSIM_ONLY

  P = (double*) malloc((k+1)*sizeof(double));
  P2 = (int*) malloc((k+1)*sizeof(int));
  M = (int*) malloc((t+1)*sizeof(int));


  if(!P || !P2 || !M)
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

#ifdef NBUG
    printf("\n\n");
    printf("q:%d\n", q);
#endif

    if ( (m-1-q) % 2 == 0 ) {sign = 1;}
    else {sign = -1;}

    getPj(q,k,p,q_);
    pascal_triangle(m-kqmin2);

    for(j=0; j<=kqmin2; j++)
    {
#ifdef NBUG
      printf("j:%d\n", j);
#endif

      if(j==0)
      {
        kChoose_j = 1;
      }
      else
      {
        kChoose_j = rnd(kChoose_j * ((k+1-j) / j));
      }
#ifdef NBUG2
      printf("%dChoose_%d: %.2f \n", k,j, kChoose_j);
      printf("%dChoose_%d: %d \n",m-kqmin2,q-j,*(P2+(q-j)));
#endif
      tmp = *(P+j) * kChoose_j;
      tmp = tmp * (*(P2+(q-j)));
#ifdef NBUG
      printf(" (P_%d:%.2lf)(%dChoose_%d:%.2lf)(%dChoose_%d:%d): %.2lf\n", \
      j,*(P+j),k,j,kChoose_j, m-kqmin2,q-j,*(P2+(q-j)),tmp);
#endif
      term += tmp;
    }
#ifdef NBUG
    printf("----->> term:(%d) %.2lf \n", sign, term);
#endif
    expected_draws += (term*sign);
  }

  printf("Theoretical expected draws: %.2lf (count %d)\n", expected_draws, cnt);

  unsigned int sim_average = 0;

  for(i=0 ; i<t; i++)
  {
    sim_average += coupon_collection_simulation(m,k,p,q_,s);
  }


  printf("Empirical simulation counter: %.2f\n", (float)(sim_average/t));




  free(P);
  free(P2);
  free(M);

  return 0;
}

void pascal_triangle(int n)
{
  unsigned int i,v;

    v = 1;
    for (i=0; i<=n; i++ )
    {
#ifdef NBUG
      printf("%3d ", v);
#endif
      *(P2+i) = v;
      v = v * (n-i)/(i+1);
    }
#ifdef NBUG
    printf("\n");
#endif
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

#ifdef NBUG
  printf("getPj for q:%d\n", q);
#endif

  int i;
  double Tj, Pj;
  int kqmin = min(k,q);
  for(i=0; i<=kqmin; i++)
  {
#ifdef NBUG
    printf("   1/( 1-(%.2lf)*%d-(%.2lf)*(%d-%d))\n", p, i, q_, q, i);
#endif
    Pj= p*i + q_*(q-i);
    //printf("Pj: %.2lf\n", Pj);
    Tj = 1 - Pj;
    Tj = 1 / Tj;
    //printf("Tj: %.2lf\n", Tj);
    *(P+i) = rnd(Tj);
    cnt++;
  }
#ifdef NBUG
  printf("\n");
  print_Pj(k);
#endif
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


int coupon_collection_simulation(int m, int k, double p, double q, int s)
{
  int r;
  srand(s);

  int i;

  C = (int*) malloc((k+1)*sizeof(int));


  double gcd_pq =  rnd(gcd(rnd(p), rnd(q)));
  int normed_p = (int) rnd(p/gcd_pq);
  int normed_q = (int) rnd(q/gcd_pq);
  int total_tokens = (normed_p*k)+(normed_q*(m-k));

#ifdef NBUG
  double norm_ratio;
 if (p<q)
  {
    norm_ratio = rnd(1.0/p);
  }
  else
  {
    norm_ratio = rnd(1.0/q);
  }

  printf("\n\n");
  printf("p: %.2lf\n", p);
  printf("q: %.2lf\n", q);
  printf("gcd_pq: %.2lf\n", gcd_pq);
  printf("normed_p: %d\n", normed_p);
  printf("normed_q: %d\n", normed_q);
  printf("Total_tokens: %d\n", total_tokens);
  printf("norm_ratio: %.2lf\n", norm_ratio);
#endif

  int sim_cntr = 0;

  int coup_;
  for (i=0; i<=50; i++)
  {
    *(C+i) = 0;
  }
  int cntr2;
  int all_done = 0;
  while(!all_done)
  {
    r = rand() % total_tokens;
    double r_double = (double) r;
#ifdef NBUG
    printf("\n\n");
    printf("r: %d\n", r);
    printf("r_double: %.2lf\n", r_double);
    printf("\n\n k threshold: %d\n", k*normed_p);
#endif

    if (r <= k*normed_p)
    {
      r_double = r_double/normed_p;
      r_double = (trunc(r_double+.5));
#ifdef NBUG
      printf("p type\n");
      printf("r_double: %.2lf\n", r_double);
#endif
      coup_  = (int) trunc(r_double+.5)+1;
      *(C+coup_) = 1;
    }
    else
    {
      r_double = r_double - (k*normed_p);
      r_double = r_double/normed_q;
      r_double = r_double + k;
      coup_  = (int) trunc(r_double+.5) +1;
#ifdef NBUG
    printf("q type\n");
    printf("r_double: %.2lf\n", r_double);
#endif
      *(C+coup_) = 1;
    }

    cntr2 = 0;
    for(i=1; i<=m; i++)
    {
     //  printf("*(C+%d): %d\n", i, *(C+i));
      if (*(C+i)== 1)
      {
        cntr2++;
      }
    }
     if (cntr2 == m)
     {
       all_done = 1;
     }

    sim_cntr++;

#ifdef NBUG
  printf("\n");
  printf("sim_cntr: %d\n", sim_cntr);
#endif

  } // end of while loop

#ifdef NBUG
  printf("Empirical simulation counter: %d\n", sim_cntr);
#endif
  free(C);
  return sim_cntr;
}

double gcd(double a, double b)
{
  if (a < b)
  {
    return gcd(b, a);
  }

  // base case
  if (fabs(b) < 0.001)
  {
    return a;
  }
  else
  {
    return (gcd(b, a - floor(a / b) * b));
  }
}
