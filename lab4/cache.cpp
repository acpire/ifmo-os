#include <iostream>
#include <stdlib.h>
#include <omp.h>

#define N 100

int sum_array_3d(int *a)
{ 
  int i, j, k, sum = 0; 
  for (i = 0; i < N; i++) 
    for (j = 0; j < N; j++) 
      for (k = 0; k < N; k++) 
        sum += a[k + i*N*N + j*N];
//        sum += a[i][j][k];
  return sum; 
} 


int main() {
  int *a = (int*)malloc(sizeof(int) * N * N * N);

  double start = omp_get_wtime();
  for (int i=0; i<1000; ++i) {
    sum_array_3d(a);
  }

  double stop = omp_get_wtime();
  std::cout << (stop - start) << std::endl;
}
