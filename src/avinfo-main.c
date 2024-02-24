#include <config.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <locale.h>
#include <nautilus-extension.h>
#include <pthread.h>

#include "info-update.h"
#include "avinfo-object.h"

#define NUM_THREADS 2

static pthread_t threads[NUM_THREADS];
static pthread_mutex_t lk;
static pthread_cond_t cv;
static GQueue queue;
static AVInfoExtensionClassData *data;
static struct UpdateFileInfoArgs *args;

static GType provider_types[1];


void
nautilus_module_initialize(GTypeModule *module)
{    
    bindtextdomain(PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(PACKAGE, "UTF_8");

    pthread_mutex_init(&lk, NULL);
    pthread_cond_init(&cv, NULL);
    g_queue_init(&queue);   
    
    data = g_new0(AVInfoExtensionClassData, 1);
    data->cv = &cv;
    data->lk = &lk;
    data->queue = &queue;

    avinfo_extension_register_type(module, data);
    provider_types[0] = avinfo_extension_get_type();

    args = g_new0(struct UpdateFileInfoArgs, 1);
    args->lk = &lk;
    args->cv = &cv;
    args->queue = &queue;
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, update_file_info_worker, args);
    }
}

void
nautilus_module_shutdown(void)
{
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_detach(threads[i]);
    }
    pthread_mutex_destroy(&lk);
    pthread_cond_destroy(&cv);
    g_queue_free(&queue);
    g_free(data);
    g_free(args);
}

void
nautilus_module_list_types(const GType **types,
                           int *num_types)
{
    *types = provider_types;
    *num_types = G_N_ELEMENTS(provider_types);
}
