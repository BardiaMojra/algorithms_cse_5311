#include <stdio.h>
#include <stdlib.h>

void pascaltriangle(unsigned int n)
{
  double  c;
  unsigned int j, k;

  //for(i=0; i < n; i++) {
    c = 1;
    for(k=0; k <= n; k++) {
      printf("%3.0lf  ", c);
      c = c * (n-k)/(k+1);
    }
    printf("\n");
  //}
}

void triangleC(int nRows) {
    if (nRows <= 0) return;
    int *prevRow = NULL;
    for (int r = 1; r <= nRows; r++) {
        int *currRow = malloc(r * sizeof(int));
        for (int i = 0; i < r; i++) {
            int val = i==0 || i==r-1 ? 1 : prevRow[i-1] + prevRow[i];
            currRow[i] = val;
            printf(" %4d", val);
        }
        printf("\n");
        free(prevRow);
        prevRow = currRow;
    }
    free(prevRow);
}

int main()
{
  pascaltriangle(50);
  printf("\n");
  //triangleC(8);

  return 0;
}
