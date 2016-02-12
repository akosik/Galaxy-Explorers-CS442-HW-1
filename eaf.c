//Galaxy Explorers
//Homework 1
//Assembly Float

#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>


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

  //We don't need output values, all changes are made to data either in the heap or in values not used
  //after we exit the loop
  asm volatile("loop:\n\t"
	       "movss (%4,%3,4), %%xmm0\n\t"
	       "movss (%0,%3,4), %%xmm1\n\t"
	       "addss %%xmm0, %%xmm1\n\t"
	       "movss %%xmm1, (%0,%3,4)\n\t"
	       "movss (%5,%3,4), %%xmm0\n\t"
	       "movss (%1,%3,4), %%xmm1\n\t"
	       "addss %%xmm0, %%xmm1\n\t"
	       "movss %%xmm1, (%1,%3,4)\n\t"
	       "movss (%6,%3,4), %%xmm0\n\t"
	       "movss (%2,%3,4), %%xmm1\n\t"
	       "addss %%xmm0, %%xmm1\n\t"
	       "movss %%xmm1, (%2,%3,4)\n\t"
	       "incq %3\n\t"
	       "cmpq %3, %7\n\t"
	       "jg loop"
	       :
	       :"r" (x), "r" (y), "r" (z), "r" (i), "r" (vx), "r" (vy), "r" (vz), "r" (size)
	       :"memory", "cc"
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

  //Free pointers, set to NULL
  free(x);
  free(y);
  free(z);
  free(vx);
  free(vy);
  free(vz);
  x = y = z = vx = vy = vz = NULL;

  return 0;
}
