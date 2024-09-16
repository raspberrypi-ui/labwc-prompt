/*
Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#define SUDO_PREFIX "SUDO_ASKPASS=/usr/lib/labwc-prompt/pwdlpr.sh sudo -A "

typedef enum {
    WM_OPENBOX,
    WM_WAYFIRE,
    WM_LABWC } wm_type;
static wm_type wm;

static GtkWidget *main_dlg, *switch_btn, *keep_btn, *later_btn;


static void remove_autostart (void)
{
    system (SUDO_PREFIX "rm -f /etc/xdg/autostart/labwc-prompt.desktop");
}

static void do_switch (GtkButton *button, gpointer data)
{
    system (SUDO_PREFIX "raspi-config nonint do_wayland W3");
    remove_autostart ();
    system ("sync;reboot");
}

static void do_keep (GtkButton *button, gpointer data)
{
    remove_autostart ();
    gtk_main_quit ();
}

static void do_later (GtkButton *button, gpointer data)
{
    gtk_main_quit ();
}

static gboolean close_prog (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit ();
    return TRUE;
}

/* The dialog... */

int main (int argc, char *argv[])
{
    GtkBuilder *builder;

#ifdef ENABLE_NLS
    setlocale (LC_ALL, "");
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif

    if (getenv ("WAYLAND_DISPLAY"))
    {
        if (getenv ("WAYFIRE_CONFIG_FILE")) wm = WM_WAYFIRE;
        else wm = WM_LABWC;
    }
    else wm = WM_OPENBOX;

    if (wm == WM_LABWC)
    {
        remove_autostart ();
        exit (0);
    }

    // GTK setup
    gtk_init (&argc, &argv);
    gtk_icon_theme_prepend_search_path (gtk_icon_theme_get_default(), PACKAGE_DATA_DIR);

    // build the UI
    builder = gtk_builder_new_from_file (PACKAGE_DATA_DIR "/labwc-prompt.ui");

    main_dlg = (GtkWidget *) gtk_builder_get_object (builder, "main_window");
    g_signal_connect (main_dlg, "delete_event", G_CALLBACK (close_prog), NULL);
    
    switch_btn = (GtkWidget *) gtk_builder_get_object (builder, "switch_btn");
    g_signal_connect (switch_btn, "clicked", G_CALLBACK (do_switch), NULL);

    later_btn = (GtkWidget *) gtk_builder_get_object (builder, "later_btn");
    g_signal_connect (later_btn, "clicked", G_CALLBACK (do_later), NULL);

    if (wm == WM_OPENBOX)
    {
        gtk_widget_hide (GTK_WIDGET (gtk_builder_get_object (builder, "wayfire_msg")));
        gtk_widget_hide (GTK_WIDGET (gtk_builder_get_object (builder, "keep_w_btn")));
        keep_btn = (GtkWidget *) gtk_builder_get_object (builder, "keep_x_btn");
    }
    else
    {
        gtk_widget_hide (GTK_WIDGET (gtk_builder_get_object (builder, "x_msg")));
        gtk_widget_hide (GTK_WIDGET (gtk_builder_get_object (builder, "keep_x_btn")));
        keep_btn = (GtkWidget *) gtk_builder_get_object (builder, "keep_w_btn");
    }

    g_signal_connect (keep_btn, "clicked", G_CALLBACK (do_keep), NULL);

    g_object_unref (builder);

    gtk_widget_show (main_dlg);
    gtk_window_set_default_size (GTK_WINDOW (main_dlg), 1, 1);
    gtk_main ();

    return 0;
}


