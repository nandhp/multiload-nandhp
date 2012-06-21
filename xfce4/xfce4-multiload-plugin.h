#ifndef __XFCE4_MULTILOAD_H__
#define __XFCE4_MULTILOAD_H__

#include "multiload.h"
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>

G_BEGIN_DECLS

typedef struct _MultiloadXfcePlugin MultiloadXfcePlugin;

struct _MultiloadXfcePlugin
{
    MultiloadPlugin ma;

    /* panel widgets */
    XfcePanelPlugin *plugin;
    GtkWidget       *ebox;
};

void
multiload_save (XfcePanelPlugin *plugin,
                MultiloadPlugin *ma);

/* show properties dialog */
G_GNUC_INTERNAL void
multiload_properties_cb (GtkAction       *action,
			 MultiloadPlugin *ma);

G_END_DECLS

#endif
