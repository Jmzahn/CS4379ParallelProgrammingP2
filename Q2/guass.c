/* Demostration code - Gaussian elimination without pivoting.
    Modified by: Jacob Zahn
 */
#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>

/* Program Parameters */
#define MAXN 2000  /* Max value of N */
int N;  /* Matrix size */
int threads;  /* Number of processors to use */

/* Matrices and vectors */
volatile double A[MAXN][MAXN], B[MAXN], X[MAXN];
volatile double L[MAXN][MAXN];
/* A * X = B, solve for X */

pthread_barrier_t   barrier; // the barrier synchronization object

/* Prototype */
void gauss();  /* The function you will provide.
		* It is this routine that is timed.
		* It is called only on the parent.
		*/

struct arg_struct {
    int normStart;
    int normStride;
};

void *LUcolWise(void *args);

/* returns a seed for srand based on the time */
unsigned int time_seed() {
  struct timeval t;

  gettimeofday(&t, NULL);
  return (unsigned int)(t.tv_usec);
}

/* Set the program parameters from the command-line arguments */
void parameters(int argc, char **argv) {
  int submit = 0;  /* = 1 if submission parameters should be used */
  int seed = 0;  /* Random seed */
  

  /* Read command-line arguments */
  srand(time_seed());  /* Randomize */
  if (argc != 3) {
    if ( argc == 2 && !strcmp(argv[1], "submit") ) {
      /* Use submission parameters */
      submit = 1;
      N = 4;
      threads = 2;
      printf("\nSubmission run for \"%s\".\n", getlogin());
    }
    else {
      if (argc == 4) {
	      seed = atoi(argv[3]);
	      srand(seed);
	      printf("Random seed = %i\n", seed);
      }
      else {
	      printf("Usage: %s <matrix_dimension> <num_threads> [random seed]\n",
	       argv[0]);
	      printf("       %s submit\n", argv[0]);
	      exit(0);
      }
    }
  }
  /* Interpret command-line args */
  if (!submit) {
    N = atoi(argv[1]);
    if (N < 1 || N > MAXN) {
      printf("N = %i is out of range.\n", N);
      exit(0);
    }
    threads = atoi(argv[2]);
    if (threads < 1) {
      printf("Warning: Invalid number of processors = %i.  Using 1.\n", threads);
      threads = 1;
    }
    
  }

  /* Print parameters */
  printf("\nMatrix dimension N = %i.\n", N);
  printf("Number of threads = %i.\n", threads);
}

/* Initialize A and B (and X to 0.0s) */
void initialize_inputs() {
  int row, col;

  printf("\nInitializing...\n");
  for (col = 0; col < N; col++) {
    for (row = 0; row < N; row++) {
      A[row][col] = (float)rand() / 32768.0;
    }
    B[col] = (float)rand() / 32768.0;
    X[col] = 0.0;
  }

}

/* Print input matrices */
void print_inputs() {
  int row, col;

  if (N <= 10) {
    printf("\nA =\n\t");
    for (row = 0; row < N; row++) {
      for (col = 0; col < N; col++) {
	      printf("%5.2f%s", A[row][col], (col < N-1) ? ", " : ";\n\t");
      }
    }
    printf("\nB = [");
    for (col = 0; col < N; col++) {
      printf("%5.2f%s", B[col], (col < N-1) ? "; " : "]\n");
    }
  }
}

void print_X() {
  int row;

  if (N <= 10) {
    printf("\nX = [");
    for (row = 0; row < N; row++) {
      printf("%5.2f%s", X[row], (row < N-1) ? "; " : "]\n");
    }
  }
}

