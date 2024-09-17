#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // Cast the thread_param to thread_data
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    // Initialize thread_complete_success to false
    thread_func_args->thread_complete_success = false;

    // Wait before obtaining mutex
    usleep(thread_func_args->wait_to_obtain_ms * 1000);

    // Obtain mutex
    if(pthread_mutex_lock(thread_func_args->mutex) != 0)
    {
        ERROR_LOG("Failed to lock mutex");
        pthread_exit(thread_param);
    }

    // Mutex obtained, wait before releasr
    usleep(thread_func_args->wait_to_release_ms * 1000);

    // Release mutex
    if(pthread_mutex_unlock(thread_func_args->mutex) != 0)
    {
        ERROR_LOG("Failed to unlock mutex");
        pthread_exit(thread_param);
    }

    // Set thread_complete_success to true
    thread_func_args->thread_complete_success = true;

    // Return thread_param data structure
    pthread_exit(thread_param);
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data* td = (struct thread_data*)malloc(sizeof(struct thread_data));
    // TODO: Verify malloc was successful

    td->mutex = mutex;
    td->wait_to_obtain_ms = wait_to_obtain_ms;
    td->wait_to_release_ms = wait_to_release_ms;

    int ret = pthread_create(thread, NULL, threadfunc, td);
    // TODO: Verify thread creation successful
    if (ret != 0){
        free(td);
    }


    return true;
}

