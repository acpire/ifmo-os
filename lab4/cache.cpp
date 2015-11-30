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
        sum += a[k*N*N + j*N + i];
  return sum; 
} 


int main() {
  int *a = (int*)malloc(sizeof(int) * N * N * N);

  double start = omp_get_wtime();
  for (int i=0; i<3000; ++i) {
    sum_array_3d(a);
  }

  double stop = omp_get_wtime();
  std::cout << (stop - start) << std::endl;
}
