/**
 *  @author Bardia Mojra
 *  @date 05/04/2021
 *  @brief Implementing minimum cut for undirected graph for cse5311 lab 02
 *  @note This code is based on implementations provided by Dr. Bob Weems of
 *  UTA-CSE department.
 *  @link http://ranger.uta.edu/~weems/NOTES5311/cse5311.html

https://www.youtube.com/results?search_query=maximum+flow+and+cut+trees
 https://en.wikipedia.org/wiki/Gomory%E2%80%93Hu_tree
https://www.youtube.com/watch?v=oHy3ddI9X3o

 **/

// Derived from
// http://www.aduni.org/courses/algorithms/handouts/Reciation_09.html#25630

// For Summer 2002 CSE 2320 Lab 4, the following changes were made to ff.c:
//   1. Adjacency lists (sorting, etc.)
//   2. Print minimum cut

// The Ford-Fulkerson Algorithm in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// For getrusage()
#include <sys/time.h>
#include <sys/resource.h>

// Basic Definitions
//#define NBUG
#define WHITE 0 // on the T side of the cut
#define GRAY 1
#define BLACK 2 // on the S side of the cut
#define oo 1000000000

// Declarations
int n;  // number of nodes
int residualEdges;  // number of edges in residual network
struct edge {
  int tail,head,capacity,flow,inverse;
};
typedef struct edge edgeType;
edgeType *edgeTab;
int *firstEdge;  // Table indicating first in range of edges with a common tail
int *color;      // Needed for breadth-first search
int *pred;       // Array to store augmenting path
int *predEdge;   // edgeTab subscript of edge used to reach vertex i in BFS

// for min-cut
int *fl; // hold max flow for an arbitrary cut
int *p; //

float CPUtime()
{
struct rusage rusage;

getrusage(RUSAGE_SELF,&rusage);
return rusage.ru_utime.tv_sec+rusage.ru_utime.tv_usec/1000000.0
       + rusage.ru_stime.tv_sec+rusage.ru_stime.tv_usec/1000000.0;
}

int min (int x, int y)
{
  return x<y ? x : y;  // returns minimum of x and y
}

// Queue for breadth-first search - not circular.

int head,tail;
int *q;

void enqueue (int x)
{
  q[tail] = x;
  tail++;
  color[x] = GRAY;
}

int dequeue ()
{
  int x = q[head];
  head++;
  color[x] = BLACK;
  return x;
}

// Breadth-First Search for an augmenting path

/**
 * Searches for path from start to target.
 * Returns 1 if found, otherwise 0.
 */
int bfs (int start, int target)
{
  int u,v,i;
  //printf("\n     --->> ln:%3d>>in BFS\n",__LINE__);

  for (u=0; u<n; u++)
    color[u] = WHITE; /// on the T side of the cut
  head = tail = 0;  // Since q is not circular, it is reinitialized for each BFS
  enqueue(start);
  pred[start] = -1;
  while (head!=tail)
  {
    u=dequeue();
    if (u==target)
      return 1;

    // Search all adjacent white nodes v. If the residual capacity
    // from u to v in the residual network is positive,
    // enqueue v.
    for (i=firstEdge[u]; i<firstEdge[u+1]; i++)
    {
      v=edgeTab[i].head;
      if (color[v]==WHITE && edgeTab[i].capacity-edgeTab[i].flow>0)
      {
        enqueue(v);
        pred[v] = u;
        predEdge[v]=i;
      }
    }
  }
  // No augmenting path remains, so a maximum flow and minimum cut
  // have been found.  Black vertices are in the
  // source side (S) of the minimum cut, while white vertices are in the
  // sink side (T).

  return 0; //(u==target); // or u==target
}

/*
void dfs(int s)
{
  int i,v;
  color[s] = BLACK; // mark source node as visited
  for (i=firstEdge[s]; i<firstEdge[s+1]; i++)
    {
      v=edgeTab[i].head;
      if (color[v]==WHITE && edgeTab[i].capacity-edgeTab[i].flow>0)
      {
        dfs(v);
      }
    }
}
*/

