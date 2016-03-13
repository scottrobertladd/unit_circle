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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

static const char * ICON_FILENAME = PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps/unit_circle_icon.png";

static const double DEFAULT_ANGLE = 1.0;

static const double PIM2   = 6.28318530717958647693;
static const double PI     = 3.14159265358979323846;
static const double RAD2PI = 0.01745329251994329577;

static double angle = 1.0; // in radians

static bool changing = false;

static const char * theta_text = "\316\270"; 

static GtkWidget       * main_window = NULL;

static cairo_pattern_t * circle_fill_color;
static cairo_pattern_t * circle_line_color;
static cairo_pattern_t * axis_line_color;
static cairo_pattern_t * angle_line_color;
static cairo_pattern_t * theta_line_color;
static cairo_pattern_t * connect_line_color;
static cairo_pattern_t * angle_text_color;
static cairo_pattern_t * axis_text_color;
static cairo_pattern_t * quadrant_text_color;
    
static cairo_text_extents_t angle_text_extents;
static cairo_text_extents_t theta_text_extents;

static double theta_width  =  3.0;
static double theta_height =  6.0;
static double font_size    = 12.0;
static double quad_offset  = 40.0;
static double inset;
static double dot_line[2] = { 2.0, 2.0 };

enum field_no
{
    NO_SKIP,
    ANGLE_DEG,
    ANGLE_RAD,
    TRIG_SIN,
    TRIG_COS,
    TRIG_TAN
};
    
/* Create a path that is a circular oval with radii xr, yr centered at xc, yc */
static void draw_ellipse (cairo_t * cr, double xc, double yc, double xr, double yr, double a)
{
    cairo_translate(cr, xc, yc);
    cairo_rotate(cr, a);
    cairo_scale(cr, 1.0, yr / xr);
    cairo_move_to(cr, xr, 0.0);
    cairo_arc(cr, 0.0, 0.0, xr, 0.0, PIM2);
    cairo_stroke(cr);
}

