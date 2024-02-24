#pragma once

#include <glib.h>
#include <pthread.h>

struct UpdateFileInfoArgs{
    pthread_mutex_t *lk;
    pthread_cond_t *cv;
    GQueue *queue;
};

void *update_file_info_worker(void *data);
