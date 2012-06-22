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
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "multiload.h"
#include "xfce4-multiload-plugin.h"
#include "xfce4-multiload-dialogs.h"

void
multiload_save (XfcePanelPlugin *plugin,
                MultiloadPlugin *ma)
{
  XfceRc *rc;
  gchar *file;
  guint i;

  /* get the config file location */
  file = xfce_panel_plugin_save_location (plugin, TRUE);

  if (G_UNLIKELY (file == NULL))
    {
       DBG ("Failed to open config file");
       return;
    }

  /* open the config file, read/write */
  rc = xfce_rc_simple_open (file, FALSE);
  g_free (file);

  if (G_LIKELY (rc != NULL))
    {
      /* save the settings */
      DBG(".");

      /* Write size and speed */
      xfce_rc_write_int_entry (rc, "speed", ma->speed);
      xfce_rc_write_int_entry (rc, "size", ma->size);
      
      for ( i = 0; i < NGRAPHS; i++ )
        { 
          char *key;
          char list[8*MAX_COLORS], *listpos = list;
          guint ncolors = graph_types[i].num_colors, j;
          GdkColor *colors = ma->graph_config[i].colors;

          /* Visibility */
          key = g_strdup_printf("%s_visible", graph_types[i].name);
          xfce_rc_write_bool_entry (rc, key, ma->graph_config[i].visible);
          g_free (key);

          /* Create color list */
          for ( j = 0; j < ncolors; j++ )
            {
              multiload_gdk_color_stringify(&colors[j], listpos);
              if ( j == ncolors-1 )
                listpos[7] = 0;
              else
                listpos[7] = ',';
              listpos += 8;
            }
          g_assert (strlen(list) == 8*graph_types[i].num_colors-1);

          /* Save colors */
          key = g_strdup_printf("%s_colors", graph_types[i].name);
          xfce_rc_write_entry (rc, key, list);
          g_free (key);
        }

      /* close the rc file */
      xfce_rc_close (rc);
    }
}

void
multiload_read (XfcePanelPlugin *plugin,
                MultiloadPlugin *ma)
{
  XfceRc *rc;
  gchar *file;
  guint i;

  /* get the plugin config file location */
  file = xfce_panel_plugin_lookup_rc_file (plugin);

  if (G_LIKELY (file != NULL))
    {
      /* open the config file, readonly */
      rc = xfce_rc_simple_open (file, TRUE);

      /* cleanup */
      g_free (file);

      if (G_LIKELY (rc != NULL))
        {
          /* Read speed and size */
          ma->speed = xfce_rc_read_int_entry(rc, "speed", DEFAULT_SPEED);
          ma->size = xfce_rc_read_int_entry(rc, "size", DEFAULT_SIZE);

          /* Read visibility and colors for each graph */
          for ( i = 0; i < NGRAPHS; i++ )
            { 
              char *key;
              const char *list, *listpos;
              guint ncolors = graph_types[i].num_colors, j;
              GdkColor *colors = ma->graph_config[i].colors;

              /* Visibility */
              key = g_strdup_printf("%s_visible", graph_types[i].name);
              ma->graph_config[i].visible =
                  xfce_rc_read_bool_entry(rc, key, i == 0 ? TRUE : FALSE);
              g_free (key);

              /* Colors - Try to load from xfconf */
              key = g_strdup_printf("%s_colors", graph_types[i].name);
              list = xfce_rc_read_entry (rc, key, NULL);
              g_free (key);

              /* Load the user's preferred colors */
              listpos = list;
              if ( G_LIKELY (listpos) )
                for ( j = 0; j < ncolors && listpos != NULL; j++ )
                  {
                    /* Check the length of the list item. */
                    int pos = 0;
                    if ( j == ncolors-1 )
                      pos = strlen(listpos);
                    else
                      pos = (int)(strchr(listpos, ',')-listpos);

                    /* Try to parse the color */
                    if ( G_LIKELY (pos == 7) )
                      {
                        /* Extract the color into a null-terminated buffer */
                        char buf[8];
                        strncpy(buf, listpos, 7);
                        buf[7] = 0;
                        if ( gdk_color_parse(buf, &colors[j]) == TRUE )
                          listpos += 8;
                        else
                          listpos = NULL;
                      }
                    else
                      listpos = NULL;
                  }

              /* Use the default colors if read failed. */
              if ( !listpos )
                for ( j = 0; j < ncolors; j++ )
                  gdk_color_parse(graph_types[i].colors[j].default_value,
                                  &colors[j]);
            }

          /* cleanup */
          xfce_rc_close (rc);

          /* leave the function, everything went well */
          return;
        }
    }

  /* something went wrong, apply default values */
  DBG ("Applying default settings");

  ma->speed = DEFAULT_SPEED;
  ma->size = DEFAULT_SIZE;
  for ( i = 0; i < NGRAPHS; i++ )
    { 
      guint ncolors = graph_types[i].num_colors, j;
      /* Default visibility */
      ma->graph_config[i].visible = i == 0 ? TRUE : FALSE;
      /* Default colors */
      for ( j = 0; j < ncolors; j++ )
        gdk_color_parse(graph_types[i].colors[j].default_value,
                        &ma->graph_config[i].colors[j]);
    }
}
