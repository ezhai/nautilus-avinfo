#include <pthread.h>

#include "column-provider.h"
#include "info-provider.h"
#include "properties-provider.h"

#include "avinfo-object.h"

static GType avinfo_extension_type;

GType
avinfo_extension_get_type(void) {
    return avinfo_extension_type;
}

void
avinfo_extension_instance_init(AVInfoExtension *object)
{
    AVInfoExtensionClass *class = (AVInfoExtensionClass*)(((GTypeInstance*)object)->g_class);
    object->lk = class->lk;
    object->cv = class->cv;
    object->queue = class->queue;
}

void
avinfo_extension_class_init(AVInfoExtensionClass *class,
                            gpointer class_data)
{
    AVInfoExtensionClassData *data = (AVInfoExtensionClassData *)class_data;
    class->lk = data->lk;
    class->cv = data->cv;
    class->queue = data->queue;
}

void
avinfo_extension_register_type(GTypeModule *module, AVInfoExtensionClassData *data)
{
    g_autofree GTypeInfo *info = NULL;
    g_autofree GInterfaceInfo *column_provider_interface_info = NULL;
    g_autofree GInterfaceInfo *info_provider_interface_info = NULL;
    g_autofree GInterfaceInfo *properties_provider_interface_info = NULL;

    info = g_new0(GTypeInfo, 1);
    info->class_size = sizeof (AVInfoExtensionClass);
    info->class_init = (GClassInitFunc)avinfo_extension_class_init;
    info->class_data = data;
    info->instance_size = sizeof (AVInfoExtension);
    info->instance_init = (GInstanceInitFunc)avinfo_extension_instance_init;

	avinfo_extension_type = g_type_module_register_type(module,
                                                        G_TYPE_OBJECT,
                                                        "AVInfoExtension",
                                                        info, 0);
    
    column_provider_interface_info = g_new0(GInterfaceInfo, 1);
    column_provider_interface_info->interface_init = (GInterfaceInitFunc)avinfo_extension_column_provider_interface_init;

    info_provider_interface_info = g_new0(GInterfaceInfo, 1);
    info_provider_interface_info->interface_init = (GInterfaceInitFunc)avinfo_extension_info_provider_interface_init;

    properties_provider_interface_info = g_new0(GInterfaceInfo, 1);
    properties_provider_interface_info->interface_init = (GInterfaceInitFunc)avinfo_extension_properties_model_provider_interface_init;
    
    g_type_module_add_interface(module, avinfo_extension_type, 
                                NAUTILUS_TYPE_COLUMN_PROVIDER,
                                column_provider_interface_info);
    g_type_module_add_interface(module, avinfo_extension_type, 
                                NAUTILUS_TYPE_INFO_PROVIDER,
                                info_provider_interface_info);
    g_type_module_add_interface(module, avinfo_extension_type,
                                NAUTILUS_TYPE_PROPERTIES_MODEL_PROVIDER,
                                properties_provider_interface_info);
}
