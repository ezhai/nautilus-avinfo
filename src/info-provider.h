#pragma once

#include <glib.h>
#include <nautilus-extension.h>


struct UpdateFileInfoHandle {
    NautilusInfoProvider *provider;
    NautilusFileInfo *file;
    GClosure *update_complete;
    int cancelled;
};

void avinfo_info_provider_interface_init(NautilusInfoProviderInterface *interface);

NautilusOperationResult avinfo_info_provider_update_file_info(NautilusInfoProvider *provider,
                                                              NautilusFileInfo *file,
                                                              GClosure *update_complete,
                                                              NautilusOperationHandle **handle);

void avinfo_info_provider_cancel_update(NautilusInfoProvider *provider,
                                        NautilusOperationHandle *handle);