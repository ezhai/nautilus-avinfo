#pragma once

#include <glib.h>
#include <nautilus-extension.h>


void avinfo_properties_model_provider_interface_init(NautilusPropertiesModelProviderInterface *interface);

GList *avinfo_properties_model_provider_get_models(NautilusPropertiesModelProvider *provider,
                                                   GList *files);
