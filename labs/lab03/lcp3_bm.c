// Use suffix array and LCP to compute
// longest common substring of two input strings. BPW

// This version outputs multiple candidate solutions.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#define NBUG
#define MAX_STRING_LENGTH 500000
#define MSL MAX_STRING_LENGTH
#define MIL 2*MSL // max input string

char s[MIL],s1[MSL],s2[MSL], s3[MSL];
int dollarPos,hashPos,
  n,           // length of s[] including \0
  sa[MIL],   // suffix array for s[]
  rank[MIL], // rank[i] gives the rank (subscript) of s[i] in sa[]
  lcp[MIL];  // lcp[i] indicates the number of identical prefix symbols
                 // for s[sa[i-1]] and s[sa[i]]


int suffixCompare(const void *xVoidPt,const void *yVoidPt)
{
  // Used in qsort call to generate suffix array.
  int *xPt=(int*) xVoidPt,*yPt=(int*) yVoidPt;
  return strcmp(&s[*xPt],&s[*yPt]);
}

void computeRank()
{
  // Computes rank as the inverse permutation of the suffix array
  int i;

  for (i=0;i<n;i++)
    rank[sa[i]]=i;
}

void computeLCP()
{
  //Kasai et al linear-time construction
  int h,i,j,k;

  h=0;  // Index to support result that lcp[rank[i]]>=lcp[rank[i-1]]-1
  for (i=0;i<n;i++)
  {
    k=rank[i];
    if (k==0)
      lcp[k]=(-1);
    else
    {
      j=sa[k-1];
      // Attempt to extend lcp
      while (i+h<n && j+h<n && s[i+h]==s[j+h])
        h++;
      lcp[k]=h;
    }
    if (h>0)
      h--;  // Decrease according to result
  }
}

int fibIndex=0;

void fib(k)
int k;
{
  if (k==0)
    s[fibIndex++]='1';
  else if (k==1)
    s[fibIndex++]='0';
  else
  {
    fib(k-1);
    fib(k-2);
  }
}

int get_t(int index)
{
  if (index<=dollarPos)
    return 0;
  if (index<=hashPos)
    return 1;
  if (index<n)
    return 2;
  else
    return -1;
}

int main()
{
  int i,j,u,LCSpos=-1,LCSlength=0;
  int SCSlength=MSL,SCSpos=-1;

  scanf("%s",s1);
  scanf("%s",s2);
  scanf("%s",s3);

  // Concatenate the two input strings, but with a $ separator.
  for (i=0;s1[i]!='\0';i++)
    s[i]=s1[i];
  dollarPos=i;
  s[i++]='$';
  for (j=0;s2[j]!='\0';j++)
    s[i+j]=s2[j];
  hashPos=i+j;
  s[i+j++]='#';
  for(u=0; s3[u]!='\0'; u++)
    s[i+j+u]=s3[u];
  s[i+j+u]='\0';
  n=i+j+u+1;


  /*
  fib(4);
  fib(6);
  dollarPos=fibIndex;
  s[fibIndex++]='$';
  fib(5);
  fib(7);
  s[fibIndex]='\0';
  n=fibIndex+1;
  */


  /*
  for (i=0;i<1000;i++)
    s[i]='a';
  s[1000]='b';
  s[1001]='\0';
  */
  /*
  s[0]='a';
  for (i=1;i<1000;i++)
    s[i]='b';
  s[1000]='\0';
  */

  //n=strlen(s)+1;
  printf("n is %d\n",n);

  // Quick-and-dirty suffix array construction
  for (i=0;i<n;i++)
    sa[i]=i;
  qsort(sa,n,sizeof(int),suffixCompare);
  computeRank();
  computeLCP();
  if (n<3000)
  {
    printf("i     sa    suffix                              lcp   t s  rank   lcp[rank]\n");
    for (i=0;i<n;i++)
      printf("%-5d %-5d %-35.35s %-5d %-1d %c  %-5d  %-5d\n",
        i,sa[i],&s[sa[i]],lcp[i],get_t(sa[i]),
        s[i],rank[i],lcp[rank[i]]);
  }
  for (i=1;i<n;i++)
  {
    int x,y,z;
    x=get_t(sa[i-1]);
    y=get_t(sa[i]);
    z=get_t(sa[i+1]);

    //SCSlength = MSL;
    //SCSpos = -1;

    if( !(x==y) && !(x==z) ) //--------------------------------------------------------
    {
      SCSlength = lcp[i];
      SCSpos = i;
      if(y==z)
      {
        for(j=i; get_t(sa[j+1])!=y; j++) // only loop over similar y+ //--------------------------------------------------------
        {
#ifdef NBUG
          printf("in inner loop\n");
#endif
          if (lcp[j]<=SCSlength)
          {
            SCSlength = lcp[j];
            SCSpos = j;
          }
        }

        z=get_t(sa[j+1]);

        if(z==x) // no good, update index (i) and return to i-for loop
        {
          i = j; // update i pos
        }
        else if ((z!=x) && (z!=y) ) // we are gucci --- xy+z
        {
          if (lcp[j+1]<=SCSlength)
          {
            SCSlength = lcp[j+1];
            SCSpos = j+1;
            i = j; // update i pos
          }
        }
        else
        {
#ifdef NBUG
          printf(" >>>> ......................................  at line %d >>>> Err: This line should be unreachable!\n", __LINE__);
#endif
        }
      } // end of for loop - j
      else if ((x==y) || (x==z))
      {
#ifdef NBUG
        printf(" .. skip -- (x==y) || (x==z)\n");
#endif
      }
      else // should be valid ---- xyz
      {
#ifdef NBUG
        printf("  >>>> ...................................... simple xyz at i:%2d \n", i);
#endif
        if (lcp[i+1]<=SCSlength)
        {
          SCSlength = lcp[i+1];
          SCSpos = i+1;
        }
      }
    } // end of if statement for checking x vs y and z

    if (SCSlength>LCSlength)
    {
      LCSpos=SCSpos;
      LCSlength=lcp[SCSpos];
      printf("Length %2d, x at %2d, y ends at %2d, z at %2d \n",LCSlength,i-1,LCSpos,LCSpos+1);
      printf("%.*s\n",LCSlength,s+sa[LCSpos]);
    }
  } // end of outer for loop
}
