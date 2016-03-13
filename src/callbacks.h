//---------------------------------------------------------------------
//  Unit Circle
//---------------------------------------------------------------------
//
//  Copyright 2006 Scott Robert Ladd
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the
//      Free Software Foundation, Inc.
//      59 Temple Place - Suite 330
//      Boston, MA 02111-1307, USA.
//
//-----------------------------------------------------------------------
//
//  For more information on this software package, please visit
//  Scott's web site, Coyote Gulch Productions, at:
//
//      http://www.coyotegulch.com
//
//-----------------------------------------------------------------------

#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>


void
on_revert_button_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_close_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_main_window_realize                 (GtkWidget       *widget,
                                        gpointer         user_data);

gboolean
on_main_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_main_window_destroy_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_figure_draw_background              (GnomeCanvas     *gnomecanvas,
                                        GdkDrawable     *arg1,
                                        gint             arg2,
                                        gint             arg3,
                                        gint             arg4,
                                        gint             arg5,
                                        gpointer         user_data);

void
on_figure_render_background            (GnomeCanvas     *gnomecanvas,
                                        gpointer         arg1,
                                        gpointer         user_data);

gboolean
on_figure_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_figure_drag_motion                  (GtkWidget       *widget,
                                        GdkDragContext  *drag_context,
                                        gint             x,
                                        gint             y,
                                        guint            time,
                                        gpointer         user_data);

gboolean
on_figure_expose_event                 (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

void
on_figure_drag_begin                   (GtkWidget       *widget,
                                        GdkDragContext  *drag_context,
                                        gpointer         user_data);

void
on_figure_drag_end                     (GtkWidget       *widget,
                                        GdkDragContext  *drag_context,
                                        gpointer         user_data);

gboolean
on_figure_motion_notify_event          (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data);

void
on_angle_deg_entry_changed             (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_coord_x_entry_changed               (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_coord_y_entry_changed               (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_angle_rad_entry_changed             (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_trig_tan_entry_changed              (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_trig_cos_entry_changed              (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_trig_sin_entry_changed              (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_about_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_close_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_figure_drag_motion                  (GtkWidget       *widget,
                                        GdkDragContext  *drag_context,
                                        gint             x,
                                        gint             y,
                                        guint            time,
                                        gpointer         user_data);

gboolean
on_figure_events_event                 (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_figure_expose_event                 (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

void
on_change_deg_button_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_change_rad_button_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_change_sin_button_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_change_cos_button_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_change_tan_button_clicked           (GtkButton       *button,
                                        gpointer         user_data);
