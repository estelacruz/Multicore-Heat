
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <string.h>
#include <omp.h>

/* To index element (i,j) of a 2D square array of dimension NxN stored as 1D */
#define index(i, j, N)  ((i)*(N)) + (j)

/* Number of threads, for the OpenMP version */
int numthreads = 0;

/*****************************************************************/

// Function declarations: Feel free to add any functions you want.
void  seq_heat_dist(float *, unsigned int, unsigned int);
void  parallel_heat_dist(float *, unsigned int, unsigned int);
void  check_result(int, unsigned int, float * ); // check that the parallel version and sequential versions match

/*****************************************************************/


int main(int argc, char * argv[])
{
  unsigned int N; /* Dimention of NxN matrix */
  int which_code = 0; // CPU or GPU
  int iterations = 0;
  int i,j;
  
  /* The 2D array of points will be treated as 1D array of NxN elements */
  float * playground; 
  
  // to measure time taken by a specific part of the code 
  double time_taken;
  clock_t start, end;
  
  if(argc != 5)
  {
    fprintf(stderr, "usage: heatdist num  iterations  who\n");
    fprintf(stderr, "num = dimension of the square matrix \n");
    fprintf(stderr, "iterations = number of iterations till stopping (1 and up)\n");
    fprintf(stderr, "who = 0: sequential code on CPU, 1: OpenMP version\n");
	fprintf(stderr, "threads = number of threads for the  OpenMP version\n");
    exit(1);
  }
  
  which_code = atoi(argv[3]);
  N = (unsigned int) atoi(argv[1]);
  iterations = (unsigned int) atoi(argv[2]);
  numthreads = (unsigned int) atoi(argv[4]);
 
  
  /* Dynamically allocate NxN array of floats */
  playground = (float *)calloc(N*N, sizeof(float));
  if( !playground )
  {
   fprintf(stderr, " Cannot allocate the %u x %u array\n", N, N);
   exit(1);
  }
  
  /* Initialize it: calloc already initalized everything to 0 */
  // Edge elements  initialization
  for(i = 0; i< N; i++) playground[index(i,0,N)] = 100;
  for(i = 0; i< N; i++) playground[index(i,N-1,N)] = 100;
  for(j = 0; j< N; j++) playground[index(0,j,N)] = 100;
  for(j = 0; j< N; j++) playground[index(N-1,j,N)] = 100;

  switch(which_code)
  {
	case 0: printf("CPU sequential version:\n");
			start = clock();
			seq_heat_dist(playground, N, iterations);
			end = clock();
			break;
		
	case 1: printf("OpenMP version:\n");
			start = clock();
			parallel_heat_dist(playground, N, iterations); 
			end = clock();  
			check_result(iterations, N, playground); 
			break;
			
			
	default: printf("Invalid device type\n");
			 exit(1);
  }
  
  time_taken = ((double)(end - start))/ CLOCKS_PER_SEC;
  
  printf("Time taken = %lf\n", time_taken);
  
  free(playground);
  
  return 0;

}


/*****************  The CPU sequential version  **************/
void  seq_heat_dist(float * playground, unsigned int N, unsigned int iterations)
{
  // Loop indices
  int i, j, k;
  int upper = N-1; //used instead of N to avoid updating the border points
  
  // number of bytes to be copied between array temp and array playground
  unsigned int num_bytes = 0;
  
  float * temp; 
  
  /* Dynamically allocate another array for temp values */
  temp = (float *)calloc(N*N, sizeof(float));
  if( !temp )
  {
   fprintf(stderr, " Cannot allocate temp %u x %u array\n", N, N);
   exit(1);
  }
  
  
  num_bytes = N*N*sizeof(float);
  
  /* Copy initial array in temp */
  memcpy((void *)temp, (void *) playground, num_bytes);
  
  for( k = 0; k < iterations; k++)
  {
    /* Calculate new values and store them in temp */
    for(i = 1; i < upper; i++)
      for(j = 1; j < upper; j++)
		temp[index(i,j,N)] = (playground[index(i-1,j,N)] + 
	                      playground[index(i+1,j,N)] + 
			      playground[index(i,j-1,N)] + 
			      playground[index(i,j+1,N)])/4.0;
  
			      
   			      
    /* Move new values into old values */ 
    memcpy((void *)playground, (void *) temp, num_bytes);
  }
  
  free(temp);
}

