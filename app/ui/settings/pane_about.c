#include "window.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if TARGET_WEBOS
#include "platform/webos/os_info.h"
#if HAVE_SDL
#include <SDL_webOS.h>
#endif
#endif

#include "stream/platform.h"

#if TARGET_WEBOS
static char webos_release[32];
static struct
{
    int w, h;
    int rate;
} webos_panel_info;
#endif

static void load_webos_info();

static bool _render(struct nk_context *ctx, bool *showing_combo)
{
    static struct nk_rect item_bounds = {0, 0, 0, 0};
    int item_index = 0;

    nk_layout_row_template_begin_s(ctx, 25);
    nk_layout_row_template_push_static_s(ctx, 120);
    nk_layout_row_template_push_variable_s(ctx, 1);
    nk_layout_row_template_end(ctx);
    nk_label(ctx, "Version", NK_TEXT_LEFT);
    nk_label(ctx, APP_VERSION, NK_TEXT_RIGHT);
    nk_label(ctx, "Decoder Module", NK_TEXT_LEFT);
    nk_label(ctx, decoder_definitions[decoder_current].name, NK_TEXT_RIGHT);
    nk_label(ctx, "Audio Backend", NK_TEXT_LEFT);
    if (audio_current == AUDIO_DECODER)
        nk_label(ctx, "Use decoder", NK_TEXT_RIGHT);
    else if (audio_current >= 0)
        nk_label(ctx, audio_definitions[audio_current].name, NK_TEXT_RIGHT);

#if TARGET_WEBOS
    nk_label(ctx, "webOS version", NK_TEXT_LEFT);
    nk_label(ctx, webos_release, NK_TEXT_RIGHT);
#if HAVE_SDL
    if (webos_panel_info.w && webos_panel_info.h)
    {
        nk_label(ctx, "Panel resolution", NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_RIGHT, "%d * %d", webos_panel_info.w, webos_panel_info.h);
    }
    if (webos_panel_info.rate)
    {
        nk_label(ctx, "Refresh rate", NK_TEXT_LEFT);
        nk_labelf(ctx, NK_TEXT_RIGHT, "%d", webos_panel_info.rate);
    }
#endif
#endif
    return true;
}

static int _itemcount()
{
    return 0;
}

static void _onselect()
{
#if TARGET_WEBOS
    load_webos_info();
#endif
}

#if TARGET_WEBOS
void load_webos_info()
{
    webos_os_info_get_release(webos_release, sizeof(webos_release));
#if HAVE_SDL
    SDL_webOSGetPanelResolution(&webos_panel_info.w, &webos_panel_info.h);
    SDL_webOSGetRefreshRate(&webos_panel_info.rate);
#endif
}
#endif
struct settings_pane settings_pane_about = {
    .title = "About",
    .render = _render,
    .navkey = NULL,
    .itemcount = _itemcount,
    .onselect = _onselect,
};