void main(int argc, char **argv) {
  /* Timing variables */
  struct timeval etstart, etstop;  /* Elapsed times using gettimeofday() */
  clock_t etstart2, etstop2;  /* Elapsed times using times() */
  unsigned long long usecstart, usecstop;
  struct tms cputstart, cputstop;  /* CPU times for my processes */

  /* Process program parameters */
  parameters(argc, argv);

  /* Initialize A and B */
  initialize_inputs();

  /* Print input matrices */
  print_inputs();

  /* Start Clock */
  printf("\nStarting clock.\n");
  gettimeofday(&etstart, NULL);
  etstart2 = times(&cputstart);

  /* Gaussian Elimination */
  gauss();

  /* Stop Clock */
  gettimeofday(&etstop, NULL);
  etstop2 = times(&cputstop);
  printf("Stopped clock.\n");
  usecstart = (unsigned long long)etstart.tv_sec * 1000000 + etstart.tv_usec;
  usecstop = (unsigned long long)etstop.tv_sec * 1000000 + etstop.tv_usec;

  /* Display output */
  print_X();

  /* Display timing results */
  printf("\nElapsed time = %g ms.\n",
	 (float)(usecstop - usecstart)/(float)1000);
  /*printf("               (%g ms according to times())\n",
   *       (etstop2 - etstart2) / (float)CLK_TCK * 1000);
   */
  printf("(CPU times are accurate to the nearest %g ms)\n",
	 1.0/(float)CLOCKS_PER_SEC * 1000.0);
  printf("My total CPU time for parent = %g ms.\n",
	 (float)( (cputstop.tms_utime + cputstop.tms_stime) -
		  (cputstart.tms_utime + cputstart.tms_stime) ) /
	 (float)CLOCKS_PER_SEC * 1000);
  printf("My system CPU time for parent = %g ms.\n",
	 (float)(cputstop.tms_stime - cputstart.tms_stime) /
	 (float)CLOCKS_PER_SEC * 1000);
  printf("My total CPU time for child processes = %g ms.\n",
	 (float)( (cputstop.tms_cutime + cputstop.tms_cstime) -
		  (cputstart.tms_cutime + cputstart.tms_cstime) ) /
	 (float)CLOCKS_PER_SEC * 1000);
      /* Contrary to the man pages, this appears not to include the parent */
  printf("--------------------------------------------\n");

}

/* ------------------ Above Was Provided --------------------- */

/****** You will replace this routine with your own parallel version *******/
/* Provided global variables are MAXN, N, threads, A[][], B[], and X[],
 * defined in the beginning of this code.  X[] is initialized to zeros.
 */
void gauss() {
  int row, col;  /* Normalization row, and zeroing
			* element row and col */
  

  //create array of args
  struct arg_struct *argArray[threads];
  
  //create pthread_t array
  pthread_t blocks[threads];

  //initialize barrier
  pthread_barrier_init(&barrier, NULL, threads);

  if(threads!=1){
    printf("Computing parallelly.\n");
  }
  else{
    printf("Computing serially.\n");
  }

  //create threads
  for(int i = 0; i < threads; i ++){
    argArray[i] = (struct arg_struct *)malloc(sizeof(struct arg_struct));
    argArray[i]->normStart = i;
    argArray[i]->normStride = threads;
    pthread_create(&blocks[i], NULL, &LUcolWise, (void *)argArray[i]);
  }
  
  //join threads
  for(int i = 0; i < threads; i ++){
    pthread_join(blocks[i], NULL);
  }

  //complete calculation

  //free memory
  for(int i = 0; i < threads; i ++){
    free( argArray[i] );
  }
  
  /* Gaussian elimination 
  for (norm = 0; norm < N - 1; norm++) {

    for (row = norm + 1; row < N; row++) {
      multiplier = A[row][norm] / A[norm][norm];
      for (col = norm; col < N; col++) {
	      A[row][col] -= A[norm][col] * multiplier;
      }
      B[row] -= B[norm] * multiplier;
    }
  }*/
  
  /* (Diagonal elements are not normalized to 1.  This is treated in back
   * substitution.)
   */


  /* Back substitution */

  //at this point U is stored in A

  //from the last row to the first
  for(row = N-1; row >= 0; row--) {
    //set X[row] to the multiplier of that row : B[row]
    X[row] = B[row];
    //for every col such that row<col<N
    for(col = row+1; col < N; col++) {
      //update X[row] via elimination 
      X[row] -= A[row][col] * X[col];
    }
    //finally update X[row] by dividing by U[row][row]
    X[row] /= A[row][row];
  }
  
}

void *LUcolWise(void* args){
  //interpret args
  int normStart = ((struct arg_struct*)args)->normStart;
  int normStride = ((struct arg_struct*)args)->normStride;
  
  //for every col thread is assigned
  for(int k = normStart; k < N; k+=normStride){
    //for every row k+1->N
    for(int i = k+1; i < N; i++){
      //store values of L 
      L[i][k] = A[i][k] / A[k][k];
      //calculate multipliers for ith eqn
      B[i] -= L[i][k]*B[k];
    }
    //making sure to traverse col wise for even load balancing
    for(int i = k+1; i < N; i++){
      for(int j = k+1; j < N; j++){
        //update submatrix via elimination
        A[i][j] -=  L[i][k] * A[k][j];
      }
    }
    //barrier for synchronizing matrix update of threads
    pthread_barrier_wait(&barrier);
  }
}