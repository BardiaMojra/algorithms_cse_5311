// Use suffix array and LCP to compute
// longest common substring of two input strings. BPW

// This version outputs multiple candidate solutions.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char s[1000000],s1[500000],s2[500000], s3[500000];
int dollarPos,hashPos,
  n,           // length of s[] including \0
  sa[1000000],   // suffix array for s[]
  rank[1000000], // rank[i] gives the rank (subscript) of s[i] in sa[]
  lcp[1000000];  // lcp[i] indicates the number of identical prefix symbols
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
  if (index<hashPos)
    return 1;
  if (index<n)
    return 2;
  else
    return -1;
}

int main()
{
  int i,j,u,k,p,LCSpos=-1,LCSlength=1;


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
    if (sa[i-1]<=dollarPos && sa[i]>dollarPos
      ||  sa[i-1]>dollarPos && sa[i]<=dollarPos)
    {
      if (lcp[i]>=LCSlength)
      {
        LCSpos=i;
        LCSlength=lcp[i];
        printf("Length of longest common substring is %d at positions %d %d\n",LCSlength,i-1,i);
        printf("%.*s\n",LCSlength,s+sa[LCSpos]);
      }
    }
  }
}
