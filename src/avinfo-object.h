#pragma once

#include <glib.h>
#include <glib-object.h>

typedef struct _AVInfoExtension AVInfoExtension;
typedef struct _AVInfoExtensionClass AVInfoExtensionClass;
typedef struct _AVInfoExtensionClassData AVInfoExtensionClassData;

struct _AVInfoExtension {
    GObject parent;
    pthread_mutex_t *lk;
    pthread_cond_t *cv;
    GQueue *queue;
};

struct _AVInfoExtensionClass {
    GObjectClass parent;
    pthread_mutex_t *lk;
    pthread_cond_t *cv;
    GQueue *queue;
};

struct _AVInfoExtensionClassData {
    pthread_mutex_t *lk;
    pthread_cond_t *cv;
    GQueue *queue;
};

void avinfo_extension_instance_init(AVInfoExtension *object);

void avinfo_extension_class_init(AVInfoExtensionClass *class,
                                 gpointer class_data);

void avinfo_extension_register_type(GTypeModule *module, 
                                    AVInfoExtensionClassData *data);

GType avinfo_extension_get_type(void);
