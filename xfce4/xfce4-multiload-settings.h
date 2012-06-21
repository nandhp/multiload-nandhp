#ifndef __XFCE4_MULTILOAD_SETTINGS_H__
#define __XFCE4_MULTILOAD_SETTINGS_H__

#include "multiload.h"
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>

G_BEGIN_DECLS

void
multiload_save (XfcePanelPlugin *plugin,
                MultiloadPlugin *ma);

void
multiload_read (XfcePanelPlugin *plugin,
                MultiloadPlugin *ma);

G_END_DECLS

#endif
