#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <pthread.h> 
#include <stdlib.h>
#include "file_searcher.h"
#define MAX_THREADS_COUNT 8
#define ROOT "/"
static volatile sig_atomic_t threads_count = 0;
static volatile sig_atomic_t thread_exit_flag = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct thread_data_t {
    char *dirname;
    char *filename;
} thread_data_t;

static void dir_tree_search(const char *dirname, const char *filename)
{
    char path[PATH_MAX];
    DIR *dir;
    struct dirent entry;
    struct dirent *result = NULL;

    if (!(dir = opendir(dirname)))
        return;
    while (readdir_r(dir, &entry, &result) == 0 && result && !thread_exit_flag)
    {
        if (entry.d_type == DT_DIR) /* if this is not a directory */
        {
            if (strcmp(entry.d_name, ".") == 0 || strcmp(entry.d_name, "..") == 0) 
            {  
                continue;
            }
            snprintf(path, PATH_MAX, "%s/%s", dirname, entry.d_name);
            dir_tree_search(path, filename);
        }
        else
        {
           if (strncmp(filename, entry.d_name, FILENAME_MAX) == 0)
            {
                thread_exit_flag = 1;
                //pthread_mutex_lock(&mutex);
                printf("File has found!\n");
                printf("Full Path to the file = %s/%s\n", path, entry.d_name);
                 pthread_mutex_lock(&mutex);
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
    }
    closedir(dir);
}

static void *thread_function(void *param)
{
    const char *dirname = ((thread_data_t *)param)->dirname;
    const char *filename = ((thread_data_t *)param)->filename;

    if (!thread_exit_flag)
    {
        dir_tree_search(dirname, filename);
    }
    free(param);
    --threads_count;
    pthread_exit(0);
}

int search_file(char *filename)
{
    char path[PATH_MAX];
    pthread_t thread;
    thread_data_t *thread_data = NULL;
    DIR *dir;
    struct dirent entry;
    struct dirent *result = NULL;

    if (!(dir = opendir(ROOT)))
    {
        perror("Error open root directory");
        return -1;
    }
    while (1)
    {
        if (threads_count < MAX_THREADS_COUNT)
        {
            if (readdir_r(dir, &entry, &result) != 0 || !result)
            {
                while (threads_count); /* waiting for close all threads */
                break;
            }
            else if (entry.d_type == DT_DIR) /* if that is a directory */
            {
                if (!strcmp(entry.d_name, ".") || !strcmp(entry.d_name, ".."))
                    continue;
                snprintf(path, PATH_MAX, "/%s", entry.d_name);
                thread_data = (thread_data_t *)malloc(sizeof(thread_data_t));
                thread_data->dirname = path;
                thread_data->filename = filename;
                if (pthread_create(&thread, NULL, thread_function,
                    thread_data))
                {
                    perror("Error pthread_create");
                    thread_data = NULL;
                    free(thread_data);
                    continue;
                }
                else
                {
                    pthread_detach(thread);
                    ++threads_count;
                }
            }
        }
    }
    while (threads_count); /* waiting for close all threads*/
    if (!thread_exit_flag)
        printf("File not found\n");
    
    return 0;
}