static void figure_draw(GtkWidget * figure, cairo_t * cr)
{
    cairo_surface_t * drawing_surface;
    cairo_t * drawing_cr; 
    
    double sin_a, cos_a;
    double width, height, origin_x, origin_y, radius;
    double angle_x, angle_y, angle_text_x, angle_text_y;
    double theta_x, theta_y;
    int quadrant;
    char angle_text[32];
    
    // make certain angle is in range
    angle = fmod(angle, PIM2);
    
    // compute trig stuff once
    sin_a = sin(angle);
    cos_a = cos(angle);
    
    if (cos_a < 0.0)
    {
        if (sin_a < 0.0)
            quadrant = 3;
        else
            quadrant = 2;
    }
    else
    {
        if (sin_a < 0.0)
            quadrant = 4;
        else
            quadrant = 1;
    }
    
    // set angle text
    snprintf(angle_text,32,"(%5.3f, %5.3f)",cos_a,sin_a);
    
    // find center point
    width    = figure->allocation.x + figure->allocation.width;
    height   = figure->allocation.y + figure->allocation.height;
    origin_x = width /  2.0;
    origin_y = height / 2.0;
    
    radius = MIN (figure->allocation.width / 2.0, figure->allocation.height / 2.0) - inset;
    
    angle_x = origin_x + cos_a * radius;
    angle_y = origin_y - sin_a * radius;
    
    
    if (quadrant <= 2)
    {
        theta_x = origin_x + cos(angle / 2.0) / 2.7 * radius;
        theta_y = origin_y - sin(angle / 2.0) / 2.7 * radius;
    }
    else
    {
        theta_x = origin_x - cos(angle / 2.0) / 2.7 * radius;
        theta_y = origin_y + sin(angle / 2.0) / 2.7 * radius;
    }
    
    switch (quadrant)
    {
        case 1:
            angle_text_x = angle_x + 5.0;
            angle_text_y = angle_y - 5.0;
            break;
            
        case 2:
            angle_text_x = angle_x - 5.0 - angle_text_extents.width;
            angle_text_y = angle_y - 5.0;
            break;
            
        case 3:
            angle_text_x = angle_x - 5.0 - angle_text_extents.width;
            angle_text_y = angle_y + 5.0 + angle_text_extents.height;
            break;
            
        case 4:
            angle_text_x = angle_x + 5.0;
            angle_text_y = angle_y + 5.0 + angle_text_extents.height;
            break;
    }
    
    // create drawing surface    
    drawing_surface = cairo_surface_create_similar(cairo_get_target(cr),
                                                   CAIRO_CONTENT_COLOR,
                                                   width, height);

    drawing_cr = cairo_create(drawing_surface);
    
    cairo_set_operator(drawing_cr, CAIRO_OPERATOR_OVER);
        
    // draw the background
    cairo_set_source(drawing_cr, circle_fill_color);
    cairo_rectangle(drawing_cr,0,0,width,height);
    cairo_fill(drawing_cr);
    
    // draw filled circle 
    cairo_set_line_width(drawing_cr, 1.0);
    cairo_arc (drawing_cr, origin_x, origin_y, radius, 0, PIM2);
    cairo_set_source(drawing_cr, circle_fill_color);
    cairo_fill_preserve(drawing_cr);
    cairo_set_source(drawing_cr, circle_line_color);
    cairo_stroke(drawing_cr);
    
    // draw the x-y axes
    cairo_set_line_width(drawing_cr, 0.5);
    cairo_move_to(drawing_cr, 0.0, origin_y);
    cairo_line_to(drawing_cr, width, origin_y);
    cairo_set_source(drawing_cr, axis_line_color);
    cairo_stroke(drawing_cr);
    
    cairo_move_to(drawing_cr, origin_x, 0.0);
    cairo_line_to(drawing_cr, origin_x, height);
    cairo_set_source(drawing_cr, axis_line_color);
    cairo_stroke(drawing_cr);
    
    // draw lines from point to axes
    cairo_save(drawing_cr);
    
    cairo_set_line_width(drawing_cr, 1.0);
    cairo_set_dash(drawing_cr, dot_line, 2, 0.0);
    cairo_move_to(drawing_cr, angle_x, angle_y);
    cairo_line_to(drawing_cr, angle_x, origin_y);
    cairo_set_source(drawing_cr, connect_line_color);
    cairo_stroke(drawing_cr);
                
    cairo_move_to(drawing_cr, angle_x, angle_y);
    cairo_line_to(drawing_cr, origin_x, angle_y);
    cairo_set_source(drawing_cr, connect_line_color);
    cairo_stroke(drawing_cr);
    
    // draw theta arc 
    cairo_set_line_width(drawing_cr, 1.0);
    cairo_arc (drawing_cr, origin_x, origin_y, radius / 4.0, -angle, 0);
    cairo_set_source(drawing_cr, theta_line_color);
    cairo_stroke(drawing_cr);
    
    cairo_restore(drawing_cr);
    
    // draw (literally) theta symbol
    //      note: Cairo's "toy" text API does not actually support true
    //            UTF-8 text, since it only uses a single non-Unicode font
    //            for glyphs. Rather than involve Pango in the drawing of
    //            a single glyph, I draw the glyph manually,
    cairo_save(drawing_cr);
    cairo_set_source(drawing_cr, angle_text_color);
    cairo_set_line_width(drawing_cr, 1.0);
    cairo_move_to(drawing_cr, theta_x - theta_width, theta_y);
    cairo_line_to(drawing_cr, theta_x + theta_width, theta_y);
    cairo_stroke(drawing_cr);
    cairo_set_line_width(drawing_cr, 0.5);
    draw_ellipse(drawing_cr, theta_x, theta_y, theta_width, theta_height, 0.2);
    cairo_restore(drawing_cr);
    
    
    // draw the angle line
    cairo_set_line_width(drawing_cr, 2.0);
    cairo_move_to(drawing_cr, origin_x, origin_y);
    cairo_line_to(drawing_cr, angle_x, angle_y);
    cairo_set_source(drawing_cr, angle_line_color);
    cairo_stroke(drawing_cr);
    cairo_arc (drawing_cr, angle_x, angle_y, 4.0, 0, PIM2);
    cairo_fill(drawing_cr);
    
    // draw coordinate text
    cairo_move_to(drawing_cr, angle_text_x, angle_text_y);
    cairo_set_source(drawing_cr, angle_text_color);
    cairo_set_font_size(drawing_cr, font_size);
    cairo_show_text(drawing_cr, angle_text);
    
    // draw axis labels
    cairo_move_to(drawing_cr, origin_x + 8.0, 10.0);
    cairo_set_source(drawing_cr, axis_text_color);
    cairo_set_font_size(drawing_cr, font_size);
    cairo_show_text(drawing_cr, "y axis");
    
    if (angle > 0.0)
        cairo_move_to(drawing_cr, 10.0, origin_y + 5.0 + angle_text_extents.height);
    else
        cairo_move_to(drawing_cr, 10.0, origin_y - 8.0);
    
    cairo_set_source(drawing_cr, axis_text_color);
    cairo_set_font_size(drawing_cr, font_size);
    cairo_show_text(drawing_cr, "x axis");
    
    // draw quadrant labels
    cairo_move_to(drawing_cr, width - quad_offset - angle_text_extents.width, quad_offset + angle_text_extents.height);
    cairo_set_source(drawing_cr, quadrant_text_color);
    cairo_set_font_size(drawing_cr, font_size);
    cairo_show_text(drawing_cr, "Quadrant 1");
    
    cairo_move_to(drawing_cr, quad_offset, quad_offset + angle_text_extents.height);
    cairo_set_source(drawing_cr, quadrant_text_color);
    cairo_set_font_size(drawing_cr, font_size);
    cairo_show_text(drawing_cr, "Quadrant 2");
    
    cairo_move_to(drawing_cr, quad_offset, height - quad_offset - angle_text_extents.height);
    cairo_set_source(drawing_cr, quadrant_text_color);
    cairo_set_font_size(drawing_cr, font_size);
    cairo_show_text(drawing_cr, "Quadrant 3");
    
    cairo_move_to(drawing_cr, width - quad_offset - angle_text_extents.width, height - quad_offset - angle_text_extents.height);
    cairo_set_source(drawing_cr, quadrant_text_color);
    cairo_set_font_size(drawing_cr, font_size);
    cairo_show_text(drawing_cr, "Quadrant 4");
    
    // now copy drawing into figure
    cairo_destroy(drawing_cr);
    cairo_set_source_surface(cr, drawing_surface, 0.0, 0.0);
    cairo_paint(cr);
    cairo_surface_destroy(drawing_surface);
}