// Ford-Fulkerson Algorithm
int maxFlow(int source, int sink)
{
#ifdef NBUG
  printf("\n\nln:%3d>>in max-flow\n",__LINE__);
#endif
  int i,u;
  int max_flow;
  int APcount=0;

  color=(int*) malloc(n*sizeof(int));
  pred=(int*) malloc(n*sizeof(int));
  predEdge=(int*) malloc(n*sizeof(int));
  q=(int*) malloc(n*sizeof(int));

  if (!color || !pred || !predEdge || !q)
  {
    printf("malloc failed %d\n",__LINE__);
    exit(0);
  }

  // Initialize empty flow.
  max_flow = 0;
  for (i=0; i<residualEdges; i++)
    edgeTab[i].flow=0;

  // While there exists an augmenting path,
  // inc the flow along this path.
  while (bfs(source,sink))
  {
    // Determine the amount by which we can increment the flow.
    int inc = oo;
    APcount++;
    for (u=sink; pred[u]!=(-1); u=pred[u])
    {
      i=predEdge[u];
      inc = min(inc,edgeTab[i].capacity - edgeTab[i].flow);
    }
    // Now increment the flow.
    for (u=sink; pred[u]!=(-1); u=pred[u])
    {
      i = edgeTab[predEdge[u]].inverse;
      edgeTab[predEdge[u]].flow += inc;
      edgeTab[i].flow -= inc;  // Reverse in residual
    }
    if (n<=20)
    {
      // Path trace
    //  for (u=sink; pred[u]!=(-1); u=pred[u])
    //    printf("%d<-",u);
     // printf("%d adds %d incremental flow\n",source,inc);
    }
    max_flow += inc;
  }

#ifdef NBUG
  printf("\n --->ln:%d:  %d augmenting paths\n", __LINE__, APcount);
  printf("%d augmenting paths\n",APcount);
  if (n<=20)
  {
    printf("S side of min-cut:\n");
    for (u=0; u<n; u++)
      if (color[u]==BLACK)
        printf("%d\n",u);

    printf("T side of min-cut:\n");
    for (u=0; u<n; u++)
      if (color[u]==WHITE)
        printf("%d\n",u);
  }
#endif
  free(pred);
  free(predEdge);
  free(q);

  return max_flow;
}

int minCut(int source, int target)
{
  int s,i,t,a;
  int minCut_maxFlow = oo; // Very large value
  fl= (int*) malloc(n*sizeof(int));
  p = (int*) malloc(n*sizeof(int));
  memset(fl, 0, n);
  memset(p, 0, n);

  for (s=1; s<n; s++)
  {
    t = p[s];
    fl[s] = maxFlow(s,t);
#ifdef NBUG
    printf("ln:%3d>> max (%d,%d) flow value is %d\n", __LINE__, s,i,fl[s]);
    for (int b=0; b<n; b++)
      printf(" color: %d \n", color[b] );
#endif
    for (i=0; i<n; i++)
    {
      if(i!=s && color[i]==BLACK && p[i]==t)
      {
        p[i] = s;
#ifdef NBUG
        printf("ln:%3d>> %d is in X\n", __LINE__, i);
#endif
      }
    }


    if (color[p[t]] == BLACK)
    {
      p[s] = p[t];
      p[t] = s;
      fl[s] = fl[t];
      fl[t] = maxFlow(s,t);
    }
#ifdef NBUG
    printf("ln:%3d>> current tree: \n", __LINE__);
    for (a=1; a<=s; a++)
    {
      printf("%d %d %d \n", a, p[a], fl[a]);
    }
#endif
  }
  printf("ln:%3d>> Final tree: \n", __LINE__);
  for (a=1; a<n; a++)
  {
    printf("%d %d %d \n", a, p[a], fl[a]);
    if(fl[a] < minCut_maxFlow)
    {
      minCut_maxFlow = fl[a];
    }
  }

  free(color);
  return minCut_maxFlow;
}

// Reading the input file and organize adjacency lists for residual network.
int tailThenHead(const void* xin, const void* yin)
// Used in calls to qsort() and bsearch() for read_input_file()
{
  int result;
  edgeType *x,*y;
  x=(edgeType*) xin;
  y=(edgeType*) yin;
  result=x->tail - y->tail;
  if (result!=0)
    return result;
  else
    return x->head - y->head;
}

void dumpEdges(int count)
{
#ifdef NBUG
  int i;
  printf("ln:%3d>>\n", __LINE__);
  printf("  i tail head  cap\n");
  for (i=0; i<count; i++)
    printf("%3d %3d  %3d %5d\n",i,edgeTab[i].tail,edgeTab[i].head,
      edgeTab[i].capacity);
#endif
}

void dumpFinal()
{
#ifdef NBUG
  int i;
  printf("\n\nln:%3d>>Initialized residual network:\n", __LINE__);
  printf("Vertex firstEdge\n");
  for (i=0; i<n; i++)
    printf(" %3d    %3d\n",i,firstEdge[i]);
  printf("=================\n");
  printf(" %3d    %3d\n",n,firstEdge[n]);

  printf("\n\n  i tail head  cap  inv\n");
  for (i=0; i<residualEdges; i++)
    printf("%3d %3d  %3d %5d  %3d\n",i,edgeTab[i].tail,edgeTab[i].head,
      edgeTab[i].capacity,edgeTab[i].inverse);
#endif
}

