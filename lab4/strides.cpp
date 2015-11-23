#include <iostream>
#include <omp.h>

const size_t BUFSIZE = 30 * 1024 * 1024;

int main()
{
	char *p = new char[BUFSIZE];

	// std::cout << "Filling buf..." << std::endl;
	for (size_t i = 0; i < BUFSIZE; ++i)
	{
		p[i] = char(i % 128);
	}
	// std::cout << "Elapsed milliseconds: " << (stop - start) << std::endl;

	for (size_t stride = 1; stride < 1000; stride += 10)
	{
		unsigned long sum = 0;
		double start = omp_get_wtime();
		for (size_t i = 0; i < BUFSIZE; ++i)
		{
			size_t index = (i * stride) % BUFSIZE;
			sum += static_cast<unsigned long>(p[index]);
		}
		double stop = omp_get_wtime();
		std::cout << (stop - start) << std::endl;
	}
	delete[] p;
	return 0;
}
