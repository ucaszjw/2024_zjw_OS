#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define THREADS 4 // Number of threads
int array[1000000];
struct thread_data_t
{
    int *array;
    int id;
}; // Structure to pass data to threads

// Function to assign values to array
void* thread_assign(void* arg){
    struct thread_data_t *data = (struct thread_data_t*)arg; // Get data from argument
    int *array = data->array; // Get array from data
    int id = data->id; // Get id from data to calculate start and end index of current thread
    int start = id * 1000000 / THREADS; // Calculate start index
    int end = (id + 1) * 1000000 / THREADS; // Calculate end index
    for (int i = start; i < end; i++)
        array[i] = i;
    return NULL;
}

int main(){
    struct thread_data_t thread_data[THREADS]; // Array of thread data to pass to threads
    pthread_t threads[THREADS]; // Array of threads to store thread ids
    for (int i = 0; i < THREADS; i++){
        thread_data[i].array = array;
        thread_data[i].id = i;
        pthread_create(&threads[i], NULL, thread_assign, &thread_data[i]); // Create threads and pass data to them
    }
    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL); // Wait for threads to finish

    int sum = 0;
    for (int i = 0; i < 1000000; i++)
        sum += array[i];
    printf("Sum: %d\n", sum);
    return 0;
}