
#include <omp.h>
#include <iostream>

void doSomething() {
   volatile int c = 0;
   for (int i=0; i<10000000; ++i) {
      for (int j=0; j<100; ++j) {
         c += 3;
      }
   }
}

int main() {

   // Starting the time measurement
   double start = omp_get_wtime();
   // Computations to be measured

   doSomething();

   // Measuring the elapsed time
   double end = omp_get_wtime();
   // Time calculation (in seconds)

   std::cout << (end - start) << std::endl;
}
