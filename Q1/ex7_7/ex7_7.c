/*
    Example 7.7 from Introduction to Parallel Computing, Grama et al.
    with code added from sec 7.8.1
*/
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>

#define INT_MAX 2147483647

void *find_min_rw(void *list_ptr);
pthread_mutex_t minimum_value_lock;
int minimum_value, partial_list_size;

typedef struct {
    int readers;
    int writer;
    pthread_cond_t readers_proceed;
    pthread_cond_t writer_proceed;
    int pending_writers;
    pthread_mutex_t read_write_lock; 
} mylib_rwlock_t;

mylib_rwlock_t read_write_lock;

void mylib_rwlock_init (mylib_rwlock_t *l) {
    l -> readers = l -> writer = l -> pending_writers = 0;
    pthread_mutex_init(&(l -> read_write_lock), NULL);
    pthread_cond_init(&(l -> readers_proceed), NULL);
    pthread_cond_init(&(l -> writer_proceed), NULL);
}

void mylib_rwlock_rlock(mylib_rwlock_t *l) {
    /* if there is a write lock or pending writers, perform condition
    wait.. else increment count of readers and grant read lock */
    
    pthread_mutex_lock(&(l -> read_write_lock));
    while ((l -> pending_writers > 0) || (l -> writer > 0))
        pthread_cond_wait(&(l -> readers_proceed),
            &(l -> read_write_lock));
    l -> readers ++;
    pthread_mutex_unlock(&(l -> read_write_lock));
}

void mylib_rwlock_wlock(mylib_rwlock_t *l) {
    /* if there are readers or writers, increment pending writers
    count and wait. On being woken, decrement pending writers
    count and increment writer count */ 

    pthread_mutex_lock(&(l -> read_write_lock));
    while ((l -> writer > 0) || (l -> readers > 0)) { 
        l -> pending_writers ++;
        pthread_cond_wait(&(l -> writer_proceed),
            &(l -> read_write_lock));
    }
    l -> pending_writers --;
    l -> writer ++;
     pthread_mutex_unlock(&(l -> read_write_lock));
}

void mylib_rwlock_unlock(mylib_rwlock_t *l) {
    /* if there is a write lock then unlock, else if there are
    read locks, decrement count of read locks. If the count
    is 0 and there is a pending writer, let it through, else
    if there are pending readers, let them all go through */
    
    pthread_mutex_lock(&(l -> read_write_lock));
    if (l -> writer > 0)
        l -> writer = 0;
    else if (l -> readers > 0)
        l -> readers --;
    pthread_mutex_unlock(&(l -> read_write_lock));
    if ((l -> readers == 0) && (l -> pending_writers > 0))
        pthread_cond_signal(&(l -> writer_proceed));
    else if (l -> readers > 0)
        pthread_cond_broadcast(&(l -> readers_proceed)); 
} 

int main(int argc, char *argv[]){
    /* declare and initialize data structures and list*/

    /* Timing variables */
    struct timeval etstart, etstop;  /* Elapsed times using gettimeofday() */
    struct timezone tzdummy;
    clock_t etstart2, etstop2;  /* Elapsed times using times() */
    unsigned long long usecstart, usecstop;
    struct tms cputstart, cputstop;  /* CPU times for my processes */

    minimum_value = INT_MAX;
    int numP;//number of processors

    //pthread_init();
    pthread_mutex_init(&minimum_value_lock, NULL);

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
        double remainder = 100000000%numP;
        if(remainder!=0.0){//check if processors cleanly divides the problem
            return(-1);
        }
        int* randArray = (int*) malloc(100000000 * sizeof(int));//create array
        srand(time(NULL));//seed
        for(int i = 0; i < 100000000; i++){//fill array
            randArray[i] = (int)rand() / 32768;
        }
        
        int split = 100000000/numP;//get partial list size
        partial_list_size = split;
        
        /* Start Clock */
        printf("\nStarting clock.\n");
        gettimeofday(&etstart, &tzdummy);
        etstart2 = times(&cputstart);
        
        for(int i = 0; i < numP; i++){//create ptrs and threads
            list_ptrs[i] = &randArray[i*split];
            pthread_create(&finders[i],NULL,find_min_rw,list_ptrs[i]);
        }
        for(int i = 0; i < numP; i++){//join threads
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
        printf("Min: %i", minimum_value);//should be 0 by odds
        free(randArray);//release the memory
    }
    
    return 0;
}

void *find_min_rw(void *list_ptr) {
    int *partial_list_pointer, my_min, i;
    my_min = INT_MAX;
    partial_list_pointer = (int *) list_ptr;
    for (i = 0; i < partial_list_size; i++) 
        if (partial_list_pointer[i] < my_min) 
            my_min = partial_list_pointer[i];
    /* lock the mutex associated with minimum_value and 
    update the variable as required */ 
    mylib_rwlock_rlock(&read_write_lock);
    if (my_min < minimum_value) { 
        mylib_rwlock_unlock(&read_write_lock);
        mylib_rwlock_wlock(&read_write_lock);
        minimum_value = my_min;
    } 
    /* and unlock the mutex */ 
    mylib_rwlock_unlock(&read_write_lock); 
    pthread_exit(0); 
}