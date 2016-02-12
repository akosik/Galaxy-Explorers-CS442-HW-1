//Galaxy Explorers
//Homework 1
//Vectorized Double

#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <xmmintrin.h>

double sum(double* list, uint64_t size)
{
  uint64_t i;
  double sum = 0.0;
  for(i = 0; i < size; i++) sum += list[i];
  return sum;
}

double* generate_random_list(uint64_t size, double bound)
{
  double* list = (double*) malloc(sizeof(double) * size);

  uint64_t i;
  for(i = 0; i < size; i++) list[i] = bound * (rand() / (double)(RAND_MAX / 2)) - bound;

  return list;
}

void update_coords(double* x, double* y, double* z, double* vx, double* vy, double* vz, uint64_t size)
{
  uint64_t i = 0;

  //Loop through each of the three array pairs at double time (loading 2 doubles into vector registers) 
  //until array is exhausted or there is one element left, at which time we update it and exit.

  //We don't need output values, all changes are made to data either in the heap or in values not used
  //after we exit the loop
  asm volatile("movq %7, %%rax\n\t"
	       "subq $1, %%rax\n\t"
	       "loop_double_time:\n\t"
	       "movupd (%4), %%xmm0\n\t"
	       "movupd (%0), %%xmm1\n\t"
	       "addpd %%xmm0, %%xmm1\n\t"
	       "movupd %%xmm1, (%0)\n\t"
	       "movupd (%5), %%xmm0\n\t"
	       "movupd (%1), %%xmm1\n\t"
	       "addpd %%xmm0, %%xmm1\n\t"
	       "movupd %%xmm1, (%1)\n\t"
	       "movupd (%6), %%xmm0\n\t"
	       "movupd (%2), %%xmm1\n\t"
	       "addpd %%xmm0, %%xmm1\n\t"
	       "movupd %%xmm1, (%2)\n\t"
	       "addq $16, %4\n\t"
	       "addq $16, %0\n\t"
	       "addq $16, %5\n\t"
	       "addq $16, %1\n\t"
	       "addq $16, %6\n\t"
	       "addq $16, %2\n\t"
	       "addq $2, %3\n\t"
	       "cmp %3, %%rax\n\t"
	       "je loop_one_at_a_time\n\t"
	       "cmp %3, %7\n\t"
	       "jg loop_double_time\n\t"
	       "je loop_end\n\t"
	       "loop_one_at_a_time:\n\t"
	       "movsd (%4), %%xmm0\n\t"
	       "movsd (%0), %%xmm1\n\t"
	       "addsd %%xmm0, %%xmm1\n\t"
	       "movsd %%xmm1, (%0)\n\t"
	       "movsd (%5), %%xmm0\n\t"
	       "movsd (%1), %%xmm1\n\t"
	       "addsd %%xmm0, %%xmm1\n\t"
	       "movsd %%xmm1, (%1)\n\t"
	       "movsd (%6), %%xmm0\n\t"
	       "movsd (%2), %%xmm1\n\t"
	       "addsd %%xmm0, %%xmm1\n\t"
	       "movsd %%xmm1, (%2)\n\t"
	       "loop_end:\n\t"
	       :
	       :"r" (x), "r" (y), "r" (z), "r" (i), "r" (vx), "r" (vy), "r" (vz), "r" (size)
	       :"xmm0", "xmm1", "memory", "cc", "rax"
	       );
}

int main(int argc, char* argv[])
{
  if(argc != 3)
    {
      printf("Required arguments: vector_length(N) and iterations_num(M). \n");
      return -1;
    }

  //Get Arguments: size and iterations
  char *endptr;
  uint64_t size = (uint64_t)strtoull(argv[1],&endptr,10);
  uint64_t iters = (uint64_t)strtoull(argv[2],&endptr,10);

  //Generate seed
  struct timespec start, end;
  srand(size);

  //Initialize Coordinates
  double* x = generate_random_list(size,1000.0);
  double* y = generate_random_list(size,1000.0);
  double* z = generate_random_list(size,1000.0);
  double* vx = generate_random_list(size,1.0);
  double* vy = generate_random_list(size,1.0);
  double* vz = generate_random_list(size,1.0);

  //Time update AKA the meat of the Operation :)
  clock_gettime(CLOCK_MONOTONIC, &start);
  uint64_t i;
  for(i = 0; i < iters; i++) update_coords(x, y, z, vx, vy, vz, size);
  clock_gettime(CLOCK_MONOTONIC, &end);

  //Calculate time elapsed and time per Coord, then print
  double elapsed = 1000000000.0 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  double timePerCoord = elapsed / (size * iters);
  printf("Mean time per coordinate: %f nanoseconds. \n", timePerCoord);

  //Calculate Checksum
  double checksum = sum(x,size) + sum(y,size) + sum(z,size);
  printf("Checksum: %f \n", checksum);

  free(x);
  free(y);
  free(z);
  free(vx);
  free(vy);
  free(vz);
  x = y = z = vx = vy = vz = NULL;

  return 0;
}