void read_input_file()
{
  int tail,head,capacity,i,j;
  int inputEdges;     // Number of edges in input file.
  int workingEdges;   // Number of residual network edges initially
                      // generated from input file.
  edgeType work;
  edgeType *ptr;

  float startCPU,stopCPU;

  // read number of nodes and edges
  scanf("%d %d",&n,&inputEdges);
  // Table is allocated at worst-case size, since space for inverses is needed.
  edgeTab=(edgeType*) malloc(2*inputEdges*sizeof(edgeType));
  if (!edgeTab)
  {
    printf("ln:%3d>>edgeTab malloc failed\n",__LINE__);
    exit(0);
  }
  // read edges, each with a capacity
  workingEdges=0;
  for (i=0; i<inputEdges; i++)
  {
    scanf("%d %d %d",&tail,&head,&capacity);
    // Test for illegal edge, including incoming to source and outgoing from
    // sink.
    if (tail<0 || tail>=n || head<0 || head>=n || capacity<=0)
    {
      printf("Invalid input %d %d %d at %d\n",tail,head,capacity,__LINE__);
      exit(0);
    }
    // Save input edge
    edgeTab[workingEdges].tail=tail;
    edgeTab[workingEdges].head=head;
    edgeTab[workingEdges].capacity=capacity;
    workingEdges++;
    // Save inverse of input edge
    edgeTab[workingEdges].tail=head;
    edgeTab[workingEdges].head=tail;
    edgeTab[workingEdges].capacity=capacity; // undirected graph
    workingEdges++;
  }
  if (n<=20)
  {
#ifdef NBUG
    printf("\n\nln:%3d>>Input & inverses:\n", __LINE__);
    dumpEdges(workingEdges);
#endif
  }

  // Sort edges to make edges with common tail contiguous in edgeTab,
  // along with making parallel edges contiguous.
  // A faster sort is unlikely to speed-up substantially.
  startCPU=CPUtime();
  qsort(edgeTab,workingEdges,sizeof(edgeType),tailThenHead);
  stopCPU=CPUtime();
#ifdef NBUG
  printf("qsort CPU %f\n",stopCPU-startCPU);
  if (n<=20)
  {
    printf("\n\nln:%3d>>\n", __LINE__);
    printf("Sorted edges:\n");
    dumpEdges(workingEdges);
  }
#endif

  // Coalesce parallel edges into a single edge by adding their capacities.
  residualEdges=0;
  for (i=1; i<workingEdges; i++)
    if (edgeTab[residualEdges].tail==edgeTab[i].tail
    &&  edgeTab[residualEdges].head==edgeTab[i].head)
      edgeTab[residualEdges].capacity+=edgeTab[i].capacity;  // || case
    else
    {
      residualEdges++;
      edgeTab[residualEdges].tail=edgeTab[i].tail;
      edgeTab[residualEdges].head=edgeTab[i].head;
      edgeTab[residualEdges].capacity=edgeTab[i].capacity;
    }
  residualEdges++;
#ifdef NBUG
  if (n<=20)
  {
    printf("\n\nln:%3d>>\n", __LINE__);
    printf("Coalesced edges:\n");
    dumpEdges(residualEdges);
  }
#endif
  // Set field in each edgeTab struct to point to inverse
  startCPU=CPUtime();
  for (i=0; i<residualEdges; i++)
  {
    work.tail=edgeTab[i].head;
    work.head=edgeTab[i].tail;
    ptr=(edgeType*) bsearch(&work,edgeTab,residualEdges,sizeof(edgeType),
      tailThenHead);
    if (ptr==NULL)
    {
      printf("bsearch %d failed %d\n",i,__LINE__);
      exit(0);
    }
    edgeTab[i].inverse=ptr-edgeTab;  // ptr arithmetic to get subscript
  }
  stopCPU=CPUtime();
#ifdef NBUG
  printf("set inverses CPU %f\n",stopCPU-startCPU);
#endif
  // For each vertex i, determine first edge in edgeTab with
  // a tail >= i.
  firstEdge=(int*) malloc((n+1)*sizeof(int));
  if (!firstEdge)
  {
    printf("malloc failed %d\n",__LINE__);
    exit(0);
  }
  j=0;
  for (i=0; i<n; i++)
  {
    firstEdge[i]=j;
    // Skip over edges with vertex i as their tail.
    for ( ;
        j<residualEdges && edgeTab[j].tail==i;
        j++)
      ;
  }
  firstEdge[n]=residualEdges;  //Sentinel
  if (n<=20)
    dumpFinal();
}

int main ()
{
  int i;
  float startCPU,stopCPU;

  read_input_file();
  startCPU=CPUtime();
  printf("\ntotal flow is %d\n", minCut(0,n-1));  // 0=source, n-1=sink
  stopCPU=CPUtime();
  printf("Ford-Fulkerson time %f\n",stopCPU-startCPU);
#ifdef NBUG
 if (n<=20)
  {
    printf("flows along edges:\n");
    for (i=0; i<residualEdges; i++)
      if (edgeTab[i].flow>0)
        printf("%d->%d has %d\n",edgeTab[i].tail,
          edgeTab[i].head,edgeTab[i].flow);
  }
#endif
  free(edgeTab);
  free(firstEdge);
}
