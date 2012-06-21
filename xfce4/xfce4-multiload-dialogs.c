/*  $Id$
 *
 *  Copyright (C) 2012 nandhp <nandhp@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>

#include <libxfce4ui/libxfce4ui.h>
#include <libxfce4panel/xfce-panel-plugin.h>

#include "multiload.h"
#include "properties.h"
#include "xfce4-multiload-plugin.h"
#include "xfce4-multiload-dialogs.h"

/* the website url */
#define PLUGIN_WEBSITE "http://goodies.xfce.org/projects/panel-plugins/xfce4-multiload-plugin"

static void
multiload_configure_response (GtkWidget           *dialog,
                              gint                 response,
                              MultiloadXfcePlugin *multiload)
{
  gboolean result;

  if (response == GTK_RESPONSE_HELP)
    {
      /* show help */
      result = g_spawn_command_line_async ("exo-open --launch WebBrowser " PLUGIN_WEBSITE, NULL);

      if (G_UNLIKELY (result == FALSE))
        g_warning (_("Unable to open the following url: %s"), PLUGIN_WEBSITE);
    }
  else
    {
      /* remove the dialog data from the plugin */
      g_object_set_data (G_OBJECT (multiload->plugin), "dialog", NULL);

      /* unlock the panel menu */
      xfce_panel_plugin_unblock_menu (multiload->plugin);

      /* save the plugin */
      multiload_save (multiload->plugin, &multiload->ma);

      /* destroy the properties dialog */
      gtk_widget_destroy (dialog);
    }
}

/* Lookup the MultiloadPlugin object from the preferences dialog. */
/* Called from multiload/properties.c */
MultiloadPlugin *
multiload_configure_get_plugin (GtkWidget *widget)
{
  GtkWidget *toplevel = gtk_widget_get_toplevel (widget);
  MultiloadPlugin *ma = NULL;
  if ( gtk_widget_is_toplevel (toplevel) )
    ma = g_object_get_data(G_OBJECT(toplevel), "MultiloadPlugin");
  else      
    g_assert_not_reached ();
  g_assert( ma != NULL);
  return ma;
}

void
multiload_configure (XfcePanelPlugin     *plugin,
                     MultiloadXfcePlugin *multiload)
{
  GtkWidget *dialog;

  /* block the plugin menu */
  xfce_panel_plugin_block_menu (plugin);

  /* create the dialog */
  dialog = xfce_titled_dialog_new_with_buttons
      (_("Multiload Plugin"),
       GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (plugin))),
       GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
       GTK_STOCK_HELP, GTK_RESPONSE_HELP,
       GTK_STOCK_CLOSE, GTK_RESPONSE_OK,
       NULL);

  /* center dialog on the screen */
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);

  /* set dialog icon */
  gtk_window_set_icon_name (GTK_WINDOW (dialog), "xfce4-settings");

  /* link the dialog to the plugin, so we can destroy it when the plugin
   * is closed, but the dialog is still open */
  g_object_set_data (G_OBJECT (plugin), "dialog", dialog);
  g_object_set_data (G_OBJECT (dialog), "MultiloadPlugin", &multiload->ma);

  /* Initialize dialog widgets */
  //gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_CLOSE);
  //gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
  //gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
  //gtk_box_set_spacing (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))), 2);
  multiload_init_preferences(dialog, &multiload->ma);
	
  //g_signal_connect(G_OBJECT(dialog), "response",
  //		   G_CALLBACK(properties_close_cb), ma);

  /* connect the reponse signal to the dialog */
  g_signal_connect (G_OBJECT (dialog), "response",
                    G_CALLBACK(multiload_configure_response), multiload);

  /* show the entire dialog */
  gtk_widget_show_all (dialog);
}

void
multiload_about (XfcePanelPlugin *plugin)
{
  const gchar license[] = 
    "This program is free software; you can redistribute it and/or\n"
    "modify it under the terms of the GNU General Public License as\n"
    "published by the Free Software Foundation; either version 2 of\n"
    "the License, or (at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program; if not, write to the Free Software\n"
    "Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA\n";

  const gchar * const authors[] = {
    "nandhp <nandhp@gmail.com>",
    "Martin Baulig <martin@home-of-linux.org>",
    "Todd Kulesza <fflewddur@dropline.net>",
    "Benoît Dejean <TazForEver@dlfp.org>",
    "Davyd Madeley <davyd@madeley.id.au>",
    NULL
  };

  gtk_show_about_dialog(NULL,
      //"logo",         icon,
      //"logo-icon-name", "utilities-system-monitor",
      "program-name", PACKAGE_NAME,
      "version",      PACKAGE_VERSION,
      "comments",     _("A system load monitor capable of displaying graphs "
                        "for CPU, RAM, and swap space use, plus network "
                        "traffic."),
      "website",      PLUGIN_WEBSITE,
      "copyright",    _("Copyright \xC2\xA9 1999-2005 nandhp, FSF, and others\n"),
      "license",      license,
      "authors",      authors,
      //"documenters",  documenters,
      "translator-credits", _("translator-credits"),
      NULL);
}
