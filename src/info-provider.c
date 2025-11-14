#include <glib.h>
#include <pthread.h>
#include <nautilus-extension.h>
#include <string.h>

#include "avinfo-object.h"
#include "column-provider.h"

#include "info-provider.h"


void
avinfo_info_provider_interface_init(NautilusInfoProviderInterface *interface)
{
    interface->update_file_info = avinfo_info_provider_update_file_info;
    interface->cancel_update = avinfo_info_provider_cancel_update;
}


NautilusOperationResult
avinfo_info_provider_update_file_info(NautilusInfoProvider *provider,
                                      NautilusFileInfo *file_info,
                                      GClosure *update_complete,
                                      NautilusOperationHandle **handle)
{    
    AVInfoExtension *object = ((AVInfoExtension *)provider);

    g_autofree char *mime_type = nautilus_file_info_get_mime_type(file_info);
    if (strncmp(mime_type, "audio/", 6) != 0 && strncmp(mime_type, "video/", 6) != 0) {
        return NAUTILUS_OPERATION_COMPLETE;
    }

    struct UpdateFileInfoHandle *update_handle = g_new0(struct UpdateFileInfoHandle, 1);
    update_handle->provider = provider;
    update_handle->file = g_object_ref(file_info);
    update_handle->update_complete = g_closure_ref(update_complete);
    *handle = (NautilusOperationHandle *)update_handle;

    pthread_mutex_lock(object->lk);
    g_queue_push_tail(object->queue, (gpointer)update_handle);
    pthread_cond_signal(object->cv);
    pthread_mutex_unlock(object->lk);
    return NAUTILUS_OPERATION_IN_PROGRESS;
}

void
avinfo_info_provider_cancel_update(NautilusInfoProvider *provider,
                                   NautilusOperationHandle *handle)
{
    AVInfoExtension *object = ((AVInfoExtension *)provider);
    struct UpdateFileInfoHandle *update_handle = (struct UpdateFileInfoHandle *)handle;

    pthread_mutex_lock(object->lk);
    update_handle->cancelled = TRUE;
    pthread_mutex_unlock(object->lk);
}