/*************** Runs the sequential version and compares its output to the parallel version ***/
void check_result(int iterations, unsigned int N, float * playground){
	
  float * temp;
  int i, j;
  
  temp = (float *)calloc(N*N, sizeof(float));
  if( !temp )
  {
   fprintf(stderr, " Cannot allocate temp %u x %u array in check_result\n", N, N);
   exit(1);
  }
  
  /* Initialize it: calloc already initalized everything to 0 */
  // Edge elements  initialization
  for(i = 0; i< N; i++) temp[index(i,0,N)] = 100;
  for(i = 0; i< N; i++) temp[index(i,N-1,N)] = 100;
  for(j = 0; j< N; j++) temp[index(0,j,N)] = 100;
  for(j = 0; j< N; j++) temp[index(N-1,j,N)] = 100;
  
  seq_heat_dist(temp, N, iterations);
  
  for(i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			if(fabsf(playground[index(i, j, N)] - temp[index(i, j, N)]) > 0.01)
			{
				printf("play[%d %d] = %f   temp[%d %d] = %f  index = %d\n", i, j, playground[index(i, j, N)], i, j, temp[index(i, j, N)], index(i, j, N));
				printf("There is a mismatch in some elements between the sequential and parallel version\n");
				free(temp);
				return;
			}
	
	printf("Result is correct!\n");
	free(temp);
	
	
	
}
/**********************************************************************************************/


/***************** The OpenMP version *********************/

/* Arguments of this function:
  playground: The array that contains the 2D points
  N: size of the 2D points is NxN
  iterations: number of iterations after which you stop.
*/
void  parallel_heat_dist(float * playground, unsigned int N, unsigned int iterations){

    //shared
    int numiter = 0;
    int i = 0;
    int j = 0;

    float *playground_new;
    //float *playground_old;
  

    // Allocate a new array to store the new playground.
    //initialize to zero
    playground_new = (float *)calloc(N * N, sizeof(float));
    //playground_old = (float *)calloc(N * N, sizeof(float));

    // __declspec(align(64)) float playground_new[N][N];

    // boundary conditions: edges have fixed temperature of 100
    //values are fixed and will not change
    //iterate through the number of rows : set temp of first and last col to 100
    //here index i is shared, parallelizing has not begun
    //each thread is assigned a row and updates its first and last col
    // k is the number of threads for the OpenMP version.

    //#pragma omp parallel for schedule(static) private(i)
    //numthreads = omp_get_num_threads(); //system call
    //each thread should update boundaries on each row
    //default: static type scheduling 
    //cant move on until all threads have updated boundaries
    
    //static, dynamic or guided schedule type : dynamic: load balance is the same so can work gere

    #pragma omp parallel for num_threads(numthreads) default(none) private(i) shared(playground_new, N) schedule(dynamic)   
    for (i = 0; i < N; i++) {
        //N = number of rows / columns
        // threads get their own private copy of i: regions are independent
        playground_new[index(i, 0, N)] = 100.0; //set left col of arr = 100 for each row i 
        playground_new[index(i, N-1, N)] = 100.0; //set right col of arr = 100 for each row i , update rightmost col at index N-1 for ith row
        playground_new[index(0, i, N)] = 100.0; //set top row of arr = 100 for each col i , update top-most row at index 0 for ith column
        playground_new[index(N-1, i, N)] = 100.0; // set bottom row of arr = 100 for each col i
        /*
        playground_old[index(i, 0, N)] = 100.0; 
        playground_old[index(i, N-1, N)] = 100.0;
        playground_old[index(0, i, N)] = 100.0;
        playground_old[index(N-1, i, N)] = 100.0;
        */
    }

    // for each iteration, each point in the grid will be uodated
    //the points in iteration i, must use old values to calc new, so do not update until

    for (numiter = 0; numiter < iterations; numiter++) {
      
      // after iterations for loop: Calc temp of the interior points using the average of the 4 neighboring points
      //all threads will access playground_new, each thread is assigned different indices/work on different parts of playground

      //parallelize inner loop: each thread is assigned region / interior points of grid to calculate temp val
      //static scheduling default
      //is load balance equal? yes, you are using old values for computations, so try dynamic scheduling 
     
      #pragma omp parallel for num_threads(numthreads) private(i, j) shared(playground, playground_new) schedule(dynamic)     
      for (i = 1; i < N-1; i++) {
          //iterates through rows. inner loop: skips boundary points, iterate through rows/cols
          for (j = 1; j < N-1; j++) {
              //iterates through columns
              //calc new temp value of interior point, store at corresponding index in new playground
              //should this be critical or atomic? 
              //should not be an issue since every thread is assigned different indicies, you write to different regions of new grid
              //when accessing old playground, you are just reading
              playground_new[index(i, j, N)] = (playground[index(i-1,j,N)] + playground[index(i+1,j,N)] + playground[index(i,j-1,N)] + playground[index(i,j+1,N)])/4.0;
          }
          //iteration computations are done, can update playground values now?
          //each thread is assigned a region so updating the playground shouldnt be an issue
      }
  
      // outside for parallezied loops: now that we have new values, update og playground
    
      #pragma omp parallel for num_threads(numthreads) private(i, j) shared(playground, playground_new) schedule(dynamic)   
      for (i = 1; i < N-1; i++) {
        //iterates through rows. inner loop: skips boundary points, iterate through rows/cols
        for (j = 1; j < N-1; j++) {
          //iterates through columns
          //calc new temp value of interior point, store at corresponding index in new playground
          playground[index(i, j, N)] = playground_new[index(i, j, N)];
        }
          //iteration computations are done, can update playground values now?
          //each thread is assigned a region so updating the playground shouldnt be an issue
      }

      //critical section
      //#pragma omp critical


    } //end iterations for loop
 
}

  

