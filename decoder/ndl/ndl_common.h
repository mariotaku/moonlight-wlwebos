#pragma once
#include <NDL_directmedia.h>

#include "stream/module/api.h"

#define media_initialized PLUGIN_SYMBOL_NAME(decoder_media_initialized)
#define media_loaded PLUGIN_SYMBOL_NAME(decoder_media_loaded)
#define media_info PLUGIN_SYMBOL_NAME(decoder_media_info)
#define media_reload PLUGIN_SYMBOL_NAME(decoder_media_reload)
#define media_unload PLUGIN_SYMBOL_NAME(decoder_media_unload)

extern bool media_initialized;
#if NDL_WEBOS5
extern bool media_loaded;
extern NDL_DIRECTMEDIA_DATA_INFO media_info;
int media_reload();
void media_unload();
#endif