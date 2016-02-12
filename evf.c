//Galaxy Explorers
//Homework 1
//Vectorized Float

#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <xmmintrin.h>

double sum(float* list, uint64_t size)
{
  uint64_t i;
  double sum = 0.0;
  for(i = 0; i < size; i++) sum += list[i];
  return sum;
}

float* generate_random_list(uint64_t size, float bound)
{
  float* list = (float*) malloc(sizeof(float) * size);

  uint64_t i;
  for(i = 0; i < size; i++) list[i] = bound * (rand() / (float)(RAND_MAX / 2)) - bound;

  return list;
}

void update_coords(float* x, float* y, float* z, float* vx, float* vy, float* vz, uint64_t size)
{
  uint64_t i = 0;

  //Loop through each of the three array pairs (x and vx, y and vy, ...) at quadruple time (loading 4 floats into vector registers) 
  //until array is exhausted or there is less than 4 elements left, at which time we loop by one until done and exit.
  //If the size is a multiple of 4 then we jump to another label for he last 4 floats to avoid incrementing counters and pointers. 

  //We don't need output values, all changes are made to data either in the heap or in values not used
  //after we exit the loop
  asm volatile("movq %7, %%rax\n\t"
	       "subq $4, %%rax\n\t"

	       "loop_quad_time:\n\t"
	       "movups (%4), %%xmm0\n\t"
	       "movups (%0), %%xmm1\n\t"
	       "addps %%xmm0, %%xmm1\n\t"
	       "movups %%xmm1, (%0)\n\t"
	       "movups (%5), %%xmm0\n\t"
	       "movups (%1), %%xmm1\n\t"
	       "addps %%xmm0, %%xmm1\n\t"
	       "movups %%xmm1, (%1)\n\t"
	       "movups (%6), %%xmm0\n\t"
	       "movups (%2), %%xmm1\n\t"
	       "addps %%xmm0, %%xmm1\n\t"
	       "movups %%xmm1, (%2)\n\t"
	       "addq $16, %4\n\t"
	       "addq $16, %0\n\t"
	       "addq $16, %5\n\t"
	       "addq $16, %1\n\t"
	       "addq $16, %6\n\t"
	       "addq $16, %2\n\t"
	       "addq $4, %3\n\t"
	       "cmp %3, %%rax\n\t"
	       "jg loop_quad_time\n\t"
	       "je loop_last_time\n\t"

	       "loop_one_at_a_time:\n\t"
	       "movss (%4), %%xmm0\n\t"
	       "movss (%0), %%xmm1\n\t"
	       "addss %%xmm0, %%xmm1\n\t"
	       "movss %%xmm1, (%0)\n\t"
	       "movss (%5), %%xmm0\n\t"
	       "movss (%1), %%xmm1\n\t"
	       "addss %%xmm0, %%xmm1\n\t"
	       "movss %%xmm1, (%1)\n\t"
	       "movss (%6), %%xmm0\n\t"
	       "movss (%2), %%xmm1\n\t"
	       "addss %%xmm0, %%xmm1\n\t"
	       "movss %%xmm1, (%2)\n\t"
	       "cmp %3, %7\n\t"
	       "jl loop_one_at_a_time\n\t"
	       "je loop_end\n\t"

	       "loop_last_time:\n\t"
	       "movups (%4), %%xmm0\n\t"
	       "movups (%0), %%xmm1\n\t"
	       "addps %%xmm0, %%xmm1\n\t"
	       "movups %%xmm1, (%0)\n\t"
	       "movups (%5), %%xmm0\n\t"
	       "movups (%1), %%xmm1\n\t"
	       "addps %%xmm0, %%xmm1\n\t"
	       "movups %%xmm1, (%1)\n\t"
	       "movups (%6), %%xmm0\n\t"
	       "movups (%2), %%xmm1\n\t"
	       "addps %%xmm0, %%xmm1\n\t"
	       "movups %%xmm1, (%2)\n\t"
	       
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
  float* x = generate_random_list(size,1000.0);
  float* y = generate_random_list(size,1000.0);
  float* z = generate_random_list(size,1000.0);
  float* vx = generate_random_list(size,1.0);
  float* vy = generate_random_list(size,1.0);
  float* vz = generate_random_list(size,1.0);

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