static void update_angle()
{
    // find the figure widget
    GtkWidget * figure = lookup_widget(main_window,"figure");
   
    // get a cairo object
    cairo_t * cr = gdk_cairo_create(figure->window);
    
    // draw the image
    figure_draw(figure, cr);

    // done
    cairo_destroy(cr);
}

static void update_numbers(GtkWidget * widget, enum field_no skip)
{
    char buffer[8];

    changing = true;
    
    if (skip != ANGLE_DEG)
    {
        snprintf(buffer,8,"%-5.1f",(angle / RAD2PI));
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(widget,"angle_deg_entry")),buffer);
    }
    
    if (skip != ANGLE_RAD)
    {
        snprintf(buffer,8,"%5.3f",angle);
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(widget,"angle_rad_entry")),buffer);
    }
    
    if (skip != TRIG_SIN)
    {
        snprintf(buffer,8,"%5.3f",sin(angle));
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(widget,"trig_sin_entry")),buffer);
    }
    
    if (skip != TRIG_COS)
    {
        snprintf(buffer,8,"%5.3f",cos(angle));
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(widget,"trig_cos_entry")),buffer);
    }
    
    if (skip != TRIG_TAN)
    {
        snprintf(buffer,8,"%5.3f",tan(angle));
        gtk_entry_set_text(GTK_ENTRY(lookup_widget(widget,"trig_tan_entry")),buffer);
        changing = false;
    }
}

gboolean
on_figure_events_event                 (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    double dx = 0.0, dy = 0.0;
    bool adjust_angle = false;
    double origin_x = widget->allocation.width /  2.0;
    double origin_y = widget->allocation.height / 2.0;
    
    switch (event->type) 
    {
        case GDK_MOTION_NOTIFY:
            dx = event->motion.x - origin_x;
            dy = origin_y - event->motion.y;
            adjust_angle = true;
            break;
        case GDK_BUTTON_PRESS:
        case GDK_BUTTON_RELEASE:
            dx = event->button.x - origin_x;
            dy = origin_y - event->button.y;
            adjust_angle = true;
            break;
            
        default:
            adjust_angle = false;
            break;
    }
    
    if (adjust_angle)
    {
        // compute new angle
        angle = atan2(dy, dx);

        // update the numbers
        update_angle();
        update_numbers(main_window,NO_SKIP);
    }

    return FALSE;
}

