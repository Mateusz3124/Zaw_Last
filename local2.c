#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

const int TILE_DIM = 32;
const int BLOCK_ROWS = 8;
const int NUM_REPS = 100;


long long wall_clock_time()
{
#ifdef __linux__
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	return (long long)(tp.tv_nsec + (long long)tp.tv_sec * 1000000000ll);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long long)(tv.tv_usec * 1000 + (long long)tv.tv_sec * 1000000000ll);
#endif
}

// Check errors and print GB/s
void postprocess(const float *ref, const float *res, int n, float ms)
{
  bool passed = true;
  for (int i = 0; i < n; i++)
    if (res[i] != ref[i]) {
      printf("%d %f %f\n", i, res[i], ref[i]);
      printf("%25s\n", "*** FAILED ***");
      passed = false;
      break;
    }
  if (passed)
    printf("%20.2f\n", 2 * n * sizeof(float) * 1e-6 * NUM_REPS / ms );
}

int main(int argc, char **argv)
{
  long long before, after;

  const int nx = 1024;
  const int ny = 1024;
  const int mem_size = nx*ny*sizeof(float);

  float *h_idata = (float*)malloc(mem_size);
  float *h_cdata = (float*)malloc(mem_size);
  float *h_tdata = (float*)malloc(mem_size);
  float *gold    = (float*)malloc(mem_size);



  for (int j = 0; j < ny; j++)
  {
    for (int i = 0; i < nx; i++)
    {
      h_idata[j*nx + i] = j*nx + i;
    }
  }
  before = wall_clock_time();
  // correct result for error checking
  for (int j = 0; j < ny; j++)
  {
    for (int i = 0; i < nx; i++)
    {
      gold[j*nx + i] = h_idata[i*nx + j];
    }
  }
  after = wall_clock_time();
  float ms = ((float)(after - before))/1000;
  printf("%25s%25s\n", "Routine", "Bandwidth (GB/s)");
  printf("%25s", "local");
  printf("%20.2f\n", 2 * nx * ny * sizeof(float) * 1e-6 * NUM_REPS / ms );

  //postprocess(gold, h_tdata, nx * ny, ms);

error_exit:
  free(h_idata);
  free(h_tdata);
  free(h_cdata);
  free(gold);
}
