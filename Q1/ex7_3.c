/*
    Example 7.3 from Introduction to Parallel Computing, Grama et al.
    additional code added by Jacob Zahn
*/
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define INT_MAX 2147483647

void *find_min(void *list_ptr);
pthread_mutex_t minimum_value_lock;
int minimum_value, partial_list_size;

int main(int argc, char *argv[]){
    /* declare and initialize data structures and list*/
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
        int randArray[1000000];
        srand(time(NULL));
        for(int i = 0; i < 1000000; i++){
            randArray[i] = rand();
        }
        int split = (int) 1000000/numP;
        
        for(int i = 0; i < numP; i++){
            list_ptrs[i] = &randArray[(split*i)];
            pthread_create(&finders[i],NULL,find_min,list_ptrs[i]);
        }
        for(int i = 0; i < numP; i++){
            pthread_join(finders[i],0);
        }
    }
    return minimum_value;

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