static void figure_reset()
{
    // set the angle to zero
    angle = DEFAULT_ANGLE;
    update_angle();
    update_numbers(main_window,NO_SKIP);
}

void
on_revert_button_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    figure_reset();
}

void
on_about_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget * dialog = gtk_about_dialog_new();
    
    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG (dialog),"Unit Circle");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG (dialog),VERSION);
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG (dialog),"Copyright 2016 Scott Robert Ladd");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG (dialog),"A tool for exploring angles and trigonometric functions.");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG (dialog),"http://www.drakontos.com");

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}


void
on_close_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
    gtk_main_quit();
}


void
on_main_window_realize                 (GtkWidget       *widget,
                                        gpointer         user_data)
{
    main_window = widget;
    gtk_window_set_icon_from_file(GTK_WINDOW(main_window),ICON_FILENAME,NULL);
    gtk_window_set_default_icon_from_file(ICON_FILENAME,NULL);
    
    circle_fill_color   = cairo_pattern_create_rgb(1.0, 1.0, 0.9);
    circle_line_color   = cairo_pattern_create_rgb(0.5, 0.5, 0.5);
    axis_line_color     = cairo_pattern_create_rgb(0.0, 0.0, 0.0);
    angle_line_color    = cairo_pattern_create_rgb(1.0, 0.0, 0.5);
    theta_line_color    = cairo_pattern_create_rgb(1.0, 0.0, 0.0);
    connect_line_color  = cairo_pattern_create_rgb(0.5, 0.0, 1.0);
    angle_text_color    = cairo_pattern_create_rgb(0.0, 0.0, 0.0);
    axis_text_color     = cairo_pattern_create_rgb(0.0, 0.0, 0.7);
    quadrant_text_color = cairo_pattern_create_rgb(0.2, 0.0, 0.5);
}


gboolean
on_main_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_main_quit();
    return FALSE;
}


gboolean
on_main_window_destroy_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_main_quit();
    return FALSE;
}


gboolean
on_figure_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    printf("on_figure_button_press_event\n");
    return FALSE;
}


void
on_angle_deg_entry_changed             (GtkEditable     *editable,
                                        gpointer         user_data)
{
    if (!changing)
    {
        angle = atof(gtk_entry_get_text(GTK_ENTRY(editable)));
        angle *= RAD2PI;
        update_angle();
        update_numbers(main_window,ANGLE_DEG);
    }
}


void
on_angle_rad_entry_changed             (GtkEditable     *editable,
                                        gpointer         user_data)
{
    if (!changing)
    {
        angle = atof(gtk_entry_get_text(GTK_ENTRY(editable)));
        update_angle();
        update_numbers(main_window,ANGLE_RAD);
    }
}


void
on_trig_tan_entry_changed              (GtkEditable     *editable,
                                        gpointer         user_data)
{
    if (!changing)
    {
        angle = atan(atof(gtk_entry_get_text(GTK_ENTRY(editable))));
        update_angle();
        update_numbers(main_window,TRIG_TAN);
    }
}


void
on_trig_cos_entry_changed              (GtkEditable     *editable,
                                        gpointer         user_data)
{
    if (!changing)
    {
        angle = acos(atof(gtk_entry_get_text(GTK_ENTRY(editable))));
        update_angle();
        update_numbers(main_window,TRIG_COS);
    }
}


void
on_trig_sin_entry_changed              (GtkEditable     *editable,
                                        gpointer         user_data)
{
    if (!changing)
    {
        angle = asin(atof(gtk_entry_get_text(GTK_ENTRY(editable))));
        update_angle();
        update_numbers(main_window,TRIG_SIN);
    }
}

gboolean
on_figure_expose_event                 (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
    // get a cairo object
    cairo_t * cr = gdk_cairo_create(widget->window);
    
    // manage clipping rectangle
    cairo_rectangle(cr, event->area.x, event->area.y,
                       event->area.width, event->area.height);
    
    cairo_clip(cr);
    
    cairo_set_font_size(cr, font_size);
    cairo_text_extents(cr, "(-0.000, -0.000)", &angle_text_extents);
    cairo_text_extents(cr, theta_text, &theta_text_extents);
    
    inset = 10.0 + angle_text_extents.width;

    // draw the image
    figure_draw(widget, cr);
    
    // update text fields
    update_numbers(main_window,NO_SKIP);

    // done
    cairo_destroy(cr);
    return FALSE;
}
