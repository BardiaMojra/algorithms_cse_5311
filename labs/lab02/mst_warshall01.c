// MST based on Warshall's algorithm (Maggs & Plotkin,
//    Information Processing Letters 26, 25 Jan 1988, 291-293)
// 3/6/03 BPW

// Modified 7/15/04 to make more robust, especially edges with same weight

#include <stdio.h>

#define maxSize (20)

struct edge {
  int weight,smallLabel,largeLabel;
};
typedef struct edge edgeType;

edgeType min(edgeType x,edgeType y)
{
// Returns smaller-weighted edge, using lexicographic tie-breaker
if (x.weight<y.weight)
  return x;
if (x.weight>y.weight)
  return y;

if (x.smallLabel<y.smallLabel)
  return x;
if (x.smallLabel>y.smallLabel)
  return y;

if (x.largeLabel<y.largeLabel)
  return x;
return y;
}

edgeType max(edgeType x,edgeType y)
{
// Returns larger-weighted edge, using lexicographic tie-breaker
if (x.weight>y.weight)
  return x;
if (x.weight<y.weight)
  return y;

if (x.smallLabel>y.smallLabel)
  return x;
if (x.smallLabel<y.smallLabel)
  return y;

if (x.largeLabel>y.largeLabel)
  return x;
return y;
}

int main()
{
int numVertices,numEdges, i, j, k;
edgeType matrix[maxSize][maxSize];
int count;

printf("enter # of vertices and edges: ");
fflush(stdout);
scanf("%d %d",&numVertices,&numEdges);
printf("enter undirected edges u v weight\n");
for (i=0;i<numVertices;i++)
  for (j=0;j<numVertices;j++)
  {
    matrix[i][j].weight=999;
    if (i<=j)
    {
      matrix[i][j].smallLabel=i;
      matrix[i][j].largeLabel=j;
    }
    else
    {
      matrix[i][j].smallLabel=j;
      matrix[i][j].largeLabel=i;
    }
  }
for (k=0;k<numEdges;k++)
{
  scanf("%d %d",&i,&j);
  scanf("%d",&matrix[i][j].weight);
  matrix[j][i].weight=matrix[i][j].weight;
}

printf("input matrix\n");
for (i=0;i<numVertices;i++)
{
  for (j=0;j<numVertices;j++)
    printf("%3d ",matrix[i][j].weight);
  printf("\n");
}

// MST by Warshall
for (k=0;k<numVertices;k++)
  for (i=0;i<numVertices;i++)
    for (j=0;j<numVertices;j++)
      matrix[i][j]=min(matrix[i][j],max(matrix[i][k],matrix[k][j]));

printf("output matrix\n");
for (i=0;i<numVertices;i++)
{
  for (j=0;j<numVertices;j++)
    printf("%3d(%3d,%3d) ",matrix[i][j].weight,matrix[i][j].smallLabel,
      matrix[i][j].largeLabel);
  printf("\n");
}

count=0;
for (i=0;i<numVertices;i++)
  for (j=i+1;j<numVertices;j++)
    if (matrix[i][j].weight<999 && i==matrix[i][j].smallLabel &&
        j==matrix[i][j].largeLabel)
    {
      count++;
      printf("%d %d %d\n",i,j,matrix[i][j].weight);
    }

if (count<numVertices-1)
  printf("Result is a spanning forest\n");
else if (count>=numVertices)
  printf("Error? . . . \n");
}
