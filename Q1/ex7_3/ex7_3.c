/*
    Example 7.3 from Introduction to Parallel Computing, Grama et al.
    additional code added by Jacob Zahn
    borrowing timing from project2/Q2/guass.c
*/
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>

#define INT_MAX 2147483647

void *find_min(void *list_ptr);
pthread_mutex_t minimum_value_lock;
int minimum_value, partial_list_size;

int main(int argc, char *argv[]){
    /* declare and initialize data structures and list*/
    /* Timing variables */
    struct timeval etstart, etstop;  /* Elapsed times using gettimeofday() */
    struct timezone tzdummy;
    clock_t etstart2, etstop2;  /* Elapsed times using times() */
    unsigned long long usecstart, usecstop;
    struct tms cputstart, cputstop;  /* CPU times for my processes */
    minimum_value = INT_MAX;
    //pthread_init();
    pthread_mutex_init(&minimum_value_lock, NULL);
    int numP;//number of processors
    
    /* initialize lists, list_ptr, and partial_list_size */
    /* create and join threads here */
    if(argc != 2){
        return(-1);
    }
    else{
        char *p;
        long conv = strtol(argv[1], &p, 10);
        numP = conv;
        pthread_t finders[numP];
        void* list_ptrs[numP];
        double remainder = 10000000%numP;
        if(remainder!=0.0){
            return(-1);
        }
        int randArray[10000000];
        srand(time(NULL));
        for(int i = 0; i < 10000000; i++){
            randArray[i] = rand();
        }
        
        int split = 10000000/numP;
        partial_list_size = split;
        int** A = (int**)malloc(sizeof(int*)*numP);
        for(int i = 0; i < numP; i++){
            A[i] = &randArray[i*partial_list_size];
        }
        

        /* Start Clock */
        printf("\nStarting clock.\n");
        gettimeofday(&etstart, &tzdummy);
        etstart2 = times(&cputstart);
        
        for(int i = 0; i < numP; i++){
            list_ptrs[i] = A[i];
            pthread_create(&finders[i],NULL,find_min,list_ptrs[i]);
        }
        for(int i = 0; i < numP; i++){
            pthread_join(finders[i],0);
        }

        /* Stop Clock */
        gettimeofday(&etstop, &tzdummy);
        etstop2 = times(&cputstop);
        printf("Stopped clock.\n");
        usecstart = (unsigned long long)etstart.tv_sec * 1000000 + etstart.tv_usec;
        usecstop = (unsigned long long)etstop.tv_sec * 1000000 + etstop.tv_usec;
        /* Display timing results */
        printf("\nElapsed time = %g ms.\n",
        	 (float)(usecstop - usecstart)/(float)1000);
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
        free( A[0] );
        free( A );
    }
    
    return 0;

}

void *find_min(void *list_ptr){
    int *partial_list_pointer, my_min, i;
    my_min = INT_MAX;
    partial_list_pointer = (int *) list_ptr;
    for(i=0; i < partial_list_size; i++)
        if(partial_list_pointer[i] < my_min)
            my_min = partial_list_pointer[i];
    /* lock the mutex associated with minimum_value and
    update the variable as required */
    pthread_mutex_lock(&minimum_value_lock);
    if(my_min < minimum_value)
       minimum_value = my_min;
    /* and unlock the mutex */
    pthread_mutex_unlock(&minimum_value_lock);
    pthread_exit(0);
}