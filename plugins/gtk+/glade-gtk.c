/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2001 Ximian, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Authors:
 *   Chema Celorio <chema@celorio.com>
 *   Tristan Van Berkom <tvb@gnome.org>
 *   Juan Pablo Ugarte <juanpablougarte@gmail.com>
 */

#include <config.h>

#include "glade-gtk.h"
#include "fixed-bg.xpm"

#include <gladeui/glade-editor-property.h>
#include <gladeui/glade-base-editor.h>


#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>
#include <string.h>
#include <stdlib.h>

/* --------------------------------- Constants ------------------------------ */
#define FIXED_DEFAULT_CHILD_WIDTH  100
#define FIXED_DEFAULT_CHILD_HEIGHT 60


/* -------------------------------- ParamSpecs ------------------------------ */
/*
GtkImageMenuItem GnomeUI "stock_item" property special case:
	
"stock_item" property is added by glade2 gnome support and makes reference to
GNOMEUIINFO_MENU_* macros. This set-function maps these properties to 
existing non deprecated gtk ones.
*/
typedef enum {
	GNOMEUIINFO_MENU_NONE,
	/* The 'File' menu */
	GNOMEUIINFO_MENU_NEW_ITEM,
	GNOMEUIINFO_MENU_NEW_SUBTREE,
	GNOMEUIINFO_MENU_OPEN_ITEM,
	GNOMEUIINFO_MENU_SAVE_ITEM,
	GNOMEUIINFO_MENU_SAVE_AS_ITEM,
	GNOMEUIINFO_MENU_REVERT_ITEM,
	GNOMEUIINFO_MENU_PRINT_ITEM,
	GNOMEUIINFO_MENU_PRINT_SETUP_ITEM,
	GNOMEUIINFO_MENU_CLOSE_ITEM,
	GNOMEUIINFO_MENU_EXIT_ITEM,
	GNOMEUIINFO_MENU_QUIT_ITEM,
	/* The "Edit" menu */
	GNOMEUIINFO_MENU_CUT_ITEM,
	GNOMEUIINFO_MENU_COPY_ITEM,
	GNOMEUIINFO_MENU_PASTE_ITEM,
	GNOMEUIINFO_MENU_SELECT_ALL_ITEM,
	GNOMEUIINFO_MENU_CLEAR_ITEM,
	GNOMEUIINFO_MENU_UNDO_ITEM,
	GNOMEUIINFO_MENU_REDO_ITEM,
	GNOMEUIINFO_MENU_FIND_ITEM,
	GNOMEUIINFO_MENU_FIND_AGAIN_ITEM,
	GNOMEUIINFO_MENU_REPLACE_ITEM,
	GNOMEUIINFO_MENU_PROPERTIES_ITEM,
	/* The Settings menu */
	GNOMEUIINFO_MENU_PREFERENCES_ITEM,
	/* The Windows menu */
	GNOMEUIINFO_MENU_NEW_WINDOW_ITEM,
	GNOMEUIINFO_MENU_CLOSE_WINDOW_ITEM,
	/* And the "Help" menu */
	GNOMEUIINFO_MENU_ABOUT_ITEM,
	/* The "Game" menu */
	GNOMEUIINFO_MENU_NEW_GAME_ITEM,
	GNOMEUIINFO_MENU_PAUSE_GAME_ITEM,
	GNOMEUIINFO_MENU_RESTART_GAME_ITEM,
	GNOMEUIINFO_MENU_UNDO_MOVE_ITEM,
	GNOMEUIINFO_MENU_REDO_MOVE_ITEM,
	GNOMEUIINFO_MENU_HINT_ITEM,
	GNOMEUIINFO_MENU_SCORES_ITEM,
	GNOMEUIINFO_MENU_END_GAME_ITEM,
	/* Some standard menus */
	GNOMEUIINFO_MENU_FILE_TREE,
	GNOMEUIINFO_MENU_EDIT_TREE,
	GNOMEUIINFO_MENU_VIEW_TREE,
	GNOMEUIINFO_MENU_SETTINGS_TREE,
	GNOMEUIINFO_MENU_FILES_TREE,
	GNOMEUIINFO_MENU_WINDOWS_TREE,
	GNOMEUIINFO_MENU_HELP_TREE,
	GNOMEUIINFO_MENU_GAME_TREE
} GladeGtkGnomeUIInfoEnum;

static GType
glade_gtk_gnome_ui_info_get_type (void)
{
	static GType etype = 0;
	if (etype == 0) {
		static const GEnumValue values[] = {
			{ GNOMEUIINFO_MENU_NONE, "GNOMEUIINFO_MENU_NONE", NULL},
			/* The 'File' menu */
			{ GNOMEUIINFO_MENU_NEW_ITEM, "GNOMEUIINFO_MENU_NEW_ITEM", "gtk-new"},
			{ GNOMEUIINFO_MENU_OPEN_ITEM, "GNOMEUIINFO_MENU_OPEN_ITEM", "gtk-open"},
			{ GNOMEUIINFO_MENU_SAVE_ITEM, "GNOMEUIINFO_MENU_SAVE_ITEM", "gtk-save"},
			{ GNOMEUIINFO_MENU_SAVE_AS_ITEM, "GNOMEUIINFO_MENU_SAVE_AS_ITEM", "gtk-save-as"},
			{ GNOMEUIINFO_MENU_REVERT_ITEM, "GNOMEUIINFO_MENU_REVERT_ITEM", "gtk-revert-to-saved"},
			{ GNOMEUIINFO_MENU_PRINT_ITEM, "GNOMEUIINFO_MENU_PRINT_ITEM", "gtk-print"},
			{ GNOMEUIINFO_MENU_PRINT_SETUP_ITEM, "GNOMEUIINFO_MENU_PRINT_SETUP_ITEM", NULL},
			{ GNOMEUIINFO_MENU_CLOSE_ITEM, "GNOMEUIINFO_MENU_CLOSE_ITEM", "gtk-close"},
			{ GNOMEUIINFO_MENU_EXIT_ITEM, "GNOMEUIINFO_MENU_EXIT_ITEM", "gtk-quit"},
			{ GNOMEUIINFO_MENU_QUIT_ITEM, "GNOMEUIINFO_MENU_QUIT_ITEM", "gtk-quit"},
			/* The "Edit" menu */
			{ GNOMEUIINFO_MENU_CUT_ITEM, "GNOMEUIINFO_MENU_CUT_ITEM", "gtk-cut"},
			{ GNOMEUIINFO_MENU_COPY_ITEM, "GNOMEUIINFO_MENU_COPY_ITEM", "gtk-copy"},
			{ GNOMEUIINFO_MENU_PASTE_ITEM, "GNOMEUIINFO_MENU_PASTE_ITEM", "gtk-paste"},
			{ GNOMEUIINFO_MENU_SELECT_ALL_ITEM, "GNOMEUIINFO_MENU_SELECT_ALL_ITEM", NULL},
			{ GNOMEUIINFO_MENU_CLEAR_ITEM, "GNOMEUIINFO_MENU_CLEAR_ITEM", "gtk-clear"},
			{ GNOMEUIINFO_MENU_UNDO_ITEM, "GNOMEUIINFO_MENU_UNDO_ITEM", "gtk-undo"},
			{ GNOMEUIINFO_MENU_REDO_ITEM, "GNOMEUIINFO_MENU_REDO_ITEM", "gtk-redo"},
			{ GNOMEUIINFO_MENU_FIND_ITEM, "GNOMEUIINFO_MENU_FIND_ITEM", "gtk-find"},
			{ GNOMEUIINFO_MENU_FIND_AGAIN_ITEM, "GNOMEUIINFO_MENU_FIND_AGAIN_ITEM", NULL},
			{ GNOMEUIINFO_MENU_REPLACE_ITEM, "GNOMEUIINFO_MENU_REPLACE_ITEM", "gtk-find-and-replace"},
			{ GNOMEUIINFO_MENU_PROPERTIES_ITEM, "GNOMEUIINFO_MENU_PROPERTIES_ITEM", "gtk-properties"},
			/* The Settings menu */
			{ GNOMEUIINFO_MENU_PREFERENCES_ITEM, "GNOMEUIINFO_MENU_PREFERENCES_ITEM", "gtk-preferences"},
			/* The Windows menu */
			{ GNOMEUIINFO_MENU_NEW_WINDOW_ITEM, "GNOMEUIINFO_MENU_NEW_WINDOW_ITEM", NULL},
			{ GNOMEUIINFO_MENU_CLOSE_WINDOW_ITEM, "GNOMEUIINFO_MENU_CLOSE_WINDOW_ITEM", NULL},
			/* And the "Help" menu */
			{ GNOMEUIINFO_MENU_ABOUT_ITEM, "GNOMEUIINFO_MENU_ABOUT_ITEM", "gtk-about"},
			/* The "Game" menu */
			{ GNOMEUIINFO_MENU_NEW_GAME_ITEM, "GNOMEUIINFO_MENU_NEW_GAME_ITEM", NULL},
			{ GNOMEUIINFO_MENU_PAUSE_GAME_ITEM, "GNOMEUIINFO_MENU_PAUSE_GAME_ITEM", NULL},
			{ GNOMEUIINFO_MENU_RESTART_GAME_ITEM, "GNOMEUIINFO_MENU_RESTART_GAME_ITEM", NULL},
			{ GNOMEUIINFO_MENU_UNDO_MOVE_ITEM, "GNOMEUIINFO_MENU_UNDO_MOVE_ITEM", NULL},
			{ GNOMEUIINFO_MENU_REDO_MOVE_ITEM, "GNOMEUIINFO_MENU_REDO_MOVE_ITEM", NULL},
			{ GNOMEUIINFO_MENU_HINT_ITEM, "GNOMEUIINFO_MENU_HINT_ITEM", NULL},
			{ GNOMEUIINFO_MENU_SCORES_ITEM, "GNOMEUIINFO_MENU_SCORES_ITEM", NULL},
			{ GNOMEUIINFO_MENU_END_GAME_ITEM, "GNOMEUIINFO_MENU_END_GAME_ITEM", NULL},
			/* Some standard menus */
			{ GNOMEUIINFO_MENU_FILE_TREE, "GNOMEUIINFO_MENU_FILE_TREE", NULL},
			{ GNOMEUIINFO_MENU_EDIT_TREE, "GNOMEUIINFO_MENU_EDIT_TREE", NULL},
			{ GNOMEUIINFO_MENU_VIEW_TREE, "GNOMEUIINFO_MENU_VIEW_TREE", NULL},
			{ GNOMEUIINFO_MENU_SETTINGS_TREE, "GNOMEUIINFO_MENU_SETTINGS_TREE", NULL},
			{ GNOMEUIINFO_MENU_FILES_TREE, "GNOMEUIINFO_MENU_FILES_TREE", NULL},
			{ GNOMEUIINFO_MENU_WINDOWS_TREE, "GNOMEUIINFO_MENU_WINDOWS_TREE", NULL},
			{ GNOMEUIINFO_MENU_HELP_TREE, "GNOMEUIINFO_MENU_HELP_TREE", NULL},
			{ GNOMEUIINFO_MENU_GAME_TREE, "GNOMEUIINFO_MENU_GAME_TREE", NULL},
			{ 0, NULL, NULL }
		};
		etype = g_enum_register_static ("GladeGtkGnomeUIInfo", values);
	}
	return etype;
}

GParamSpec *
glade_gtk_gnome_ui_info_spec (void)
{
	return g_param_spec_enum ("gnomeuiinfo", _("GnomeUIInfo"), 
				  _("Choose the GnomeUIInfo stock item"),
				  glade_gtk_gnome_ui_info_get_type (),
				  0, G_PARAM_READWRITE);
}

GType
glade_gtk_image_type_get_type (void)
{
	static GType etype = 0;
	if (etype == 0) {
		static GEnumValue values[] = {
			{ GLADEGTK_IMAGE_FILENAME,  "GLADEGTK_IMAGE_FILENAME",   "glade-gtk-image-filename" },
			{ GLADEGTK_IMAGE_STOCK,     "GLADEGTK_IMAGE_STOCK",      "glade-gtk-image-stock" },
			{ GLADEGTK_IMAGE_ICONTHEME, "GLADEGTK_IMAGE_ICONTHEME", "glade-gtk-image-icontheme" },
			{ 0, NULL, NULL }
		};

		etype = g_enum_register_static ("GladeGtkImageType", values);
	}
	return etype;
}

GType
glade_gtk_button_type_get_type (void)
{
	static GType etype = 0;
	if (etype == 0) {
		static GEnumValue values[] = {
			{ GLADEGTK_BUTTON_LABEL,     "GLADEGTK_BUTTON_LABEL", "glade-gtk-button-label" },
			{ GLADEGTK_BUTTON_STOCK,     "GLADEGTK_BUTTON_STOCK", "glade-gtk-button-stock" },
			{ GLADEGTK_BUTTON_CONTAINER, "GLADEGTK_BUTTON_CONTAINER", "glade-gtk-button-container" },
			{ 0, NULL, NULL }
		};

		etype = g_enum_register_static ("GladeGtkButtonType", values);
	}
	return etype;
}

GParamSpec *
glade_gtk_image_type_spec (void)
{
	return g_param_spec_enum ("type", _("Method"), 
				  _("The method to use to edit this image"),
				  glade_gtk_image_type_get_type (),
				  1, G_PARAM_READWRITE);
}

GParamSpec *
glade_gtk_button_type_spec (void)
{
	return g_param_spec_enum ("type", _("Method"), 
				  _("The method to use to edit this button"),
				  glade_gtk_button_type_get_type (),
				  0, G_PARAM_READWRITE);
}


/* This function does absolutely nothing
 * (and is for use in overriding post_create functions).
 */
void
empty (GObject *container, GladeCreateReason reason)
{
}

/* This function is used to stop default handlers  */
static void
glade_gtk_stop_emission_POINTER (gpointer instance, gpointer dummy, gpointer data)
{
	g_signal_stop_emission (instance, GPOINTER_TO_UINT (data) , 0);
}

/* ----------------------------- GtkWidget ------------------------------ */
static void
widget_parent_changed (GtkWidget          *widget,
		       GParamSpec         *pspec,
		       GladeWidgetAdaptor *adaptor)
{
	GladeWidget *gwidget = glade_widget_get_from_gobject (widget);

	/* this could get called for a stale instance of an object
	 * being rebuilt for a contruct-only property. */
	if (!gwidget)
		return;

	if (gwidget->parent && !GTK_IS_WINDOW (glade_widget_get_object (gwidget->parent)) &&
	    gwidget->parent->internal == NULL)
		glade_widget_set_action_sensitive (gwidget, "remove_parent", TRUE);
	else
		glade_widget_set_action_sensitive (gwidget, "remove_parent", FALSE);

	if (gwidget->internal)
		glade_widget_set_action_sensitive (gwidget, "add_parent", FALSE);	
}

void
glade_gtk_widget_deep_post_create (GladeWidgetAdaptor *adaptor,
				   GObject            *widget, 
				   GladeCreateReason   reason)
{
	GladeWidget *gwidget = glade_widget_get_from_gobject (widget);

	/* Work around bug 472555 by resetting the default event mask,
	 * this way only user edits will be saved to the glade file. */
	if (reason == GLADE_CREATE_USER)
		glade_widget_property_reset (gwidget, "events");
	
	glade_widget_set_action_sensitive (gwidget, "remove_parent", FALSE);

	if (GTK_IS_WINDOW (widget) || gwidget->internal)
		glade_widget_set_action_sensitive (gwidget, "add_parent", FALSE);

	/* Watch parents and set actions sensitive/insensitive */
	g_signal_connect (G_OBJECT (widget), "notify::parent",
			  G_CALLBACK (widget_parent_changed), adaptor);
}


void
glade_gtk_widget_set_property (GladeWidgetAdaptor *adaptor,
			       GObject            *object, 
			       const gchar        *id,
			       const GValue       *value)
{
        /* FIXME: is this still needed with the new gtk+ tooltips? */
	if (!strcmp (id, "tooltip"))
	{
                id = "tooltip-text";
        }

        GWA_GET_CLASS (G_TYPE_OBJECT)->set_property (adaptor, object, id, value);
}

void
glade_gtk_widget_get_property (GladeWidgetAdaptor *adaptor,
			       GObject            *object, 
			       const gchar        *id,
			       GValue             *value)
{
	if (!strcmp (id, "tooltip"))
	{
                id = "tooltip-text";
	}
	
        GWA_GET_CLASS (G_TYPE_OBJECT)->get_property (adaptor, object, id, value);
}

static GList *
create_command_property_list (GladeWidget *gnew, GList *saved_props)
{
	GList *l, *command_properties = NULL;
	
	for (l = saved_props; l; l = l->next)
	{
		GladeProperty *property = l->data;
		GladeProperty *orig_prop = glade_widget_get_pack_property (gnew, property->klass->id);
		GCSetPropData *pdata = g_new0 (GCSetPropData, 1);

		pdata->property  = orig_prop;
		pdata->old_value = g_new0 (GValue, 1);
		pdata->new_value = g_new0 (GValue, 1);

		glade_property_get_value (orig_prop, pdata->old_value);
		glade_property_get_value (property, pdata->new_value);

		command_properties = g_list_prepend (command_properties, pdata);
	}
	return g_list_reverse (command_properties);
}


void
glade_gtk_widget_action_activate (GladeWidgetAdaptor *adaptor,
				  GObject *object,
				  const gchar *action_path)
{
	GladeWidget *gwidget = glade_widget_get_from_gobject (object), *gparent;
	GList       this_widget = { 0, }, that_widget = { 0, };
	GtkWidget   *parent = GTK_WIDGET (object)->parent;

	g_assert (parent);

	gparent = glade_widget_get_from_gobject (parent);
	
	if (strcmp (action_path, "remove_parent") == 0)
	{
		GladeWidget *new_gparent = gparent->parent;

		/* Since toplevel project objects for now must be GtkWindow,
		 * we'll just assert this for now (should be an insensitive action).
		 */
		g_assert (!GTK_IS_WINDOW (parent));
		
		glade_command_push_group (_("Removing parent of %s"), 
					  gwidget->name);

		/* Remove "this" widget */
		this_widget.data = gwidget;
		glade_command_cut (&this_widget);
		
		/* Delete the parent */
		that_widget.data = gparent;
		glade_command_delete (&that_widget);

		/* Add "this" widget to the new parent */
		glade_command_paste(&this_widget, new_gparent, NULL);
		
		glade_command_pop_group ();
	}
	else if (strncmp (action_path, "add_parent/", 11) == 0)
	{
		GType new_type = 0;
		
		if (strcmp (action_path + 11, "alignment") == 0)
			new_type = GTK_TYPE_ALIGNMENT;
		else if (strcmp (action_path + 11, "viewport") == 0)
			new_type = GTK_TYPE_VIEWPORT;
		else if (strcmp (action_path + 11, "eventbox") == 0)
			new_type = GTK_TYPE_EVENT_BOX;
		else if (strcmp (action_path + 11, "frame") == 0)
			new_type = GTK_TYPE_FRAME;
		else if (strcmp (action_path + 11, "aspect_frame") == 0)
			new_type = GTK_TYPE_ASPECT_FRAME;
		else if (strcmp (action_path + 11, "scrolled_window") == 0)
			new_type = GTK_TYPE_SCROLLED_WINDOW;
		else if (strcmp (action_path + 11, "expander") == 0)
			new_type = GTK_TYPE_EXPANDER;
		else if (strcmp (action_path + 11, "table") == 0)
			new_type = GTK_TYPE_TABLE;
		else if (strcmp (action_path + 11, "hbox") == 0)
			new_type = GTK_TYPE_HBOX;
		else if (strcmp (action_path + 11, "vbox") == 0)
			new_type = GTK_TYPE_VBOX;
		else if (strcmp (action_path + 11, "hpaned") == 0)
			new_type = GTK_TYPE_HPANED;
		else if (strcmp (action_path + 11, "vpaned") == 0)
			new_type = GTK_TYPE_VPANED;

		
		if (new_type)
		{
			GladeWidgetAdaptor *adaptor = glade_widget_adaptor_get_by_type (new_type);
			GList              *saved_props, *prop_cmds;
			
			glade_command_push_group (_("Adding parent %s to %s"), 
						  adaptor->title, gwidget->name);

			/* Record packing properties */
			saved_props = glade_widget_dup_properties (gwidget->packing_properties, FALSE);
			
			/* Remove "this" widget */
			this_widget.data = gwidget;
			glade_command_cut (&this_widget);
			
			/* Create new widget and put it where the placeholder was */
			that_widget.data =
				glade_command_create (adaptor, gparent, NULL, 
						      glade_widget_get_project (gparent));


			/* Remove the alignment that we added in the frame's post_create... */
			if (new_type == GTK_TYPE_FRAME)
			{
				GObject     *frame = glade_widget_get_object (that_widget.data);
				GladeWidget *galign = glade_widget_get_from_gobject (GTK_BIN (frame)->child);
				GList        to_delete = { 0, };

				to_delete.data = galign;
				glade_command_delete (&to_delete);
			}
						
			/* Create heavy-duty glade-command properties stuff */
			prop_cmds = create_command_property_list (that_widget.data, saved_props);
			g_list_foreach (saved_props, (GFunc)g_object_unref, NULL);
			g_list_free (saved_props);

			/* Apply the properties in an undoable way */
			if (prop_cmds)
				glade_command_set_properties_list (glade_widget_get_project (gparent), prop_cmds);
			
			/* Add "this" widget to the new parent */
			glade_command_paste(&this_widget, GLADE_WIDGET (that_widget.data), NULL);
			
			glade_command_pop_group ();
		}

	}

	else
		GWA_GET_CLASS (G_TYPE_OBJECT)->action_activate (adaptor,
								object,
								action_path);
}


/* ----------------------------- GtkContainer ------------------------------ */
void
glade_gtk_container_post_create (GladeWidgetAdaptor *adaptor,
				 GObject            *container, 
				 GladeCreateReason   reason)
{
	GList *children;
	g_return_if_fail (GTK_IS_CONTAINER (container));

	if (reason == GLADE_CREATE_USER)
	{
		if ((children = gtk_container_get_children (GTK_CONTAINER (container))) == NULL)
			gtk_container_add (GTK_CONTAINER (container), glade_placeholder_new ());
		else
			g_list_free (children);
	}
}

void
glade_gtk_container_replace_child (GladeWidgetAdaptor *adaptor,
				   GtkWidget          *container,
				   GtkWidget          *current,
				   GtkWidget          *new_widget)
{
	GParamSpec **param_spec;
	GladePropertyClass *pclass;
	GValue *value;
	guint nproperties;
	guint i;

	if (current->parent != container)
		return;

	param_spec = gtk_container_class_list_child_properties
		(G_OBJECT_GET_CLASS (container), &nproperties);
	value = g_malloc0 (sizeof(GValue) * nproperties);

	for (i = 0; i < nproperties; i++) 
	{
		g_value_init (&value[i], param_spec[i]->value_type);
		gtk_container_child_get_property
			(GTK_CONTAINER (container), current, param_spec[i]->name, &value[i]);
	}

	gtk_container_remove (GTK_CONTAINER (container), current);
	gtk_container_add (GTK_CONTAINER (container), new_widget);

	for (i = 0; i < nproperties; i++) 
	{
		/* If the added widget is a placeholder then we
		 * want to keep all the "tranfer-on-paste" properties
		 * as default so that it looks fresh (transfer-on-paste
		 * properties dont effect the position/slot inside a 
		 * contianer)
		 */
		if (GLADE_IS_PLACEHOLDER (new_widget))
		{
			pclass = glade_widget_adaptor_get_pack_property_class
				(adaptor, param_spec[i]->name);

			if (pclass && pclass->transfer_on_paste)
				continue;
		}

		gtk_container_child_set_property
			(GTK_CONTAINER (container), new_widget, param_spec[i]->name, &value[i]);
	}
	
	for (i = 0; i < nproperties; i++)
		g_value_unset (&value[i]);

	g_free (param_spec);
	g_free (value);
}

void
glade_gtk_container_add_child (GladeWidgetAdaptor *adaptor,
			       GtkWidget          *container,
			       GtkWidget          *child)
{
	/* Get a placeholder out of the way before adding the child if its a GtkBin
	 */
	if (GTK_IS_BIN (container) && GTK_BIN (container)->child != NULL && 
	    GLADE_IS_PLACEHOLDER (GTK_BIN (container)->child))
		gtk_container_remove (GTK_CONTAINER (container), GTK_BIN (container)->child);

	gtk_container_add (GTK_CONTAINER (container), child);
}

void
glade_gtk_container_remove_child (GladeWidgetAdaptor *adaptor,
				  GtkWidget          *container,
				  GtkWidget          *child)
{
	gtk_container_remove (GTK_CONTAINER (container), child);

	/* If this is the last one, add a placeholder by default.
	 */
	if (gtk_container_get_children (GTK_CONTAINER (container)) == NULL)
		gtk_container_add (GTK_CONTAINER (container), glade_placeholder_new ());
}

void
glade_gtk_container_set_child_property (GladeWidgetAdaptor *adaptor,
					GObject            *container,
					GObject            *child,
					const gchar        *property_name,
					const GValue       *value)
{
	gtk_container_child_set_property (GTK_CONTAINER (container),
					  GTK_WIDGET (child),
					  property_name, value);
}

void
glade_gtk_container_get_child_property (GladeWidgetAdaptor *adaptor,
					GObject            *container,
					GObject            *child,
					const gchar        *property_name,
					GValue             *value)
{
	gtk_container_child_get_property (GTK_CONTAINER (container),
					  GTK_WIDGET (child),
					  property_name, value);
}


GList *
glade_gtk_container_get_children (GladeWidgetAdaptor  *adaptor,
				  GtkContainer        *container)
{
	return glade_util_container_get_all_children (container);
}

/* ----------------------------- GtkBox ------------------------------ */
typedef struct {
	GtkWidget *widget;
	gint       position;
} GladeGtkBoxChild;

static GList *glade_gtk_box_original_positions = NULL;

static gboolean
glade_gtk_box_configure_child (GladeFixed   *fixed,
			       GladeWidget  *child,
			       GdkRectangle *rect,
			       GtkWidget    *box)
{
	GList       *list;
	GtkBoxChild *bchild;
	gint         point, trans_point, span, 
		iter_span, position, old_position, 
		offset, orig_offset;
	gboolean     found = FALSE;

	if (GTK_IS_HBOX (box) || GTK_IS_HBUTTON_BOX (box))
	{
		point       = fixed->mouse_x;
		span        = GTK_WIDGET (child->object)->allocation.width;
		offset      = rect->x;
		orig_offset = fixed->child_x_origin;
	}
	else
	{
		point       = fixed->mouse_y;
		span        = GTK_WIDGET (child->object)->allocation.height;
		offset      = rect->y;
		orig_offset = fixed->child_y_origin;
	}

	glade_widget_pack_property_get
		(child, "position", &old_position);

	for (list = GTK_BOX (box)->children; list; list = list->next)
	{
		bchild = list->data;

		if (bchild->widget == GTK_WIDGET (child->object))
			continue;

		/* Find the widget in the box where the center of
		 * this rectangle fits... and set the position to that
		 * position.
		 */

		if (GTK_IS_HBOX (box) || GTK_IS_HBUTTON_BOX (box))
		{
			gtk_widget_translate_coordinates 
				(GTK_WIDGET (box), bchild->widget,
				 point, 0, &trans_point, NULL);

			iter_span   = bchild->widget->allocation.width;
		}
		else
		{
			gtk_widget_translate_coordinates 
				(GTK_WIDGET (box), bchild->widget,
				 0, point, NULL, &trans_point);
			iter_span = bchild->widget->allocation.height;
		}

#if 0
		gtk_container_child_get (GTK_CONTAINER (box),
					 bchild->widget,
					 "position", &position, NULL);
		g_print ("widget: %p pos %d, point %d, trans_point %d, iter_span %d\n", 
			 bchild->widget, position, point, trans_point, iter_span);
#endif

		if (iter_span <= span)
		{
			found = trans_point >= 0 && trans_point < iter_span;
		}
		else
		{
			if (offset > orig_offset)
				found = trans_point >= iter_span - span && 
					trans_point < iter_span;
			else if (offset < orig_offset)
				found = trans_point >= 0 &&
					trans_point < span;
		}

		if (found)
		{
			gtk_container_child_get (GTK_CONTAINER (box),
						 bchild->widget,
						 "position", &position, NULL);

#if 0
			g_print ("setting position of %s from %d to %d, "
				 "(point %d iter_span %d)\n", 
				 child->name, old_position, position, trans_point, iter_span);
#endif

			glade_widget_pack_property_set
				(child, "position", position);
							
			break;
		}

	}
	return TRUE;
}

static gboolean
glade_gtk_box_configure_begin (GladeFixed  *fixed,
			       GladeWidget *child,
			       GtkWidget   *box)
{
	GList       *list;
	GtkBoxChild *bchild;

	g_assert (glade_gtk_box_original_positions == NULL);

	for (list = GTK_BOX (box)->children; list; list = list->next)
	{
		GladeGtkBoxChild *gbchild;
		GladeWidget      *gchild;
				
		bchild = list->data;
		if ((gchild = glade_widget_get_from_gobject (bchild->widget)) == NULL)
			continue;

		gbchild         = g_new0 (GladeGtkBoxChild, 1);
		gbchild->widget = bchild->widget;
		glade_widget_pack_property_get (gchild, "position",
						&gbchild->position);

		glade_gtk_box_original_positions = 
			g_list_prepend (glade_gtk_box_original_positions, gbchild);
	}

	return TRUE;
}

static gboolean
glade_gtk_box_configure_end (GladeFixed  *fixed,
			     GladeWidget *child,
			     GtkWidget   *box)
{
	GList         *list, *l;
	GList         *prop_list = NULL;

	for (list = GTK_BOX (box)->children; list; list = list->next)
	{
		GtkBoxChild *bchild = list->data;

		for (l = glade_gtk_box_original_positions; l; l = l->next)
		{
			GladeGtkBoxChild *gbchild = l->data;
			GladeWidget      *gchild = 
				glade_widget_get_from_gobject (gbchild->widget);


			if (bchild->widget == gbchild->widget)
			{
				GCSetPropData *prop_data = g_new0 (GCSetPropData, 1);
				prop_data->property = 
					glade_widget_get_pack_property
					(gchild, "position");

				prop_data->old_value = g_new0 (GValue, 1);
				prop_data->new_value = g_new0 (GValue, 1);

				glade_property_get_value (prop_data->property,
							  prop_data->new_value);

				g_value_init (prop_data->old_value, G_TYPE_INT);
				g_value_set_int (prop_data->old_value, gbchild->position);

				prop_list = g_list_prepend (prop_list, prop_data);
				break;
			}
		}
	}

	glade_command_push_group (_("Ordering children of %s"), 
				  GLADE_WIDGET (fixed)->name);
	glade_property_push_superuser ();
	if (prop_list)
		glade_command_set_properties_list (GLADE_WIDGET (fixed)->project,
						   prop_list);
	glade_property_pop_superuser ();
	glade_command_pop_group ();

	for (l = glade_gtk_box_original_positions; l; l = l->next)
		g_free (l->data);

	glade_gtk_box_original_positions = 
		(g_list_free (glade_gtk_box_original_positions), NULL);


	return TRUE;
}

void
glade_gtk_box_post_create (GladeWidgetAdaptor *adaptor,
			   GObject            *container, 
			   GladeCreateReason   reason)
{
	GladeWidget    *gwidget =
		glade_widget_get_from_gobject (container);

	/* Implement drag in GtkBox but not resize.
	 */
	g_object_set (gwidget,
		      "can-resize", FALSE,
		      NULL);

	g_signal_connect (G_OBJECT (gwidget), "configure-child",
			  G_CALLBACK (glade_gtk_box_configure_child), container);

	g_signal_connect (G_OBJECT (gwidget), "configure-begin",
			  G_CALLBACK (glade_gtk_box_configure_begin), container);

	g_signal_connect (G_OBJECT (gwidget), "configure-end",
			  G_CALLBACK (glade_gtk_box_configure_end), container);

}

static gint
sort_box_children (GtkWidget *widget_a, GtkWidget *widget_b)
{
	GtkWidget   *box;
	GladeWidget *gwidget_a, *gwidget_b;
	gint         position_a, position_b;

	gwidget_a = glade_widget_get_from_gobject (widget_a);
	gwidget_b = glade_widget_get_from_gobject (widget_b);

	box = gtk_widget_get_parent (widget_a);

	if (gwidget_a)
		glade_widget_pack_property_get
			(gwidget_a, "position", &position_a);
	else
		gtk_container_child_get (GTK_CONTAINER (box),
					 widget_a,
					 "position", &position_a,
					 NULL);

	if (gwidget_b)
		glade_widget_pack_property_get
			(gwidget_b, "position", &position_b);
	else
		gtk_container_child_get (GTK_CONTAINER (box),
					 widget_b,
					 "position", &position_b,
					 NULL);
	return position_a - position_b;
}

void
glade_gtk_box_set_child_property (GladeWidgetAdaptor *adaptor,
				  GObject            *container,
				  GObject            *child,
				  const gchar        *property_name,
				  GValue             *value)
{
	GladeWidget *gbox, *gchild, *gchild_iter;
	GList       *children, *list;
	gboolean     is_position;
	gint         old_position, iter_position, new_position;
	static       gboolean recursion = FALSE;
	
	g_return_if_fail (GTK_IS_BOX (container));
	g_return_if_fail (GTK_IS_WIDGET (child));
	g_return_if_fail (property_name != NULL || value != NULL);

	gbox   = glade_widget_get_from_gobject (container);
	gchild = glade_widget_get_from_gobject (child);

	g_return_if_fail (GLADE_IS_WIDGET (gbox));

	/* Get old position */
	if ((is_position = (strcmp (property_name, "position") == 0)) != FALSE)
	{
		gtk_container_child_get (GTK_CONTAINER (container),
					 GTK_WIDGET (child),
					 property_name, &old_position, NULL);

		
		/* Get the real value */
		new_position = g_value_get_int (value);

	}

	if (is_position && recursion == FALSE)
	{
		children = glade_widget_adaptor_get_children
			(gbox->adaptor, container);

		children = g_list_sort (children, (GCompareFunc)sort_box_children);

		for (list = children; list; list = list->next)
		{
			if ((gchild_iter = 
			     glade_widget_get_from_gobject (list->data)) == NULL)
				continue;

			if (gchild_iter == gchild)
			{
				gtk_box_reorder_child (GTK_BOX (container),
						       GTK_WIDGET (child),
						       new_position);
				continue;
			}

			/* Get the old value from glade */
			glade_widget_pack_property_get
				(gchild_iter, "position", &iter_position);

			/* Search for the child at the old position and update it */
			if (iter_position == new_position &&
			    glade_property_superuser () == FALSE)
			{
				/* Update glade with the real value */
				recursion = TRUE;
				glade_widget_pack_property_set
					(gchild_iter, "position", old_position);
				recursion = FALSE;
				continue;
			}
			else
			{
				gtk_box_reorder_child (GTK_BOX (container),
						       GTK_WIDGET (list->data),
						       iter_position);
			}
		}

		for (list = children; list; list = list->next)
		{
			if ((gchild_iter = 
			     glade_widget_get_from_gobject (list->data)) == NULL)
				continue;

			/* Refresh values yet again */
			glade_widget_pack_property_get
				(gchild_iter, "position", &iter_position);

			gtk_box_reorder_child (GTK_BOX (container),
					       GTK_WIDGET (list->data),
					       iter_position);

		}

		if (children)
			g_list_free (children);
	}

	/* Chain Up */
	if (!is_position)
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_set_property (adaptor, 
								  container, 
								  child,
								  property_name, 
								  value);

	gtk_container_check_resize (GTK_CONTAINER (container));

}


void
glade_gtk_box_get_property (GladeWidgetAdaptor *adaptor,
			    GObject            *object, 
			    const gchar        *id,
			    GValue             *value)
{
	if (!strcmp (id, "size"))
	{
		GtkBox *box = GTK_BOX (object);

		g_value_reset (value);
		g_value_set_int (value, g_list_length (box->children));
	}
	else 
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->get_property (adaptor, object, id, value);
}

static gint
glade_gtk_box_get_first_blank (GtkBox *box)
{
	GList       *child;
	GladeWidget *gwidget;
	gint         position;
	
	for (child = box->children, position = 0;
	     child && child->data;
	     child = child->next, position++)
	{
		GtkWidget *widget = ((GtkBoxChild *) (child->data))->widget;

		if ((gwidget = glade_widget_get_from_gobject (widget)) != NULL)
		{
			gint gwidget_position;
			GladeProperty *property =
				glade_widget_get_property (gwidget, "position");
			gwidget_position = g_value_get_int (property->value);

			if (gwidget_position > position)
				return position;
		}
	}
	return position;
}

static void
glade_gtk_box_set_size (GObject *object, const GValue *value)
{
	GtkBox      *box;
	GList       *child;
	guint new_size, old_size, i;

	box = GTK_BOX (object);
	g_return_if_fail (GTK_IS_BOX (box));

	old_size = g_list_length (box->children);
	new_size = g_value_get_int (value);
	
	if (old_size == new_size)
		return;

	/* Ensure placeholders first...
	 */
	for (i = 0; i < new_size; i++)
	{
		if (g_list_length(box->children) < (i + 1))
		{
			GtkWidget *placeholder = glade_placeholder_new ();
			gint       blank       = glade_gtk_box_get_first_blank (box);

			gtk_container_add (GTK_CONTAINER (box), placeholder);
			gtk_box_reorder_child (box, placeholder, blank);
		}
	}

	/* The box has shrunk. Remove the widgets that are on those slots */
	for (child = g_list_last (box->children);
	     child && old_size > new_size;
	     child = g_list_last (box->children), old_size--)
	{
		GtkWidget *child_widget = ((GtkBoxChild *) (child->data))->widget;

		/* Refuse to remove any widgets that are either GladeWidget objects
		 * or internal to the hierarchic entity (may be a composite widget,
		 * not all internal widgets have GladeWidgets).
		 */
		if (glade_widget_get_from_gobject (child_widget) ||
		    GLADE_IS_PLACEHOLDER (child_widget) == FALSE)
			break;

		g_object_ref (G_OBJECT (child_widget));
		gtk_container_remove (GTK_CONTAINER (box), child_widget);
		gtk_widget_destroy (child_widget);
	}
}

void
glade_gtk_box_set_property (GladeWidgetAdaptor *adaptor,
			    GObject            *object, 
			    const gchar        *id,
			    const GValue       *value)
{
	if (!strcmp (id, "size"))
		glade_gtk_box_set_size (object, value);
	else 
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor, object, id, value);
}

static gboolean
glade_gtk_box_verify_size (GObject *object, const GValue *value)
{
	GtkBox *box = GTK_BOX(object);
	GList  *child;
	gint    old_size = g_list_length (box->children);
	gint    new_size = g_value_get_int (value);

	for (child = g_list_last (box->children);
	     child && old_size > new_size;
	     child = g_list_previous (child), old_size--)
	{
		GtkWidget *widget = ((GtkBoxChild *) (child->data))->widget;
		if (glade_widget_get_from_gobject (widget) != NULL)
			/* In this case, refuse to shrink */
			return FALSE;
	}
	return new_size >= 0;
}


gboolean
glade_gtk_box_verify_property (GladeWidgetAdaptor *adaptor,
			       GObject            *object, 
			       const gchar        *id,
			       const GValue       *value)
{
	if (!strcmp (id, "size"))
		return glade_gtk_box_verify_size (object, value);
	else if (GWA_GET_CLASS (GTK_TYPE_CONTAINER)->verify_property)
		return GWA_GET_CLASS (GTK_TYPE_CONTAINER)->verify_property (adaptor, object, 
									    id, value);

	return TRUE;
}

void
glade_gtk_box_add_child (GladeWidgetAdaptor *adaptor,
			 GObject            *object,
			 GObject            *child)
{
	GladeWidget  *gbox, *gchild;
	GladeProject *project;
	gint          num_children;
	
	g_return_if_fail (GTK_IS_BOX (object));
	g_return_if_fail (GTK_IS_WIDGET (child));
	
	gbox = glade_widget_get_from_gobject (object);
	project = glade_widget_get_project (gbox);

	/*
	  Try to remove the last placeholder if any, this way GtkBox`s size 
	  will not be changed.
	*/
	if (glade_widget_superuser () == FALSE &&
	    !GLADE_IS_PLACEHOLDER (child))
	{
		GList *l;
		GtkBox *box = GTK_BOX (object);
		
		for (l = g_list_last (box->children); l; l = g_list_previous (l))
		{
			GtkWidget *child_widget = ((GtkBoxChild *) (l->data))->widget;			
			if (GLADE_IS_PLACEHOLDER (child_widget))
			{
				gtk_container_remove (GTK_CONTAINER (box), child_widget);
				break;
			}
		}
	}

	gtk_container_add (GTK_CONTAINER (object), GTK_WIDGET (child));
	
	num_children = g_list_length (GTK_BOX (object)->children);
	glade_widget_property_set (gbox, "size", num_children);
	
	gchild = glade_widget_get_from_gobject (child);

	/* The "Remove Slot" operation only makes sence on placeholders,
	 * otherwise its a "Delete" operation on the child widget.
	 */
	if (gchild)
		glade_widget_remove_pack_action (gchild, "remove_slot");
	
	/* Packing props arent around when parenting during a glade_widget_dup() */
	if (gchild && gchild->packing_properties)
		glade_widget_pack_property_set (gchild, "position", num_children - 1);
}

void
glade_gtk_box_remove_child (GladeWidgetAdaptor *adaptor,
			    GObject            *object, 
			    GObject            *child)
{
	GladeWidget *gbox;
	gint size;
	
	g_return_if_fail (GTK_IS_BOX (object));
	g_return_if_fail (GTK_IS_WIDGET (child));
	
	gbox = glade_widget_get_from_gobject (object);
	
	gtk_container_remove (GTK_CONTAINER (object), GTK_WIDGET (child));

	if (glade_widget_superuser () == FALSE)
	{
		glade_widget_property_get (gbox, "size", &size);
		glade_widget_property_set (gbox, "size", size);
	}
}


void
glade_gtk_box_replace_child (GladeWidgetAdaptor *adaptor,
			     GObject            *container,
			     GObject            *current,
			     GObject            *new_widget)
{
	GladeWidget  *gchild;

	GWA_GET_CLASS (GTK_TYPE_CONTAINER)->replace_child (adaptor,
							   container,
							   current,
							   new_widget);

	if ((gchild = glade_widget_get_from_gobject (new_widget)) != NULL)
		/* The "Remove Slot" operation only makes sence on placeholders,
		 * otherwise its a "Delete" operation on the child widget.
		 */
		glade_widget_remove_pack_action (gchild, "remove_slot");

}


GObject *
glade_gtk_box_get_internal_child (GladeWidgetAdaptor *adaptor,
				  GObject            *object, 
				  const gchar        *name)
{
	GList   *children, *l;
	GObject *child = NULL;	

	g_return_val_if_fail (GTK_IS_BOX (object), NULL);
	
	children = l = gtk_container_get_children (GTK_CONTAINER (object));
	
	while (l)
	{
		GladeWidget *gw = glade_widget_get_from_gobject (l->data);
		
		if (gw && gw->internal && strcmp (gw->internal, name) == 0)
		{
			child = G_OBJECT (l->data);
			break;
		}
		
		l= l->next;
	}
	g_list_free (children);

	return child;
}

static void
glade_gtk_box_notebook_child_insert_remove_action (GladeWidgetAdaptor *adaptor,
						   GObject *container,
						   GObject *object,
						   const gchar *size_prop,
						   const gchar *group_format,
						   gboolean remove,
						   gboolean after);

void
glade_gtk_box_child_action_activate (GladeWidgetAdaptor *adaptor,
				     GObject            *container,
				     GObject            *object,
				     const gchar        *action_path)
{
	if (strcmp (action_path, "insert_after") == 0)
	{
		glade_gtk_box_notebook_child_insert_remove_action (adaptor, container,
								   object, "size",
								   _("Insert placeholder to %s"),
								   FALSE, TRUE);
	}
	else if (strcmp (action_path, "insert_before") == 0)
	{
		glade_gtk_box_notebook_child_insert_remove_action (adaptor, container,
								   object, "size",
								   _("Insert placeholder to %s"),
								   FALSE, FALSE);
	}
	else if (strcmp (action_path, "remove_slot") == 0)
	{
		glade_gtk_box_notebook_child_insert_remove_action (adaptor, container,
								   object, "size",
								   _("Remove placeholder from %s"),
								   TRUE, FALSE);
	}
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->child_action_activate (adaptor,
									   container,
									   object,
									   action_path);
}

/* ----------------------------- GtkTable ------------------------------ */
typedef struct {
	/* comparable part: */
	GladeWidget *widget;
	gint         left_attach;
	gint         right_attach;
	gint         top_attach;
	gint         bottom_attach;
} GladeGtkTableChild;

typedef enum {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
} GladeTableDir;

#define TABLE_CHILD_CMP_SIZE (sizeof (GladeWidget *) + (sizeof (gint) * 4))

static GladeGtkTableChild table_edit = { 0, };
static GladeGtkTableChild table_cur_attach = { 0, };


/* Takes a point (x or y depending on 'row') relative to
 * table, and returns the row or column in which the point
 * was found.
 */
static gint
glade_gtk_table_get_row_col_from_point (GtkTable *table,
					gboolean  row,
					gint      point)
{
	GtkTableChild *tchild;
	GList         *list;
	gint           span, trans_point, size, base, end;

	for (list = table->children; list; list = list->next)
	{
		tchild = list->data;

		if (row)
			gtk_widget_translate_coordinates 
				(GTK_WIDGET (table), tchild->widget,
				 0, point, NULL, &trans_point);
		else
			gtk_widget_translate_coordinates
				(GTK_WIDGET (table), tchild->widget,
				 point, 0, &trans_point, NULL);
		
		/* Find any widget in our row/column
		 */
		end = row ? 
			tchild->widget->allocation.height :
			tchild->widget->allocation.width;

		if (trans_point >= 0 &&
		    /* should be trans_point < end ... test FIXME ! */
		    trans_point <  end)
		{
			base = row ? tchild->top_attach : tchild->left_attach;
			size = row ? (tchild->widget->allocation.height) :
				(tchild->widget->allocation.width);
			span = row ? (tchild->bottom_attach - tchild->top_attach) :
				(tchild->right_attach - tchild->left_attach);

			return base + (trans_point * span / size);
		}
	}

	return -1;
}


static gboolean
glade_gtk_table_point_crosses_threshold (GtkTable      *table,
					 gboolean       row,
					 gint           num,
					 GladeTableDir  dir,
					 gint           point)
{

	GtkTableChild *tchild;
	GList         *list;
	gint           span, trans_point, size, rowcol_size, base;
	
	for (list = table->children; list; list = list->next)
	{
		tchild = list->data;
	
		/* Find any widget in our row/column
		 */
		if ((row && num >= tchild->top_attach && num < tchild->bottom_attach) ||
		    (!row && num >= tchild->left_attach && num < tchild->right_attach))
		{

			if (row)
				gtk_widget_translate_coordinates 
					(GTK_WIDGET (table), tchild->widget,
					 0, point, NULL, &trans_point);
			else
				gtk_widget_translate_coordinates
					(GTK_WIDGET (table), tchild->widget,
					 point, 0, &trans_point, NULL);

			span = row ? (tchild->bottom_attach - tchild->top_attach) :
				(tchild->right_attach - tchild->left_attach);
			size = row ? (tchild->widget->allocation.height) :
				(tchild->widget->allocation.width);

			base         = row ? tchild->top_attach : tchild->left_attach;
			rowcol_size  = size / span;
			trans_point -= (num - base) * rowcol_size;

#if 0
			g_print ("dir: %s, widget size: %d, rowcol size: %d, "
				 "requested rowcol: %d, widget base rowcol: %d, trim: %d, "
				 "widget point: %d, thresh: %d\n",
				 dir == DIR_UP ? "up" : dir == DIR_DOWN ? "down" :
				 dir == DIR_LEFT ? "left" : "right",
				 size, rowcol_size, num, base, (num - base) * rowcol_size,
				 trans_point,
				 dir == DIR_UP || dir == DIR_LEFT ?
				 (rowcol_size / 2) :
				 (rowcol_size / 2));
#endif
			switch (dir)
			{
			case DIR_UP:
			case DIR_LEFT:
				return trans_point <= (rowcol_size / 2);
			case DIR_DOWN:
			case DIR_RIGHT:
				return trans_point >= (rowcol_size / 2);
			default:
				break;
			}
		}
		
	}
	return FALSE;
}

static gboolean
glade_gtk_table_get_attachments (GladeFixed         *fixed,
				 GtkTable           *table,
				 GdkRectangle       *rect,
				 GladeGtkTableChild *configure)
{
	gint  center_x, center_y, row, column;
	center_x  = rect->x + (rect->width / 2);
	center_y  = rect->y + (rect->height / 2);

	column = glade_gtk_table_get_row_col_from_point
		(table, FALSE, center_x);

	row = glade_gtk_table_get_row_col_from_point
		(table, TRUE, center_y);

	/* its a start, now try to grow when the rect extents
	 * reach at least half way into the next row/column 
	 */
	configure->left_attach      = column;
	configure->right_attach     = column + 1;
	configure->top_attach       = row;
	configure->bottom_attach    = row +1;

	if (column >= 0 && row >= 0)
	{

		/* Check and expand left
		 */
		while (configure->left_attach > 0)
		{
			if (rect->x < fixed->child_x_origin &&
			    fixed->operation != GLADE_CURSOR_DRAG &&
			    GLADE_FIXED_CURSOR_LEFT (fixed->operation) == FALSE)
				break;

			if (glade_gtk_table_point_crosses_threshold 
			    (table, FALSE, configure->left_attach -1,
			     DIR_LEFT, rect->x) == FALSE)
				break;

			configure->left_attach--;
		}

		/* Check and expand right
		 */
		while (configure->right_attach < (table->ncols))
		{
			if (rect->x + rect->width >
			    fixed->child_x_origin + fixed->child_width_origin &&
			    fixed->operation != GLADE_CURSOR_DRAG &&
			    GLADE_FIXED_CURSOR_RIGHT (fixed->operation) == FALSE)
				break;

			if (glade_gtk_table_point_crosses_threshold
			    (table, FALSE, configure->right_attach,
			     DIR_RIGHT, rect->x + rect->width) == FALSE)
				break;

			configure->right_attach++;
		}

		/* Check and expand top
		 */
		while (configure->top_attach > 0)
		{
			if (rect->y < fixed->child_y_origin &&
			    fixed->operation != GLADE_CURSOR_DRAG &&
			    GLADE_FIXED_CURSOR_TOP (fixed->operation) == FALSE)
				break;

			if (glade_gtk_table_point_crosses_threshold 
			    (table, TRUE, configure->top_attach -1,
			     DIR_UP, rect->y) == FALSE)
				break;

			configure->top_attach--;
		}

		/* Check and expand bottom
		 */
		while (configure->bottom_attach < (table->nrows))
		{
			if (rect->y + rect->height >
			    fixed->child_y_origin + fixed->child_height_origin &&
			    fixed->operation != GLADE_CURSOR_DRAG &&
			    GLADE_FIXED_CURSOR_BOTTOM (fixed->operation) == FALSE)
				break;

			if (glade_gtk_table_point_crosses_threshold
			    (table, TRUE, configure->bottom_attach,
			     DIR_DOWN, rect->y + rect->height) == FALSE)
				break;

			configure->bottom_attach++;
		}
	}

	/* Keep the same row/col span when performing a drag
	 */
	if (fixed->operation == GLADE_CURSOR_DRAG)
	{
		gint col_span =	table_edit.right_attach - table_edit.left_attach;
		gint row_span = table_edit.bottom_attach - table_edit.top_attach;

		if (rect->x < fixed->child_x_origin)
			configure->right_attach = configure->left_attach + col_span;
		else
			configure->left_attach = configure->right_attach - col_span;

		if (rect->y < fixed->child_y_origin)
			configure->bottom_attach = configure->top_attach + row_span;
		else
			configure->top_attach = configure->bottom_attach - row_span;
	} else if (fixed->operation == GLADE_CURSOR_RESIZE_RIGHT) {
		configure->left_attach   = table_edit.left_attach;
		configure->top_attach    = table_edit.top_attach;
		configure->bottom_attach = table_edit.bottom_attach;
	} else if (fixed->operation == GLADE_CURSOR_RESIZE_LEFT) {
		configure->right_attach  = table_edit.right_attach;
		configure->top_attach    = table_edit.top_attach;
		configure->bottom_attach = table_edit.bottom_attach;
	} else if (fixed->operation == GLADE_CURSOR_RESIZE_TOP) {
		configure->left_attach   = table_edit.left_attach;
		configure->right_attach  = table_edit.right_attach;
		configure->bottom_attach = table_edit.bottom_attach;
	} else if (fixed->operation == GLADE_CURSOR_RESIZE_BOTTOM) {
		configure->left_attach   = table_edit.left_attach;
		configure->right_attach  = table_edit.right_attach;
		configure->top_attach    = table_edit.top_attach;
	}

	return column >= 0 && row >= 0;
}

static gboolean
glade_gtk_table_configure_child (GladeFixed   *fixed,
				 GladeWidget  *child,
				 GdkRectangle *rect,
				 GtkWidget    *table)
{
	GladeGtkTableChild configure = { child, };

	/* Sometimes we are unable to find a widget in the appropriate column,
	 * usually because a placeholder hasnt had its size allocation yet.
	 */
	if (glade_gtk_table_get_attachments (fixed, GTK_TABLE (table), rect, &configure))
	{
		if (memcmp (&configure, &table_cur_attach, TABLE_CHILD_CMP_SIZE) != 0)
		{

			glade_property_push_superuser ();
			glade_widget_pack_property_set (child, "left-attach",   
							configure.left_attach);
			glade_widget_pack_property_set (child, "right-attach",  
							configure.right_attach);
			glade_widget_pack_property_set (child, "top-attach",    
							configure.top_attach);
			glade_widget_pack_property_set (child, "bottom-attach", 
							configure.bottom_attach);
			glade_property_pop_superuser ();

			memcpy (&table_cur_attach, &configure, TABLE_CHILD_CMP_SIZE);
		}
	}
	return TRUE;
}


static gboolean
glade_gtk_table_configure_begin (GladeFixed  *fixed,
				 GladeWidget *child,
				 GtkWidget   *table)
{

	table_edit.widget = child;

	glade_widget_pack_property_get (child, "left-attach", 
					&table_edit.left_attach);
	glade_widget_pack_property_get (child, "right-attach", 
					&table_edit.right_attach);
	glade_widget_pack_property_get (child, "top-attach", 
					&table_edit.top_attach);
	glade_widget_pack_property_get (child, "bottom-attach", 
					&table_edit.bottom_attach);

	memcpy (&table_cur_attach, &table_edit, TABLE_CHILD_CMP_SIZE);

	return TRUE;
}

static gboolean
glade_gtk_table_configure_end (GladeFixed  *fixed,
			       GladeWidget *child,
			       GtkWidget   *table)
{
	GladeGtkTableChild new_child = { child, };

	glade_widget_pack_property_get (child, "left-attach", 
					&new_child.left_attach);
	glade_widget_pack_property_get (child, "right-attach", 
					&new_child.right_attach);
	glade_widget_pack_property_get (child, "top-attach", 
					&new_child.top_attach);
	glade_widget_pack_property_get (child, "bottom-attach", 
					&new_child.bottom_attach);

	/* Compare the meaningfull part of the current edit. */
	if (memcmp (&new_child, &table_edit, TABLE_CHILD_CMP_SIZE) != 0)
	{
		GValue left_attach_value   = { 0, };
		GValue right_attach_value = { 0, };
		GValue top_attach_value    = { 0, };
		GValue bottom_attach_value = { 0, };

		GValue new_left_attach_value   = { 0, };
		GValue new_right_attach_value = { 0, };
		GValue new_top_attach_value    = { 0, };
		GValue new_bottom_attach_value = { 0, };

		GladeProperty *left_attach_prop, *right_attach_prop, 
			*top_attach_prop, *bottom_attach_prop;

		left_attach_prop   = glade_widget_get_pack_property (child, "left-attach");
		right_attach_prop  = glade_widget_get_pack_property (child, "right-attach");
		top_attach_prop    = glade_widget_get_pack_property (child, "top-attach");
		bottom_attach_prop = glade_widget_get_pack_property (child, "bottom-attach");

		g_return_val_if_fail (GLADE_IS_PROPERTY (left_attach_prop), FALSE);
		g_return_val_if_fail (GLADE_IS_PROPERTY (right_attach_prop), FALSE);
		g_return_val_if_fail (GLADE_IS_PROPERTY (top_attach_prop), FALSE);
		g_return_val_if_fail (GLADE_IS_PROPERTY (bottom_attach_prop), FALSE);

		glade_property_get_value (left_attach_prop,   &new_left_attach_value);
		glade_property_get_value (right_attach_prop,  &new_right_attach_value);
		glade_property_get_value (top_attach_prop,    &new_top_attach_value);
		glade_property_get_value (bottom_attach_prop, &new_bottom_attach_value);

		g_value_init (&left_attach_value, G_TYPE_UINT);
		g_value_init (&right_attach_value, G_TYPE_UINT);
		g_value_init (&top_attach_value, G_TYPE_UINT);
		g_value_init (&bottom_attach_value, G_TYPE_UINT);

		g_value_set_uint (&left_attach_value,   table_edit.left_attach);
		g_value_set_uint (&right_attach_value,  table_edit.right_attach);
		g_value_set_uint (&top_attach_value,    table_edit.top_attach);
		g_value_set_uint (&bottom_attach_value, table_edit.bottom_attach);

		glade_command_push_group (_("Placing %s inside %s"), 
					  child->name,
					  GLADE_WIDGET (fixed)->name);
		glade_command_set_properties
			(left_attach_prop,   &left_attach_value,   &new_left_attach_value,
			 right_attach_prop,  &right_attach_value,  &new_right_attach_value,
			 top_attach_prop,    &top_attach_value,    &new_top_attach_value,
			 bottom_attach_prop, &bottom_attach_value, &new_bottom_attach_value,
			 NULL);
		glade_command_pop_group ();

		g_value_unset (&left_attach_value);
		g_value_unset (&right_attach_value);
		g_value_unset (&top_attach_value);
		g_value_unset (&bottom_attach_value);
		g_value_unset (&new_left_attach_value);
		g_value_unset (&new_right_attach_value);
		g_value_unset (&new_top_attach_value);
		g_value_unset (&new_bottom_attach_value);
	}

	return TRUE;
}

void
glade_gtk_table_post_create (GladeWidgetAdaptor *adaptor,
			     GObject            *container, 
			     GladeCreateReason   reason)
{
	GladeWidget    *gwidget =
		glade_widget_get_from_gobject (container);
	
	g_signal_connect (G_OBJECT (gwidget), "configure-child",
			  G_CALLBACK (glade_gtk_table_configure_child), container);

	g_signal_connect (G_OBJECT (gwidget), "configure-begin",
			  G_CALLBACK (glade_gtk_table_configure_begin), container);

	g_signal_connect (G_OBJECT (gwidget), "configure-end",
			  G_CALLBACK (glade_gtk_table_configure_end), container);
}

static gboolean
glade_gtk_table_has_child (GtkTable *table, guint left_attach, guint top_attach)
{
	GList *list;

	for (list = table->children; list && list->data; list = list->next)
	{
		GtkTableChild *child = list->data;
		
		if (left_attach >= child->left_attach && left_attach < child->right_attach &&
		    top_attach >= child->top_attach && top_attach < child->bottom_attach)
			return TRUE;
	}
	return FALSE;
}

static gboolean
glade_gtk_table_widget_exceeds_bounds (GtkTable *table, gint n_rows, gint n_cols)
{
	GList *list;
	for (list = table->children; list && list->data; list = list->next)
	{
		GtkTableChild *child = list->data;
		if (GLADE_IS_PLACEHOLDER(child->widget) == FALSE &&
		    (child->right_attach  > n_cols ||
		     child->bottom_attach > n_rows))
			return TRUE;
	}
	return FALSE;
}

static void
glade_gtk_table_refresh_placeholders (GtkTable *table)
{
	GList *list, *toremove = NULL;
	gint i, j;

	for (list = table->children; list && list->data; list = list->next)
	{
		GtkTableChild *child = list->data;
		
		if (GLADE_IS_PLACEHOLDER (child->widget))
			toremove = g_list_prepend (toremove, child->widget);
	}

	if (toremove)
	{
		for (list = toremove; list; list = list->next)
			gtk_container_remove (GTK_CONTAINER (table),
					      GTK_WIDGET (list->data));
		g_list_free (toremove);
	}

	for (i = 0; i < table->ncols; i++)
		for (j = 0; j < table->nrows; j++)
			if (glade_gtk_table_has_child (table, i, j) == FALSE)
				gtk_table_attach_defaults (table,
							   glade_placeholder_new (),
					 		   i, i + 1, j, j + 1);
	
	gtk_container_check_resize (GTK_CONTAINER (table));
}


void
glade_gtk_table_add_child (GladeWidgetAdaptor *adaptor,
			   GObject            *object, 
			   GObject            *child)
{
	g_return_if_fail (GTK_IS_TABLE (object));
	g_return_if_fail (GTK_IS_WIDGET (child));

	gtk_container_add (GTK_CONTAINER (object), GTK_WIDGET (child));

	glade_gtk_table_refresh_placeholders (GTK_TABLE (object));
}

void
glade_gtk_table_remove_child (GladeWidgetAdaptor *adaptor,
			      GObject            *object, 
			      GObject            *child)
{
	g_return_if_fail (GTK_IS_TABLE (object));
	g_return_if_fail (GTK_IS_WIDGET (child));
	
	gtk_container_remove (GTK_CONTAINER (object), GTK_WIDGET (child));
	
	glade_gtk_table_refresh_placeholders (GTK_TABLE (object));
}

void
glade_gtk_table_replace_child (GladeWidgetAdaptor *adaptor,
			       GtkWidget          *container,
			       GtkWidget          *current,
			       GtkWidget          *new_widget)
{
	g_return_if_fail (GTK_IS_TABLE (container));
	g_return_if_fail (GTK_IS_WIDGET (current));
	g_return_if_fail (GTK_IS_WIDGET (new_widget));
	
	/* Chain Up */
	GWA_GET_CLASS
		(GTK_TYPE_CONTAINER)->replace_child (adaptor, 
						     G_OBJECT (container), 
						     G_OBJECT (current), 
						     G_OBJECT (new_widget));
	
	/* If we are replacing a GladeWidget, we must refresh placeholders
	 * because the widget may have spanned multiple rows/columns, we must
	 * not do so in the case we are pasting multiple widgets into a table,
	 * where destroying placeholders results in default packing properties
	 * (since the remaining placeholder templates no longer exist, only the
	 * first pasted widget would have proper packing properties).
	 */
	if (glade_widget_get_from_gobject (new_widget) == FALSE)
		glade_gtk_table_refresh_placeholders (GTK_TABLE (container));

}

static void
glade_gtk_table_set_n_common (GObject *object, const GValue *value, gboolean for_rows)
{
	GladeWidget *widget;
	GtkTable    *table;
	guint new_size, old_size;

	table = GTK_TABLE (object);
	g_return_if_fail (GTK_IS_TABLE (table));

	new_size = g_value_get_uint (value);
	old_size = for_rows ? table->nrows : table->ncols;

	if (new_size < 1)
		return;

	if (glade_gtk_table_widget_exceeds_bounds
	    (table,
	     for_rows ? new_size : table->nrows,
	     for_rows ? table->ncols : new_size))
		/* Refuse to shrink if it means orphaning widgets */
		return;

	widget = glade_widget_get_from_gobject (GTK_WIDGET (table));
	g_return_if_fail (widget != NULL);

	if (for_rows)
		gtk_table_resize (table, new_size, table->ncols);
	else
		gtk_table_resize (table, table->nrows, new_size);

	/* Fill table with placeholders */
	glade_gtk_table_refresh_placeholders (table);
	
	if (new_size < old_size)
	{
		/* Remove from the bottom up */
		GList *list;
		GList *list_to_free = NULL;

		for (list = table->children; list && list->data; list = list->next)
		{
			GtkTableChild *child = list->data;
			guint start = for_rows ? child->top_attach : child->left_attach;
			guint end = for_rows ? child->bottom_attach : child->right_attach;

			/* We need to completely remove it */
			if (start >= new_size)
			{
				list_to_free = g_list_prepend (list_to_free, child->widget);
				continue;
			}

			/* If the widget spans beyond the new border,
			 * we should resize it to fit on the new table */
			if (end > new_size)
				gtk_container_child_set
					(GTK_CONTAINER (table), GTK_WIDGET (child),
					 for_rows ? "bottom_attach" : "right_attach",
					 new_size, NULL);
		}

		if (list_to_free)
		{
			for (list = g_list_first(list_to_free);
			     list && list->data;
			     list = list->next)
			{
				g_object_ref (G_OBJECT (list->data));
				gtk_container_remove (GTK_CONTAINER (table),
						      GTK_WIDGET(list->data));
				/* This placeholder is no longer valid, force destroy */
				gtk_widget_destroy (GTK_WIDGET(list->data));
			}
			g_list_free (list_to_free);
		}
		gtk_table_resize (table,
				  for_rows ? new_size : table->nrows,
				  for_rows ? table->ncols : new_size);
	}
}

void
glade_gtk_table_set_property (GladeWidgetAdaptor *adaptor,
			      GObject            *object, 
			      const gchar        *id,
			      const GValue       *value)
{
	if (!strcmp (id, "n-rows"))
		glade_gtk_table_set_n_common (object, value, TRUE);
	else if (!strcmp (id, "n-columns"))
		glade_gtk_table_set_n_common (object, value, FALSE);
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor, object,
								  id, value);
}

static gboolean 
glade_gtk_table_verify_n_common (GObject *object, const GValue *value, gboolean for_rows)
{
	GtkTable *table = GTK_TABLE(object);
	guint new_size = g_value_get_uint (value);

	if (glade_gtk_table_widget_exceeds_bounds
	    (table,
	     for_rows ? new_size : table->nrows,
	     for_rows ? table->ncols : new_size))
		/* Refuse to shrink if it means orphaning widgets */
		return FALSE;

	return TRUE;
}

gboolean
glade_gtk_table_verify_property (GladeWidgetAdaptor *adaptor,
				 GObject            *object, 
				 const gchar        *id,
				 const GValue       *value)
{
	if (!strcmp (id, "n-rows"))
		return glade_gtk_table_verify_n_common (object, value, TRUE);
	else if (!strcmp (id, "n-columns"))
		return glade_gtk_table_verify_n_common (object, value, FALSE);
	else if (GWA_GET_CLASS (GTK_TYPE_CONTAINER)->verify_property)
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->verify_property (adaptor, object,
								     id, value);

	return TRUE;
}

void
glade_gtk_table_set_child_property (GladeWidgetAdaptor *adaptor,
				    GObject            *container,
				    GObject            *child,
				    const gchar        *property_name,
				    GValue             *value)
{
	g_return_if_fail (GTK_IS_TABLE (container));
	g_return_if_fail (GTK_IS_WIDGET (child));
	g_return_if_fail (property_name != NULL && value != NULL);

	GWA_GET_CLASS
		(GTK_TYPE_CONTAINER)->child_set_property (adaptor, 
							  container, child,
							  property_name, value);
	
	if (strcmp (property_name, "bottom-attach") == 0 ||
	    strcmp (property_name, "left-attach") == 0 ||
	    strcmp (property_name, "right-attach") == 0 ||
	    strcmp (property_name, "top-attach") == 0)
	{
		/* Refresh placeholders */
		glade_gtk_table_refresh_placeholders (GTK_TABLE (container));
	}

}

static gboolean
glade_gtk_table_verify_attach_common (GObject *object,
				      GValue *value,
				      guint *val,
				      const gchar *prop,
				      guint *prop_val,
				      const gchar *parent_prop,
				      guint *parent_val)
{
	GladeWidget *widget, *parent;
	
	widget = glade_widget_get_from_gobject (object);
	g_return_val_if_fail (GLADE_IS_WIDGET (widget), TRUE);
	parent = glade_widget_get_parent (widget);
	g_return_val_if_fail (GLADE_IS_WIDGET (parent), TRUE);
	
	*val = g_value_get_uint (value);
	glade_widget_property_get (widget, prop, prop_val);
	glade_widget_property_get (parent, parent_prop, parent_val);

	return FALSE;
}

static gboolean
glade_gtk_table_verify_left_top_attach (GObject *object,
					GValue *value,
					const gchar *prop,
					const gchar *parent_prop)
{
	guint val, prop_val, parent_val;
	
	if (glade_gtk_table_verify_attach_common (object, value, &val,
						  prop, &prop_val,
						  parent_prop, &parent_val))
		return FALSE;
	
	if (val >= parent_val || val >= prop_val) 
		return FALSE;
		
	return TRUE;
}

static gboolean
glade_gtk_table_verify_right_bottom_attach (GObject *object,
					    GValue *value,
					    const gchar *prop,
					    const gchar *parent_prop)
{
	guint val, prop_val, parent_val;
	
	if (glade_gtk_table_verify_attach_common (object, value, &val,
						  prop, &prop_val,
						  parent_prop, &parent_val))
		return FALSE;
	
	if (val <= prop_val || val > parent_val) 
		return FALSE;
		
	return TRUE;
}

gboolean
glade_gtk_table_child_verify_property (GladeWidgetAdaptor *adaptor,
				       GObject            *container,
				       GObject            *child,
				       const gchar        *id,
				       GValue             *value)
{
	if (!strcmp (id, "left-attach"))
		return glade_gtk_table_verify_left_top_attach (child, 
							       value,
							       "right-attach",
							       "n-columns");
	else if (!strcmp (id, "right-attach"))
		return glade_gtk_table_verify_right_bottom_attach (child,
								   value,
								   "left-attach",
								   "n-columns");
	else if (!strcmp (id, "top-attach"))
		return glade_gtk_table_verify_left_top_attach (child, 
							       value,
							       "bottom-attach",
							       "n-rows");
	else if (!strcmp (id, "bottom-attach"))
		return glade_gtk_table_verify_right_bottom_attach (child,
								   value,
								   "top-attach",
								   "n-rows");
	else if (GWA_GET_CLASS (GTK_TYPE_CONTAINER)->child_verify_property)
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_verify_property (adaptor, 
								     container, child,
								     id, value);

	return TRUE;
}

static void
glade_gtk_table_child_insert_remove_action (GladeWidgetAdaptor *adaptor,
					    GObject *container,
					    GObject *object,
					    const gchar *group_format,
					    const gchar *n_row_col,
					    const gchar *attach1,
					    const gchar *attach2,
					    gboolean remove,
					    gboolean after)
{
	GladeWidget *parent;
	GList *children, *l;
	gint child_pos, size, offset;
	
	gtk_container_child_get (GTK_CONTAINER (container),
				 GTK_WIDGET (object),
				 attach1, &child_pos, NULL);
	
	parent = glade_widget_get_from_gobject (container);
	glade_command_push_group (group_format, glade_widget_get_name (parent));
		
	children = glade_widget_adaptor_get_children (adaptor, container);
	/* Make sure widgets does not get destroyed */
	g_list_foreach (children, (GFunc) g_object_ref, NULL);
	
	glade_widget_property_get (parent, n_row_col, &size);
	
	if (remove)
	{
		GList *del = NULL;
		/* Remove children first */
		for (l = children; l; l = g_list_next (l))
		{
			GladeWidget *gchild = glade_widget_get_from_gobject (l->data);
			gint pos;
			
			/* Skip placeholders */
			if (gchild == NULL) continue;
		
			glade_widget_pack_property_get (gchild, attach1, &pos);
			if (pos == child_pos) del = g_list_prepend (del, gchild);
		}
		if (del)
		{
			glade_command_delete (del);
			g_list_free (del);
		}
		offset = -1;
	}
	else
	{
		/* Expand the table */
		glade_command_set_property (glade_widget_get_property (parent, n_row_col),
					    size + 1);
		offset = 1;
	}
	
	/* Reorder children */
	for (l = children; l; l = g_list_next (l))
	{
		GladeWidget *gchild = glade_widget_get_from_gobject (l->data);
		gint pos, pos2;
			
		/* Skip placeholders */
		if (gchild == NULL) continue;
		
		glade_widget_pack_property_get (gchild, attach1, &pos);
		if ((after) ? pos > child_pos : pos >= child_pos)
		{
			glade_widget_pack_property_get (gchild, attach2, &pos2);
			glade_command_set_property (glade_widget_get_pack_property (gchild, attach1),
						    pos + offset);
			glade_command_set_property (glade_widget_get_pack_property (gchild, attach2),
						    pos2 + offset);
		}
	}
	
	if (remove)
	{
		/* Shrink the table */
		glade_command_set_property (glade_widget_get_property (parent, n_row_col),
					    size - 1);
	}
	
	g_list_foreach (children, (GFunc) g_object_unref, NULL);
	g_list_free (children);
	
	glade_command_pop_group ();
}

void
glade_gtk_table_child_action_activate (GladeWidgetAdaptor *adaptor,
				       GObject            *container,
				       GObject            *object,
				       const gchar        *action_path)
{
	if (strcmp (action_path, "insert_row/after") == 0)
	{
		glade_gtk_table_child_insert_remove_action (adaptor, container, object,
							    _("Insert Row on %s"),
							    "n-rows","top-attach",
							    "bottom-attach",
							    FALSE, TRUE);
	}
	else if (strcmp (action_path, "insert_row/before") == 0)
	{
		glade_gtk_table_child_insert_remove_action (adaptor, container, object,
							    _("Insert Row on %s"),
							    "n-rows","top-attach",
							    "bottom-attach",
							    FALSE, FALSE);
	}
	else if (strcmp (action_path, "insert_column/after") == 0)
	{
		glade_gtk_table_child_insert_remove_action (adaptor, container, object,
							    _("Insert Column on %s"),
							    "n-columns","right-attach",
							    "left-attach",
							    FALSE, TRUE);
	}
	else if (strcmp (action_path, "insert_column/before") == 0)
	{
		glade_gtk_table_child_insert_remove_action (adaptor, container, object,
							    _("Insert Column on %s"),
							    "n-columns","right-attach",
							    "left-attach",
							    FALSE, FALSE);
	}
	else if (strcmp (action_path, "remove_column") == 0)
	{
		glade_gtk_table_child_insert_remove_action (adaptor, container, object,
							    _("Remove Column on %s"),
							    "n-columns","right-attach",
							    "left-attach",
							    TRUE, TRUE);
	}
	else if (strcmp (action_path, "remove_row") == 0)
	{
		glade_gtk_table_child_insert_remove_action (adaptor, container, object,
							    _("Remove Row on %s"),
							    "n-rows","top-attach",
							    "bottom-attach",
							    TRUE, TRUE);
	}
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->child_action_activate (adaptor,
									   container,
									   object,
									   action_path);
}

/* ----------------------------- GtkFrame ------------------------------ */
void
glade_gtk_frame_post_create (GladeWidgetAdaptor *adaptor,
			     GObject            *frame, 
			     GladeCreateReason   reason)
{
	static GladeWidgetAdaptor *label_adaptor = NULL, *alignment_adaptor = NULL;
	GladeWidget *gframe, *glabel, *galignment;
	GtkWidget *label;
	gchar     *label_text;
	
	if (reason != GLADE_CREATE_USER)
		return;
		
	g_return_if_fail (GTK_IS_FRAME (frame));
	gframe = glade_widget_get_from_gobject (frame);
	g_return_if_fail (GLADE_IS_WIDGET (gframe));
	
	/* If we didnt put this object here or if frame is an aspect frame... */
	if (((label = gtk_frame_get_label_widget (GTK_FRAME (frame))) == NULL ||
	    (glade_widget_get_from_gobject (label) == NULL)) &&
	    (GTK_IS_ASPECT_FRAME (frame) == FALSE))
	{

		if (label_adaptor == NULL)
			label_adaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_LABEL);
		if (alignment_adaptor == NULL)
			alignment_adaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_ALIGNMENT);

		/* add label (as an internal child) */
		glabel = glade_widget_adaptor_create_widget (label_adaptor, FALSE,
							   "parent", gframe, 
							   "project", glade_widget_get_project (gframe), 
							   NULL);

		label_text = g_strdup_printf ("<b>%s</b>", glade_widget_get_name (gframe));

		glade_widget_property_set (glabel, "label", label_text);
		glade_widget_property_set (glabel, "use-markup", "TRUE");

		g_object_set_data (glabel->object, "special-child-type", "label_item");
		gtk_frame_set_label_widget (GTK_FRAME (frame), GTK_WIDGET (glabel->object));
		gtk_widget_show (GTK_WIDGET (glabel->object));
		g_free (label_text);

		/* add alignment */
		galignment = glade_widget_adaptor_create_widget (alignment_adaptor, FALSE,
						 	       "parent", gframe, 
						               "project", glade_widget_get_project (gframe), 
					                       NULL);

		glade_widget_property_set (galignment, "left-padding", 12);
		gtk_container_add (GTK_CONTAINER (frame), GTK_WIDGET (galignment->object));
		gtk_widget_show (GTK_WIDGET (galignment->object));
	}

	/* Chain Up */
	GWA_GET_CLASS (GTK_TYPE_CONTAINER)->post_create (adaptor, frame, reason);
}

void
glade_gtk_frame_replace_child (GladeWidgetAdaptor *adaptor,
			       GtkWidget          *container,
			       GtkWidget          *current,
			       GtkWidget          *new_widget)
{
	gchar *special_child_type;

	special_child_type =
		g_object_get_data (G_OBJECT (current), "special-child-type");

	if (special_child_type && !strcmp (special_child_type, "label_item"))
	{
		g_object_set_data (G_OBJECT (new_widget), "special-child-type", "label_item");
		gtk_frame_set_label_widget (GTK_FRAME (container), new_widget);
		return;
	}

	/* Chain Up */
	GWA_GET_CLASS
		(GTK_TYPE_CONTAINER)->replace_child (adaptor, 
						     G_OBJECT (container), 
						     G_OBJECT (current),
						     G_OBJECT (new_widget));
}

void
glade_gtk_frame_add_child (GladeWidgetAdaptor *adaptor,
			   GObject            *object, 
			   GObject            *child)
{
	gchar *special_child_type;

	special_child_type = g_object_get_data (child, "special-child-type");

	if (special_child_type &&
	    !strcmp (special_child_type, "label_item"))
	{
		gtk_frame_set_label_widget (GTK_FRAME (object),
					    GTK_WIDGET (child));
	}
	else
	{
		gtk_container_add (GTK_CONTAINER (object),
				   GTK_WIDGET (child));
	}
}

/* ----------------------------- GtkNotebook ------------------------------ */
typedef struct 
{
	gint   pages;
	gint   page;

	GList *children;
	GList *tabs;

	GList *extra_children;
	GList *extra_tabs;
} NotebookChildren;

static gboolean glade_gtk_notebook_setting_position = FALSE;

static gint
notebook_child_compare_func (GtkWidget *widget_a, GtkWidget *widget_b)
{
	GladeWidget *gwidget_a, *gwidget_b;
	gint pos_a = 0, pos_b = 0;

	gwidget_a = glade_widget_get_from_gobject (widget_a);
	gwidget_b = glade_widget_get_from_gobject (widget_b);
	
	g_assert (gwidget_a && gwidget_b);
	
	glade_widget_pack_property_get (gwidget_a, "position", &pos_a);
	glade_widget_pack_property_get (gwidget_b, "position", &pos_b);
	
	return pos_a - pos_b;
}

static gint
notebook_find_child (GtkWidget *check,
		     gpointer   cmp_pos_p)
{
	GladeWidget *gcheck;
	gint         position = 0, cmp_pos = GPOINTER_TO_INT (cmp_pos_p);
		
	gcheck  = glade_widget_get_from_gobject (check);
	g_assert (gcheck);
	
	glade_widget_pack_property_get (gcheck, "position", &position);

	return position - cmp_pos;
}

static gint
notebook_search_tab (GtkNotebook *notebook,
		     GtkWidget   *tab)
{
	GtkWidget *page;
	gint       i;

	for (i = 0; i < gtk_notebook_get_n_pages (notebook); i++)
	{
		page = gtk_notebook_get_nth_page (notebook, i);

		if (tab == gtk_notebook_get_tab_label (notebook, page))
			return i;
	}
	g_critical ("Unable to find tab position in a notebook");
	return -1;
}

static GtkWidget *
notebook_get_filler (NotebookChildren *nchildren, gboolean page)
{
	GtkWidget *widget = NULL;
	
	if (page && nchildren->extra_children)
	{
		widget = nchildren->extra_children->data;
		nchildren->extra_children =
			g_list_remove (nchildren->extra_children, widget);
		g_assert (widget);
	}
	else if (!page && nchildren->extra_tabs)
	{
		widget = nchildren->extra_tabs->data;
		nchildren->extra_tabs =
			g_list_remove (nchildren->extra_tabs, widget);
		g_assert (widget);
	}

	if (widget == NULL)
	{
		/* Need explicit reference here */
		widget = glade_placeholder_new ();

		g_object_ref (G_OBJECT (widget));	

		if (!page)
			g_object_set_data (G_OBJECT (widget), 
					   "special-child-type", "tab");

	}
	return widget;
}

static GtkWidget *
notebook_get_page (NotebookChildren *nchildren, gint position)
{
	GList     *node;
	GtkWidget *widget = NULL;

	if ((node = g_list_find_custom
	     (nchildren->children,
	      GINT_TO_POINTER (position),
	      (GCompareFunc)notebook_find_child)) != NULL)
	{
		widget = node->data;
		nchildren->children = 
			g_list_remove (nchildren->children, widget);
	}
	else
		widget = notebook_get_filler (nchildren, TRUE);

	return widget;
}

static GtkWidget *
notebook_get_tab (NotebookChildren *nchildren, gint position)
{
	GList     *node;
	GtkWidget *widget = NULL;

	if ((node = g_list_find_custom
	     (nchildren->tabs,
	      GINT_TO_POINTER (position),
	      (GCompareFunc)notebook_find_child)) != NULL)
	{
		widget = node->data;
		nchildren->tabs = 
			g_list_remove (nchildren->tabs, widget);
	}
	else
		widget = notebook_get_filler (nchildren, FALSE);

	return widget;
}

static NotebookChildren *
glade_gtk_notebook_extract_children (GtkWidget *notebook)
{
	NotebookChildren *nchildren;
	gchar            *special_child_type;
	GList            *list, *children =
		glade_util_container_get_all_children (GTK_CONTAINER (notebook));
	GladeWidget      *gchild;
	GtkWidget        *page;
	gint              position = 0;

	nchildren        = g_new0 (NotebookChildren, 1);
	nchildren->pages = gtk_notebook_get_n_pages (GTK_NOTEBOOK (notebook));
	nchildren->page  = gtk_notebook_get_current_page (GTK_NOTEBOOK (notebook));
		
	/* Ref all the project widgets and build returned list first */
	for (list = children; list; list = list->next)
	{
		if ((gchild = glade_widget_get_from_gobject (list->data)) != NULL)
		{
			special_child_type =
				g_object_get_data (G_OBJECT (list->data),
						   "special-child-type");

			glade_widget_pack_property_get (gchild, "position", &position);

			g_object_ref (G_OBJECT (list->data));

			/* Sort it into the proper struct member
			 */
			if (special_child_type == NULL)
			{
				if (g_list_find_custom (nchildren->children,
							GINT_TO_POINTER (position),
							(GCompareFunc)notebook_find_child))
					nchildren->extra_children =
						g_list_insert_sorted
						(nchildren->extra_children, list->data,
						 (GCompareFunc)notebook_child_compare_func);
				else
					nchildren->children =
						g_list_insert_sorted
						(nchildren->children, list->data,
						 (GCompareFunc)notebook_child_compare_func);
			} else {
				if (g_list_find_custom (nchildren->tabs,
							GINT_TO_POINTER (position),
							(GCompareFunc)notebook_find_child))
					nchildren->extra_tabs =
						g_list_insert_sorted
						(nchildren->extra_tabs, list->data,
						 (GCompareFunc)notebook_child_compare_func);
				else
					nchildren->tabs =
						g_list_insert_sorted
						(nchildren->tabs, list->data,
						 (GCompareFunc)notebook_child_compare_func);
			}
		}
	}

	/* Remove all pages, resulting in the unparenting of all widgets including tab-labels.
	 */
	while (gtk_notebook_get_n_pages (GTK_NOTEBOOK (notebook)) > 0)
	{
		page = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0);

		/* Explicitly remove the tab label first */
		gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), page, NULL);
		gtk_notebook_remove_page (GTK_NOTEBOOK (notebook), 0);
	}

	if (children)
		g_list_free (children);

	return nchildren;
}

static void
glade_gtk_notebook_insert_children (GtkWidget *notebook, NotebookChildren *nchildren)
{
	gint i;
	
	/*********************************************************
                                INSERT PAGES
	 *********************************************************/
	for (i = 0; i < nchildren->pages; i++)
	{
		GtkWidget *page = notebook_get_page (nchildren, i);
		GtkWidget *tab  = notebook_get_tab (nchildren, i);

		gtk_notebook_insert_page   (GTK_NOTEBOOK (notebook), page, NULL, i);
		gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), page, tab);

		g_object_unref (G_OBJECT (page));
		g_object_unref (G_OBJECT (tab));
	}

	/* Stay on the same page */
	gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), nchildren->page);
	
	/* Free the original lists now */
	if (nchildren->children)
		g_list_free (nchildren->children);

	if (nchildren->tabs)
		g_list_free (nchildren->tabs);

	if (nchildren->children       ||
	    nchildren->tabs           ||
	    nchildren->extra_children ||
	    nchildren->extra_tabs)
		g_critical ("Unbalanced children when inserting notebook children"
			    " (pages: %d tabs: %d extra pages: %d extra tabs %d)",
			    g_list_length (nchildren->children),
			    g_list_length (nchildren->tabs),
			    g_list_length (nchildren->extra_children),
			    g_list_length (nchildren->extra_tabs));
	
	g_free (nchildren);
}

static void
glade_gtk_notebook_switch_page (GtkNotebook     *notebook,
				GtkNotebookPage *page,
				guint            page_num,
				gpointer         user_data)
{
	GladeWidget *gnotebook = glade_widget_get_from_gobject (notebook);

	glade_widget_property_set (gnotebook, "page", page_num);

}


void
glade_gtk_notebook_post_create (GladeWidgetAdaptor *adaptor,
				GObject            *notebook, 
				GladeCreateReason   reason)
{
	gtk_notebook_popup_disable (GTK_NOTEBOOK (notebook));

	g_signal_connect (G_OBJECT (notebook), "switch-page",
			  G_CALLBACK (glade_gtk_notebook_switch_page), NULL);
}

static gint
glade_gtk_notebook_get_first_blank_page (GtkNotebook *notebook)
{
	GladeWidget *gwidget;
	GtkWidget   *widget;
	gint         position;

	for (position = 0; position < gtk_notebook_get_n_pages (notebook); position++)
	{
		widget = gtk_notebook_get_nth_page (notebook, position);
		if ((gwidget = glade_widget_get_from_gobject (widget)) != NULL)
		{
 			GladeProperty *property =
				glade_widget_get_property (gwidget, "position");
			gint gwidget_position = g_value_get_int (property->value);

			if ((gwidget_position - position) > 0)
				return position;
		}
	}
	return position;
}

static void
glade_gtk_notebook_set_n_pages (GObject *object, const GValue *value)
{
	static GladeWidgetAdaptor *wadaptor = NULL;
	GladeWidget *widget;
	GtkNotebook *notebook;
	GtkWidget   *child_widget, *tab_widget;
	gint new_size, i;
	gint old_size;

	notebook = GTK_NOTEBOOK (object);
	g_return_if_fail (GTK_IS_NOTEBOOK (notebook));

	widget = glade_widget_get_from_gobject (GTK_WIDGET (notebook));
	g_return_if_fail (widget != NULL);

	new_size = g_value_get_int (value);

	if (wadaptor == NULL)
		wadaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_LABEL);
	
	/* Ensure base size of notebook */
	if (glade_widget_superuser () == FALSE)
	{
		for (i = gtk_notebook_get_n_pages (notebook); i < new_size; i++)
		{
			gint position = glade_gtk_notebook_get_first_blank_page (notebook);
			GtkWidget *placeholder = glade_placeholder_new ();
			gchar *str;

			GladeWidget *glabel =
				glade_widget_adaptor_create_widget
				(wadaptor, FALSE,
				 "parent", widget, 
				 "project", glade_widget_get_project (widget), 
				 NULL);
			str = g_strdup_printf ("page %d", i + 1);
			glade_widget_property_set (glabel, "label", str);
			g_free (str);
			
			g_object_set_data (glabel->object, "special-child-type", "tab");
			gtk_widget_show (GTK_WIDGET (glabel->object));
			
			gtk_notebook_insert_page (notebook, placeholder,
						  NULL, position);

			/* Must tell the project that were adding a widget (so that
			 * saving works properly & it appears in the inspector properly)
			 */
			glade_project_add_object (glade_widget_get_project (widget), NULL, glabel->object);

			/* Must pass through GladeWidget api so that packing props
			 * are correctly assigned.
			 */
			glade_widget_add_child (widget, glabel, FALSE);
		}
	}

	old_size = gtk_notebook_get_n_pages (GTK_NOTEBOOK (notebook));

	/*
	 * Thing to remember is that GtkNotebook starts the
	 * page numbers from 0, not 1 (C-style). So we need to do
	 * old_size-1, where we're referring to "nth" widget.
	 */
	while (old_size > new_size) {
		/* Get the last widget. */
		GladeWidget *gtab;
		child_widget = gtk_notebook_get_nth_page (notebook, old_size-1);
		tab_widget   = gtk_notebook_get_tab_label (notebook, child_widget);

		/* Ok there shouldnt be widget in the content area, that's
		 * the placeholder, we should clean up the project widget that
		 * we put in the tab here though (this happens in the case where
		 * we undo increasing the "pages" property).
		 */
		if (glade_widget_get_from_gobject (child_widget))
			g_critical ("Bug in notebook_set_n_pages()");
		
		gtk_notebook_remove_page (notebook, old_size-1);

		/* Cleanup possible tab widgets
		 */
		if ((gtab = glade_widget_get_from_gobject (tab_widget)) != NULL)
		{
			glade_project_remove_object (glade_widget_get_project (gtab), gtab->object);
			g_object_unref (gtab);
		}

		old_size--;
	}
}

void
glade_gtk_notebook_set_property (GladeWidgetAdaptor *adaptor,
				 GObject            *object, 
				 const gchar        *id,
				 const GValue       *value)
{
	if (!strcmp (id, "pages"))
		glade_gtk_notebook_set_n_pages (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor, object,
								  id, value);
}

static gboolean
glade_gtk_notebook_verify_n_pages (GObject *object, const GValue *value)
{
	GtkNotebook *notebook = GTK_NOTEBOOK(object);
	GtkWidget *child_widget, *tab_widget;
	gint old_size, new_size = g_value_get_int (value);
	
	for (old_size = gtk_notebook_get_n_pages (notebook);
	     old_size > new_size; old_size--) {
		/* Get the last widget. */
		child_widget = gtk_notebook_get_nth_page (notebook, old_size-1);
		tab_widget   = gtk_notebook_get_tab_label (notebook, child_widget);

		/* 
		 * If we got it, and its not a placeholder, remove it
		 * from project.
		 */
		if (glade_widget_get_from_gobject (child_widget) ||
		    glade_widget_get_from_gobject (tab_widget))
			return FALSE;
	}
	return TRUE;
}

gboolean
glade_gtk_notebook_verify_property (GladeWidgetAdaptor *adaptor,
				    GObject            *object, 
				    const gchar        *id,
				    const GValue       *value)
{
	if (!strcmp (id, "pages"))
		return glade_gtk_notebook_verify_n_pages (object, value);
	else if (GWA_GET_CLASS (GTK_TYPE_CONTAINER)->verify_property)
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->verify_property (adaptor, object,
								     id, value);

	return TRUE;
}

void 
glade_gtk_notebook_add_child (GladeWidgetAdaptor *adaptor,
			      GObject            *object, 
			      GObject            *child)
{
	GtkNotebook 	*notebook;
	gint             num_page, position = 0;
	GtkWidget	*last_page;
	GladeWidget	*gwidget;
	gchar		*special_child_type;
	
	notebook = GTK_NOTEBOOK (object);

	num_page = gtk_notebook_get_n_pages (notebook);
	gwidget = glade_widget_get_from_gobject (object);
	
	/* Just append pages blindly when loading/dupping
	 */
	if (glade_widget_superuser ())
	{
		special_child_type = g_object_get_data (child, "special-child-type");
		if (special_child_type &&
		    !strcmp (special_child_type, "tab"))
		{
			last_page = gtk_notebook_get_nth_page (notebook, num_page - 1);
			gtk_notebook_set_tab_label (notebook, last_page,
						    GTK_WIDGET (child));
		}
		else
		{
			gtk_container_add (GTK_CONTAINER (object), GTK_WIDGET (child));
			
			glade_widget_property_set (gwidget, "pages", num_page + 1);
			
			gwidget = glade_widget_get_from_gobject (child);
			if (gwidget && gwidget->packing_properties)
				glade_widget_pack_property_set (gwidget, "position", num_page);
		}
	}
	else
	{
		NotebookChildren *nchildren;

		/* Just destroy placeholders */
		if (GLADE_IS_PLACEHOLDER (child))
		{
			if (GTK_OBJECT_FLOATING (child))
				gtk_object_sink (GTK_OBJECT (child));
			else
				g_object_unref (G_OBJECT (child));
		}
		else
		{
			gwidget = glade_widget_get_from_gobject (child);
			g_assert (gwidget);

			glade_widget_pack_property_get (gwidget, "position", &position);
			
			nchildren = glade_gtk_notebook_extract_children (GTK_WIDGET (notebook));
			
			if (g_object_get_data (child, "special-child-type") != NULL)
			{
				if (g_list_find_custom (nchildren->tabs,
							GINT_TO_POINTER (position),
							(GCompareFunc)notebook_find_child))
					nchildren->extra_tabs =
						g_list_insert_sorted
						(nchildren->extra_tabs, child,
						 (GCompareFunc)notebook_child_compare_func);
				else
					nchildren->tabs =
						g_list_insert_sorted
						(nchildren->tabs, child,
						 (GCompareFunc)notebook_child_compare_func);
			}
			else
			{
				if (g_list_find_custom (nchildren->children,
							GINT_TO_POINTER (position),
							(GCompareFunc)notebook_find_child))
					nchildren->extra_children =
						g_list_insert_sorted
						(nchildren->extra_children, child,
						 (GCompareFunc)notebook_child_compare_func);
				else
					nchildren->children =
						g_list_insert_sorted
						(nchildren->children, child,
						 (GCompareFunc)notebook_child_compare_func);
			}

			/* Takes an explicit reference when sitting on the list */
			g_object_ref (child);
			
			glade_gtk_notebook_insert_children (GTK_WIDGET (notebook), nchildren);
		}
	}
}

void
glade_gtk_notebook_remove_child (GladeWidgetAdaptor *adaptor,
				 GObject            *object, 
				 GObject            *child)
{
	NotebookChildren *nchildren;

	nchildren = glade_gtk_notebook_extract_children (GTK_WIDGET (object));
			
	if (g_list_find (nchildren->children, child))
	{
		nchildren->children =
			g_list_remove (nchildren->children, child);
		g_object_unref (child);
	}
	else if (g_list_find (nchildren->extra_children, child))
	{
		nchildren->extra_children =
			g_list_remove (nchildren->extra_children, child);
		g_object_unref (child);
	}
	else if (g_list_find (nchildren->tabs, child))
	{
		nchildren->tabs =
			g_list_remove (nchildren->tabs, child);
		g_object_unref (child);
	}
	else if (g_list_find (nchildren->extra_tabs, child))
	{
		nchildren->extra_tabs =
			g_list_remove (nchildren->extra_tabs, child);
		g_object_unref (child);
	}
	
	glade_gtk_notebook_insert_children (GTK_WIDGET (object), nchildren);
	
}

void
glade_gtk_notebook_replace_child (GladeWidgetAdaptor *adaptor,
				  GtkWidget          *container,
				  GtkWidget          *current,
				  GtkWidget          *new_widget)
{
	GtkNotebook *notebook;
	GladeWidget *gcurrent, *gnew;
	gint         position = 0;

	notebook = GTK_NOTEBOOK (container);

	if ((gcurrent = glade_widget_get_from_gobject (current)) != NULL)
		glade_widget_pack_property_get (gcurrent, "position", &position);
	else
	{
		g_assert (GLADE_IS_PLACEHOLDER (current));

		if ((position = gtk_notebook_page_num (notebook, current)) < 0)
		{
			position = notebook_search_tab (notebook, current);
			g_assert (position >= 0);
		}
	}
	
	if (g_object_get_data (G_OBJECT (current), "special-child-type"))
		g_object_set_data (G_OBJECT (new_widget), "special-child-type", "tab");

	glade_gtk_notebook_remove_child (adaptor, 
					 G_OBJECT (container), 
					 G_OBJECT (current));

	if (GLADE_IS_PLACEHOLDER (new_widget) == FALSE)
	{
		gnew = glade_widget_get_from_gobject (new_widget);

		glade_gtk_notebook_add_child (adaptor, 
					      G_OBJECT (container), 
					      G_OBJECT (new_widget));
		
		if (glade_widget_pack_property_set (gnew, "position", position) == FALSE)
			g_critical ("No position property found on new widget");
	}
	else 
		gtk_widget_destroy (GTK_WIDGET (new_widget));
}	

gboolean
glade_gtk_notebook_child_verify_property (GladeWidgetAdaptor *adaptor,
					  GObject            *container,
					  GObject            *child,
					  const gchar        *id,
					  GValue             *value)
{
	if (!strcmp (id, "position"))
		return g_value_get_int (value) >= 0 &&
			g_value_get_int (value) < 
			gtk_notebook_get_n_pages (GTK_NOTEBOOK (container));
	else if (GWA_GET_CLASS (GTK_TYPE_CONTAINER)->child_verify_property)
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_verify_property (adaptor, 
								     container, child,
								     id, value);

	return TRUE;
}

void
glade_gtk_notebook_set_child_property (GladeWidgetAdaptor *adaptor,
				       GObject            *container,
				       GObject            *child,
				       const gchar        *property_name,
				       const GValue       *value)
{
	NotebookChildren *nchildren;

	if (strcmp (property_name, "position") == 0)
	{
		/* If we are setting this internally, avoid feedback. */
		if (glade_gtk_notebook_setting_position ||
		    glade_widget_superuser ())
			return;
		
		/* Just rebuild the notebook, property values are already set at this point */
		nchildren = glade_gtk_notebook_extract_children (GTK_WIDGET (container));
		glade_gtk_notebook_insert_children (GTK_WIDGET (container), nchildren);
	}
	/* packing properties are unsupported on tabs ... except "position" */
	else if (g_object_get_data (child, "special-child-type") == NULL)
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_set_property (adaptor, 
								  container, child,
								  property_name, value);
}

void
glade_gtk_notebook_get_child_property (GladeWidgetAdaptor *adaptor,
				       GObject            *container,
				       GObject            *child,
				       const gchar        *property_name,
				       GValue             *value)
{
	gint position;
	
	if (strcmp (property_name, "position") == 0)
	{
		if (g_object_get_data (child, "special-child-type") != NULL)
		{
			if ((position = notebook_search_tab (GTK_NOTEBOOK (container),
							     GTK_WIDGET (child))) >= 0)
				g_value_set_int (value, position);
			else
				g_value_set_int (value, 0);
		}
		else
			gtk_container_child_get_property (GTK_CONTAINER (container),
							  GTK_WIDGET (child),
							  property_name,
							  value);
	}
	/* packing properties are unsupported on tabs ... except "position" */
	else if (g_object_get_data (child, "special-child-type") == NULL)
		gtk_container_child_get_property (GTK_CONTAINER (container),
						  GTK_WIDGET (child),
						  property_name,
						  value);
}

static void
glade_gtk_box_notebook_child_insert_remove_action (GladeWidgetAdaptor *adaptor,
						   GObject *container,
						   GObject *object,
						   const gchar *size_prop,
						   const gchar *group_format,
						   gboolean remove,
						   gboolean after)
{
	GladeWidget *parent;
	GList *children, *l;
	gint child_pos, size, offset;
	
	if (GTK_IS_NOTEBOOK (container) &&
	    g_object_get_data (object, "special-child-type"))
		/* Its a Tab! */
		child_pos = notebook_search_tab (GTK_NOTEBOOK (container),
						 GTK_WIDGET (object));
	else
		gtk_container_child_get (GTK_CONTAINER (container),
					 GTK_WIDGET (object),
					 "position", &child_pos, NULL);
	
	parent = glade_widget_get_from_gobject (container);
	glade_command_push_group (group_format, glade_widget_get_name (parent));
	
	children = glade_widget_adaptor_get_children (adaptor, container);
	/* Make sure widgets does not get destroyed */
	g_list_foreach (children, (GFunc) g_object_ref, NULL);
	
	glade_widget_property_get (parent, size_prop, &size);
	
	if (remove)
	{
		GList *del = NULL;
		offset = -1;
		/* Remove children first */
		for (l = children; l; l = g_list_next (l))
		{
			GladeWidget *gchild = glade_widget_get_from_gobject (l->data);
			gint pos;
			
			/* Skip placeholders */
			if (gchild == NULL) continue;
		
			glade_widget_pack_property_get (gchild, "position", &pos);
			if (pos == child_pos) del = g_list_prepend (del, gchild);
		}
		if (del)
		{
			glade_command_delete (del);
			g_list_free (del);
		}
	}
	else
	{
		/* Expand container */
		glade_command_set_property (glade_widget_get_property (parent, size_prop),
					    size + 1);
		offset = 1;
	}
	
	/* Reoder children */
	for (l = children; l; l = g_list_next (l))
	{
		GladeWidget *gchild = glade_widget_get_from_gobject (l->data);
		gint pos;
			
		/* Skip placeholders */
		if (gchild == NULL) continue;
		
		glade_widget_pack_property_get (gchild, "position", &pos);
		if ((after) ? pos > child_pos : pos >= child_pos)
			glade_command_set_property (glade_widget_get_pack_property (gchild, "position"),
						    pos + offset);
	}
	
	if (remove)
	{
		/* Shrink container */
		glade_command_set_property (glade_widget_get_property (parent, size_prop),
					    size - 1);
	}
	
	g_list_foreach (children, (GFunc) g_object_unref, NULL);
	g_list_free (children);
	glade_command_pop_group ();
}

void
glade_gtk_notebook_child_action_activate (GladeWidgetAdaptor *adaptor,
					  GObject            *container,
					  GObject            *object,
					  const gchar        *action_path)
{
	if (strcmp (action_path, "insert_page_after") == 0)
	{
		glade_gtk_box_notebook_child_insert_remove_action (adaptor, container,
								   object, "pages",
								   _("Insert page on %s"),
								   FALSE, TRUE);
	}
	else if (strcmp (action_path, "insert_page_before") == 0)
	{
		glade_gtk_box_notebook_child_insert_remove_action (adaptor, container,
								   object, "pages",
								   _("Insert page on %s"),
								   FALSE, FALSE);
	}
	else if (strcmp (action_path, "remove_page") == 0)
	{
		glade_gtk_box_notebook_child_insert_remove_action (adaptor, container,
								   object, "pages",
								   _("Remove page from %s"),
								   TRUE, TRUE);
	}
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->child_action_activate (adaptor,
									   container,
									   object,
									   action_path);
}

/* ----------------------------- GtkPaned ------------------------------ */
void
glade_gtk_paned_post_create (GladeWidgetAdaptor *adaptor,
			     GObject            *paned, 
			     GladeCreateReason   reason)
{
	g_return_if_fail (GTK_IS_PANED (paned));

	if (reason == GLADE_CREATE_USER && gtk_paned_get_child1 (GTK_PANED (paned)) == NULL)
		gtk_paned_add1 (GTK_PANED (paned), glade_placeholder_new ());
	
	if (reason == GLADE_CREATE_USER && gtk_paned_get_child2 (GTK_PANED (paned)) == NULL)
		gtk_paned_add2 (GTK_PANED (paned), glade_placeholder_new ());
}

void 
glade_gtk_paned_add_child (GladeWidgetAdaptor *adaptor,
			   GObject            *object, 
			   GObject            *child)
{
	GtkPaned *paned;
	GtkWidget *child1, *child2;
	gboolean loading;
	
	g_return_if_fail (GTK_IS_PANED (object));
	
	paned = GTK_PANED (object);
	loading = glade_util_object_is_loading (object);
	
	child1 = gtk_paned_get_child1 (paned);
	child2 = gtk_paned_get_child2 (paned);

	if (loading == FALSE)
	{
		/* Remove a placeholder */
		if (child1 && GLADE_IS_PLACEHOLDER (child1))
		{
			gtk_container_remove (GTK_CONTAINER (object), child1);
			child1 = NULL;
		}
		else if (child2 && GLADE_IS_PLACEHOLDER (child2))
		{
			gtk_container_remove (GTK_CONTAINER (object), child2);
			child2 = NULL;
		}
	}

	/* Add the child */
	if (child1 == NULL)
		gtk_paned_add1 (paned, GTK_WIDGET (child));
	else if (child2 == NULL)
		gtk_paned_add2 (paned, GTK_WIDGET (child));
	
	if (GLADE_IS_PLACEHOLDER (child) == FALSE && loading)
	{
		GladeWidget *gchild = glade_widget_get_from_gobject (child);
		
		if (gchild && gchild->packing_properties)
		{
			if (child1 == NULL)
				glade_widget_pack_property_set (gchild, "first", TRUE);
			else if (child2 == NULL)
				glade_widget_pack_property_set (gchild, "first", FALSE);
		}
	}
}

void 
glade_gtk_paned_remove_child (GladeWidgetAdaptor *adaptor,
			      GObject            *object,
			      GObject            *child)
{
	gtk_container_remove (GTK_CONTAINER (object), GTK_WIDGET (child));

	glade_gtk_paned_post_create (adaptor, object, GLADE_CREATE_USER);
}

void
glade_gtk_paned_set_child_property (GladeWidgetAdaptor *adaptor,
				    GObject            *container,
				    GObject            *child,
				    const gchar        *property_name,
				    const GValue       *value)
{
	if (strcmp (property_name, "first") == 0)
	{
		GtkPaned *paned = GTK_PANED (container);
		gboolean first = g_value_get_boolean (value);
		GtkWidget *place, *wchild = GTK_WIDGET (child);
		
		place = (first) ? gtk_paned_get_child1 (paned) :
				  gtk_paned_get_child2 (paned);
		
		if (place && GLADE_IS_PLACEHOLDER (place))
			gtk_container_remove (GTK_CONTAINER (container), place);
		
		g_object_ref (child);
		gtk_container_remove (GTK_CONTAINER (container), wchild);
		if (first)
			gtk_paned_add1 (paned, wchild);
		else
			gtk_paned_add2 (paned, wchild);
		g_object_unref (child);
		
		/* Ensure placeholders */
		if (glade_util_object_is_loading (child) == FALSE)
		{
			if ((place = gtk_paned_get_child1 (paned)) == NULL)
				gtk_paned_add1 (paned, glade_placeholder_new ());
		
			if ((place = gtk_paned_get_child2 (paned)) == NULL)
				gtk_paned_add2 (paned, glade_placeholder_new ());
		}
	}
	else
		/* Chain Up */
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_set_property (adaptor, 
								  container, child,
								  property_name, value);
}

void
glade_gtk_paned_get_child_property (GladeWidgetAdaptor *adaptor,
				    GObject            *container,
				    GObject            *child,
				    const gchar        *property_name,
				    GValue             *value)
{
	if (strcmp (property_name, "first") == 0)
		g_value_set_boolean (value, GTK_WIDGET (child) ==
				     gtk_paned_get_child1 (GTK_PANED (container)));
	else
		/* Chain Up */
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_get_property (adaptor, 
								  container, child,
								  property_name, value);
}

/* ----------------------------- GtkExpander ------------------------------ */
void
glade_gtk_expander_post_create (GladeWidgetAdaptor *adaptor,
				GObject            *expander, 
				GladeCreateReason   reason)
{
	static GladeWidgetAdaptor *wadaptor = NULL;
	GladeWidget *gexpander, *glabel;
	GtkWidget *label;
	
	if (wadaptor == NULL)
		wadaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_LABEL);
	
	if (reason != GLADE_CREATE_USER) return;
	
	g_return_if_fail (GTK_IS_EXPANDER (expander));
	gexpander = glade_widget_get_from_gobject (expander);
	g_return_if_fail (GLADE_IS_WIDGET (gexpander));
	
	/* If we didnt put this object here... */
	if ((label = gtk_expander_get_label_widget (GTK_EXPANDER (expander))) == NULL ||
	    (glade_widget_get_from_gobject (label) == NULL))
	{
		glabel = glade_widget_adaptor_create_widget (wadaptor, FALSE,
							     "parent", gexpander, 
							     "project", glade_widget_get_project (gexpander), 
							     NULL);
		
		glade_widget_property_set (glabel, "label", "expander");

		g_object_set_data (glabel->object, "special-child-type", "label_item");
		gtk_expander_set_label_widget (GTK_EXPANDER (expander), 
					       GTK_WIDGET (glabel->object));

		gtk_widget_show (GTK_WIDGET (glabel->object));
	}

	gtk_expander_set_expanded (GTK_EXPANDER (expander), TRUE);
	
	gtk_container_add (GTK_CONTAINER (expander), glade_placeholder_new ());

}

void
glade_gtk_expander_replace_child (GladeWidgetAdaptor *adaptor,
				  GtkWidget          *container,
				  GtkWidget          *current,
				  GtkWidget          *new_widget)
{
	gchar *special_child_type;

	special_child_type =
		g_object_get_data (G_OBJECT (current), "special-child-type");

	if (special_child_type && !strcmp (special_child_type, "label_item"))
	{
		g_object_set_data (G_OBJECT (new_widget), "special-child-type", "label_item");
		gtk_expander_set_label_widget (GTK_EXPANDER (container), new_widget);
		return;
	}

	/* Chain Up */
	GWA_GET_CLASS
		(GTK_TYPE_CONTAINER)->replace_child (adaptor, 
						     G_OBJECT (container), 
						     G_OBJECT (current),
						     G_OBJECT (new_widget));
}


void
glade_gtk_expander_add_child (GladeWidgetAdaptor *adaptor,
			      GObject            *object, 
			      GObject            *child)
{
	gchar *special_child_type;

	special_child_type = g_object_get_data (child, "special-child-type");
	
	if (special_child_type &&
	    !strcmp (special_child_type, "label_item"))
	{
		gtk_expander_set_label_widget (GTK_EXPANDER (object),
					       GTK_WIDGET (child));
	}
	else
		/* Chain Up */
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->add (adaptor, object, child); 
}

void
glade_gtk_expander_remove_child (GladeWidgetAdaptor *adaptor,
				 GObject            *object, 
				 GObject            *child)
{
	gchar *special_child_type;

	special_child_type = g_object_get_data (child, "special-child-type");
	if (special_child_type &&
	    !strcmp (special_child_type, "label_item"))
	{
		gtk_expander_set_label_widget (GTK_EXPANDER (object),
					       glade_placeholder_new ());
	}
	else
	{
		gtk_container_remove (GTK_CONTAINER (object),
				      GTK_WIDGET (child));
		gtk_container_add (GTK_CONTAINER (object),
				   glade_placeholder_new ());
	}
}

/* -------------------------------- GtkEntry -------------------------------- */
static void
glade_gtk_entry_changed (GtkEditable *editable, GladeWidget *gentry)
{
	const gchar *text, *text_prop;
	GladeProperty *prop;
	
	text = gtk_entry_get_text (GTK_ENTRY (editable));
	
	glade_widget_property_get (gentry, "text", &text_prop);
	
	if (strcmp (text, text_prop))
		if ((prop = glade_widget_get_property (gentry, "text")))
			glade_command_set_property (prop, text);
}

void
glade_gtk_entry_post_create (GladeWidgetAdaptor *adaptor,
			     GObject            *object,
			     GladeCreateReason   reason)
{
	GladeWidget *gentry;
	
	g_return_if_fail (GTK_IS_ENTRY (object));
	gentry = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gentry));
	
	g_signal_connect (object, "changed",
			  G_CALLBACK (glade_gtk_entry_changed), gentry);
}

/* ----------------------------- GtkFixed/GtkLayout ------------------------------ */
static void
glade_gtk_fixed_layout_finalize(GdkPixmap *backing)
{
	g_object_unref(backing);
}

static void
glade_gtk_fixed_layout_realize (GtkWidget *widget)
{
	GdkPixmap *backing =
		gdk_pixmap_colormap_create_from_xpm_d (NULL, gtk_widget_get_colormap (widget),
						       NULL, NULL, fixed_bg_xpm);

	if (GTK_IS_LAYOUT (widget))
		gdk_window_set_back_pixmap (GTK_LAYOUT (widget)->bin_window, backing, FALSE);
	else
		gdk_window_set_back_pixmap (widget->window, backing, FALSE);

	/* For cleanup later
	 */
	g_object_weak_ref(G_OBJECT(widget), 
			  (GWeakNotify)glade_gtk_fixed_layout_finalize, backing);
}

void
glade_gtk_fixed_layout_post_create (GladeWidgetAdaptor *adaptor,
				    GObject            *object, 
				    GladeCreateReason   reason)
{
	/* This is needed at least to set a backing pixmap. */
	GTK_WIDGET_UNSET_FLAGS(object, GTK_NO_WINDOW);

	/* For backing pixmap
	 */
	g_signal_connect_after(object, "realize",
			       G_CALLBACK(glade_gtk_fixed_layout_realize), NULL);
}

void
glade_gtk_fixed_layout_add_child (GladeWidgetAdaptor  *adaptor,
				  GObject             *object, 
				  GObject             *child)
{
	g_return_if_fail (GTK_IS_CONTAINER (object));
	g_return_if_fail (GTK_IS_WIDGET (child));

	gtk_container_add (GTK_CONTAINER (object), GTK_WIDGET (child));
}

void
glade_gtk_fixed_layout_remove_child (GladeWidgetAdaptor  *adaptor,
				     GObject             *object, 
				     GObject             *child)
{
	g_return_if_fail (GTK_IS_CONTAINER (object));
	g_return_if_fail (GTK_IS_WIDGET (child));

	gtk_container_remove (GTK_CONTAINER (object), GTK_WIDGET (child));
}

/* ----------------------------- GtkWindow ------------------------------ */
void
glade_gtk_window_post_create (GladeWidgetAdaptor *adaptor,
			      GObject            *object,
			      GladeCreateReason   reason)
{
	GtkWindow *window = GTK_WINDOW (object);

	g_return_if_fail (GTK_IS_WINDOW (window));

	/* Chain her up first */
	GWA_GET_CLASS (GTK_TYPE_CONTAINER)->post_create (adaptor, object, reason);
}

/* ----------------------------- GtkDialog(s) ------------------------------ */
static void
glade_gtk_file_chooser_default_forall (GtkWidget *widget, gpointer data)
{
	static gpointer hierarchy = NULL, screen;
	
	/* Since GtkFileChooserDefault is not exposed we check if its a
	 * GtkFileChooser
	 */
	if (GTK_IS_FILE_CHOOSER (widget))
	{
		if (hierarchy == NULL)
		{
			hierarchy = GUINT_TO_POINTER (g_signal_lookup (
							"hierarchy-changed",
							GTK_TYPE_WIDGET));
			screen = GUINT_TO_POINTER (g_signal_lookup (
							"screen-changed",
							GTK_TYPE_WIDGET));
		}
		/* Finally we can connect to the signals we want to stop its
		 * default handler. Since both signals has the same signature
		 * we use one callback for both :)
		 */
		g_signal_connect (widget, "hierarchy-changed",
				  G_CALLBACK (glade_gtk_stop_emission_POINTER),
				  hierarchy);
		g_signal_connect (widget, "screen-changed",
				  G_CALLBACK (glade_gtk_stop_emission_POINTER),
				  screen);
	}
}

static void
glade_gtk_file_chooser_forall (GtkWidget *widget, gpointer data)
{
	/* GtkFileChooserWidget packs a GtkFileChooserDefault */
	if (GTK_IS_FILE_CHOOSER_WIDGET (widget))
		gtk_container_forall (GTK_CONTAINER (widget),
				      glade_gtk_file_chooser_default_forall,
				      NULL);
}

static void
glade_gtk_input_dialog_forall (GtkWidget *widget, gpointer data)
{
	/* Make every option menu insensitive, yes it use a deprecated widget */
	if (GTK_IS_OPTION_MENU (widget))
		gtk_widget_set_sensitive (widget, FALSE);
	else if (GTK_IS_CONTAINER (widget))
		gtk_container_forall (GTK_CONTAINER (widget),
				      glade_gtk_input_dialog_forall,
				      NULL);
}

void 
glade_gtk_dialog_post_create (GladeWidgetAdaptor *adaptor,
			      GObject            *object, 
			      GladeCreateReason   reason)
{
	GtkDialog    *dialog = GTK_DIALOG (object);
	GladeWidget  *widget;
	GladeWidget  *vbox_widget, *actionarea_widget, *colorsel, *fontsel;
	GladeWidget  *save_button = NULL, *close_button = NULL, *ok_button = NULL,
		*cancel_button = NULL, *help_button = NULL, *apply_button = NULL;
	
	g_return_if_fail (GTK_IS_DIALOG (dialog));

	widget = glade_widget_get_from_gobject (GTK_WIDGET (dialog));
	if (!widget)
		return;

	if (reason == GLADE_CREATE_USER)
	{
		/* HIG complient border-width defaults on dialogs */
		glade_widget_property_set (widget, "border-width", 5);
	}

	if (GTK_IS_INPUT_DIALOG (object))
	{
		GtkInputDialog *id = GTK_INPUT_DIALOG (dialog);
			
		save_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (id->save_button),
			 "save_button", "inputdialog", FALSE, reason);
		close_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (id->close_button),
			 "close_button", "inputdialog", FALSE, reason);
		/*
		  On device and mode menu items "activate" signal handlers 
		  GtkInputDialog call gtk_widget_get_toplevel() and assume that
		  the toplevel returned is the GtkInputDialog but since the 
		  dialog is embed inside glade the returned pointer is not what
		  expected and this gives a segfault :S
		*/
		gtk_container_forall (GTK_CONTAINER (dialog),
				      glade_gtk_input_dialog_forall,
				      NULL);
	}
	else if (GTK_IS_FILE_SELECTION (object))
	{
		ok_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_FILE_SELECTION (object)->ok_button),
			 "ok_button", "filesel", FALSE, reason);

		cancel_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_FILE_SELECTION (object)->cancel_button),
			 "cancel_button", "filesel", FALSE, reason);
	}
	else if (GTK_IS_COLOR_SELECTION_DIALOG (object))
	{
		ok_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_COLOR_SELECTION_DIALOG (object)->ok_button),
			 "ok_button", "colorsel", FALSE, reason);

		cancel_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_COLOR_SELECTION_DIALOG (object)->cancel_button),
			 "cancel_button", "colorsel", FALSE, reason);

		help_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_COLOR_SELECTION_DIALOG (object)->help_button),
			 "help_button", "colorsel", FALSE, reason);

		colorsel = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_COLOR_SELECTION_DIALOG (object)->colorsel),
			 "color_selection", "colorsel", FALSE, reason);

		/* Set this to 1 at load time, if there are any children then
		 * size will adjust appropriately (otherwise the default "3" gets
		 * set and we end up with extra placeholders).
		 */
		if (reason == GLADE_CREATE_LOAD)
			glade_widget_property_set (colorsel, "size", 1);

	}
	else if (GTK_IS_FONT_SELECTION_DIALOG (object))
	{
		ok_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_FONT_SELECTION_DIALOG (object)->ok_button),
			 "ok_button", "fontsel", FALSE, reason);

		apply_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_FONT_SELECTION_DIALOG (object)->apply_button),
			 "apply_button", "fontsel", FALSE, reason);
		
		cancel_button = glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_FONT_SELECTION_DIALOG (object)->cancel_button),
			 "cancel_button", "fontsel", FALSE, reason);

		fontsel =  glade_widget_adaptor_create_internal
			(widget, G_OBJECT (GTK_FONT_SELECTION_DIALOG (object)->fontsel),
			 "font_selection", "fontsel", FALSE, reason);

		/* Set this to 1 at load time, if there are any children then
		 * size will adjust appropriately (otherwise the default "3" gets
		 * set and we end up with extra placeholders).
		 */
		if (reason == GLADE_CREATE_LOAD)
			glade_widget_property_set (fontsel, "size", 2);
	}
	else
	{
		/* We need to stop default emissions of "hierarchy-changed" and 
		 * "screen-changed" of GtkFileChooserDefault to avoid an abort()
		 * when doing a reparent.
		 * GtkFileChooserDialog packs a GtkFileChooserWidget in 
		 * his internal vbox.
		 */
		if (GTK_IS_FILE_CHOOSER_DIALOG (object))
			gtk_container_forall (GTK_CONTAINER (dialog->vbox),
					      glade_gtk_file_chooser_forall,
					      NULL);
		
		vbox_widget = glade_widget_adaptor_create_internal 
			(widget, G_OBJECT(dialog->vbox),
			 "vbox", "dialog", FALSE, reason);

		actionarea_widget = glade_widget_adaptor_create_internal
			(vbox_widget, G_OBJECT(dialog->action_area),
			 "action_area", "dialog", FALSE, reason);

		/* These properties are controlled by the GtkDialog style properties:
		 * "content-area-border", "button-spacing" and "action-area-border",
		 * so we must disable thier use.
		 */
		glade_widget_remove_property (vbox_widget, "border-width");
		glade_widget_remove_property (actionarea_widget, "border-width");
		glade_widget_remove_property (actionarea_widget, "spacing");

		/* Only set these on the original create. */
		if (reason == GLADE_CREATE_USER)
		{

			/* HIG complient spacing defaults on dialogs */
			glade_widget_property_set (vbox_widget, "spacing", 2);

			if (GTK_IS_MESSAGE_DIALOG (object))
				glade_widget_property_set (vbox_widget, "size", 2);
			else if (GTK_IS_ABOUT_DIALOG (object))
				glade_widget_property_set (vbox_widget, "size", 3);
			else if (GTK_IS_FILE_CHOOSER_DIALOG (object))
				glade_widget_property_set (vbox_widget, "size", 3);
			else		
				glade_widget_property_set (vbox_widget, "size", 2);

			glade_widget_property_set (actionarea_widget, "size", 2);
			glade_widget_property_set (actionarea_widget, "layout-style", GTK_BUTTONBOX_END);
		}
	}
}


GtkWidget *
glade_gtk_dialog_get_internal_child (GladeWidgetAdaptor  *adaptor,
				     GtkDialog           *dialog,
				     const gchar         *name)
{
	GtkWidget *child = NULL;

	g_return_val_if_fail (GTK_IS_DIALOG (dialog), NULL);

	if (GTK_IS_INPUT_DIALOG (dialog))
	{
		if (strcmp ("close_button", name) == 0)
			child = GTK_INPUT_DIALOG (dialog)->close_button;
		else if (strcmp ("save_button", name) == 0)
			child = GTK_INPUT_DIALOG (dialog)->save_button;
	}
	else if (GTK_IS_FILE_SELECTION (dialog))
	{
		if (strcmp ("ok_button", name) == 0)
			child = GTK_FILE_SELECTION (dialog)->ok_button;
		else if (strcmp ("cancel_button", name) == 0)
			child = GTK_FILE_SELECTION (dialog)->cancel_button;
	}
	else if (GTK_IS_COLOR_SELECTION_DIALOG (dialog))
	{
		if (strcmp ("ok_button", name) == 0)
			child = GTK_COLOR_SELECTION_DIALOG (dialog)->ok_button;
		else if (strcmp ("cancel_button", name) == 0)
			child = GTK_COLOR_SELECTION_DIALOG (dialog)->cancel_button;
		else if (strcmp ("help_button", name) == 0)
			child = GTK_COLOR_SELECTION_DIALOG (dialog)->help_button;
		else if (strcmp ("color_selection", name) == 0)
			child = GTK_COLOR_SELECTION_DIALOG (dialog)->colorsel;
	}
	else if (GTK_IS_FONT_SELECTION_DIALOG (dialog))
	{
		if (strcmp ("ok_button", name) == 0)
			child = GTK_FONT_SELECTION_DIALOG (dialog)->ok_button;
		else if (strcmp ("apply_button", name) == 0)
			child = GTK_FONT_SELECTION_DIALOG (dialog)->apply_button;
		else if (strcmp ("cancel_button", name) == 0)
			child = GTK_FONT_SELECTION_DIALOG (dialog)->cancel_button;
		else if (strcmp ("font_selection", name) == 0)
			child = GTK_FONT_SELECTION_DIALOG (dialog)->fontsel;
	}
	else
	{
		/* Default generic dialog handling
		 */
		if (strcmp ("vbox", name) == 0)
			child = dialog->vbox;
		else if (strcmp ("action_area", name) == 0)
			child = dialog->action_area;
	}

	return child;
}

GList *
glade_gtk_dialog_get_children (GladeWidgetAdaptor  *adaptor,
			       GtkDialog           *dialog)
{
	GList *list = NULL;

	g_return_val_if_fail (GTK_IS_DIALOG (dialog), NULL);

	list = glade_util_container_get_all_children (GTK_CONTAINER (dialog));

	if (GTK_IS_INPUT_DIALOG (dialog))
	{
		list = g_list_prepend (list, GTK_INPUT_DIALOG (dialog)->close_button);
		list = g_list_prepend (list, GTK_INPUT_DIALOG (dialog)->save_button);
	}
	else if (GTK_IS_FILE_SELECTION (dialog))
	{
		list = g_list_prepend (list, GTK_FILE_SELECTION (dialog)->ok_button);
		list = g_list_prepend (list, GTK_FILE_SELECTION (dialog)->cancel_button);
	}
	else if (GTK_IS_COLOR_SELECTION_DIALOG (dialog))
	{
		list = g_list_prepend (list, GTK_COLOR_SELECTION_DIALOG (dialog)->ok_button);
		list = g_list_prepend (list, GTK_COLOR_SELECTION_DIALOG (dialog)->cancel_button);
		list = g_list_prepend (list, GTK_COLOR_SELECTION_DIALOG (dialog)->help_button);
		list = g_list_prepend (list, GTK_COLOR_SELECTION_DIALOG (dialog)->colorsel);
	}
	else if (GTK_IS_FONT_SELECTION_DIALOG (dialog))
	{
		list = g_list_prepend (list, GTK_FONT_SELECTION_DIALOG (dialog)->ok_button);
		list = g_list_prepend (list, GTK_FONT_SELECTION_DIALOG (dialog)->apply_button);
		list = g_list_prepend (list, GTK_FONT_SELECTION_DIALOG (dialog)->cancel_button);
		list = g_list_prepend (list, GTK_FONT_SELECTION_DIALOG (dialog)->fontsel);
	}
	return list;
}


void
glade_gtk_dialog_set_property (GladeWidgetAdaptor *adaptor,
			       GObject            *object, 
			       const gchar        *id,
			       const GValue       *value)
{
	if (GTK_IS_MESSAGE_DIALOG (object) && !strcmp (id, "image"))
	{
		/* Gtk+ 2.10 crashes when you unset the image of 
		 * a message dialog, so we dont ever unset it.
		 */
		if (g_value_get_object (value))
			gtk_message_dialog_set_image (GTK_MESSAGE_DIALOG (object),
						      GTK_WIDGET (g_value_get_object (value)));
	}
	else
		GWA_GET_CLASS (GTK_TYPE_WINDOW)->set_property (adaptor, object,
							       id, value);
}

/* ----------------------------- GtkFileChooserWidget ------------------------------ */
void
glade_gtk_file_chooser_widget_post_create (GladeWidgetAdaptor *adaptor,
					   GObject            *object,
					   GladeCreateReason   reason)
{
	gtk_container_forall (GTK_CONTAINER (object),
			      glade_gtk_file_chooser_default_forall,
			      NULL);
}

/* ----------------------------- GtkFontButton ------------------------------ */
/* Use the font-buttons launch dialog to actually set the font-name
 * glade property through the glade-command api.
 */
static void
glade_gtk_font_button_refresh_font_name (GtkFontButton  *button,
					 GladeWidget    *gbutton)
{
	GladeProperty *property;
	
	if ((property =
	     glade_widget_get_property (gbutton, "font-name")) != NULL)
		glade_command_set_property  (property,
					     gtk_font_button_get_font_name (button));
}


/* ----------------------------- GtkColorButton ------------------------------ */
static void
glade_gtk_color_button_refresh_color (GtkColorButton  *button,
				      GladeWidget     *gbutton)
{
	GladeProperty *property;
	GdkColor       color = { 0, };
	
	if ((property = glade_widget_get_property (gbutton, "color")) != NULL)
		glade_command_set_property (property, &color);
}

/* ----------------------------- GtkButton ------------------------------ */
static void
glade_gtk_button_disable_label (GladeWidget *gwidget)
{
	glade_widget_property_set (gwidget, "use-underline", FALSE);

	glade_widget_property_set_sensitive
		(gwidget, "label", FALSE,
		 _("This only applies with label type buttons"));

	glade_widget_property_set_sensitive
		(gwidget, "use-underline", FALSE,
		 _("This only applies with label type buttons"));
}

static void
glade_gtk_button_disable_stock (GladeWidget *gwidget)
{
	glade_widget_property_set (gwidget, "use-stock", FALSE);
	glade_widget_property_set (gwidget, "stock", 0);

	glade_widget_property_set_sensitive
		(gwidget, "stock", FALSE,
		 _("This only applies with stock type buttons"));

	glade_widget_property_set_sensitive
		(gwidget, "image-position", FALSE,
		 _("This only applies with stock type buttons"));

}

static void
glade_gtk_button_restore_container (GladeWidget *gwidget)
{
	GtkWidget *child = GTK_BIN (gwidget->object)->child;
	if (child && glade_widget_get_from_gobject (child) == NULL)
		gtk_container_remove (GTK_CONTAINER (gwidget->object), child);

	if (GTK_BIN (gwidget->object)->child == NULL)
		gtk_container_add (GTK_CONTAINER (gwidget->object), 
				   glade_placeholder_new ());
}

static void
glade_gtk_button_post_create_parse_finished (GladeProject *project,
					     GObject *button)
{
	gboolean     use_stock = FALSE;
	gchar       *label = NULL;
	GEnumValue  *eval;
	GEnumClass  *eclass;
	GladeWidget *gbutton = glade_widget_get_from_gobject (button);
	GladeCreateReason reason;

	eclass   = g_type_class_ref (GLADE_TYPE_STOCK);

	g_object_set_data (button, "glade-button-post-ran", GINT_TO_POINTER (1));
	reason = GPOINTER_TO_INT (g_object_get_data (button, "glade-reason"));

	glade_widget_property_get (gbutton, "use-stock", &use_stock);
	glade_widget_property_get (gbutton, "label", &label);

	if (GTK_BIN (button)->child != NULL && 
	    glade_widget_get_from_gobject (GTK_BIN (button)->child) != NULL)
	{
		glade_widget_property_set (gbutton, "glade-type", GLADEGTK_BUTTON_CONTAINER);
	}
	else if (use_stock)
	{
		if (label != NULL && strcmp (label, "glade-none") != 0 &&
		    (eval = g_enum_get_value_by_nick (eclass, label)) != NULL)
		{
			g_object_set_data (G_OBJECT (gbutton), "stock", 
					   GINT_TO_POINTER (eval->value));

			glade_widget_property_set (gbutton, "stock", eval->value);
		}

		glade_widget_property_set (gbutton, "glade-type", GLADEGTK_BUTTON_STOCK);
	}
	else
		/* Fallback to label type */
		glade_widget_property_set (gbutton, "glade-type", GLADEGTK_BUTTON_LABEL);

	g_type_class_unref (eclass);
}

void
glade_gtk_button_post_create (GladeWidgetAdaptor  *adaptor,
			      GObject             *button, 
			      GladeCreateReason    reason)
{
	GladeWidget *gbutton = glade_widget_get_from_gobject (button);

	g_return_if_fail (GTK_IS_BUTTON (button));
	g_return_if_fail (GLADE_IS_WIDGET (gbutton));

	if (GTK_IS_FONT_BUTTON (button))
		g_signal_connect
			(button, "font-set", 
			 G_CALLBACK (glade_gtk_font_button_refresh_font_name), gbutton);
	else if (GTK_IS_COLOR_BUTTON (button))
		g_signal_connect
			(button, "color-set", 
			 G_CALLBACK (glade_gtk_color_button_refresh_color), gbutton);


	if (GTK_IS_COLOR_BUTTON (button) ||
	    GTK_IS_FONT_BUTTON (button))
		return;

	/* Internal buttons get there stock stuff introspected. */
	if (reason == GLADE_CREATE_USER && gbutton->internal == NULL)
	{
		g_object_set_data (button, "glade-button-post-ran", GINT_TO_POINTER (1));

		glade_widget_property_set (gbutton, "glade-type", GLADEGTK_BUTTON_LABEL);
		glade_project_selection_set (GLADE_PROJECT (gbutton->project), 
					     G_OBJECT (button), TRUE);
	}
	else
	{
		g_object_set_data (button, "glade-reason", GINT_TO_POINTER (reason));
		g_signal_connect (glade_widget_get_project (gbutton),
				  "parse-finished",
				  G_CALLBACK (glade_gtk_button_post_create_parse_finished),
				  button);
	}
}

static void
glade_gtk_button_set_type (GObject *object, const GValue *value)
{
	GladeWidget        *gwidget;
	GladeGtkButtonType  type;
	
	gwidget = glade_widget_get_from_gobject (object);
	g_return_if_fail (GTK_IS_BUTTON (object));
	g_return_if_fail (GLADE_IS_WIDGET (gwidget));

	/* Exit if we're still loading project objects
	 */
	if (GPOINTER_TO_INT (g_object_get_data
			     (object, "glade-button-post-ran")) == 0)
		return;

	type = g_value_get_enum (value);

	switch (type)
	{
	case GLADEGTK_BUTTON_LABEL:
		glade_widget_property_set_sensitive (gwidget, "label", TRUE, NULL);
		glade_widget_property_set_sensitive (gwidget, "use-underline", TRUE, NULL);
		glade_gtk_button_disable_stock (gwidget);
		break;
	case GLADEGTK_BUTTON_STOCK:
		glade_widget_property_set (gwidget, "use-stock", TRUE);
		glade_widget_property_set_sensitive (gwidget, "stock", TRUE, NULL);
		glade_widget_property_set_sensitive (gwidget, "image-position", TRUE, NULL);
		glade_gtk_button_disable_label (gwidget);
		break;
	case GLADEGTK_BUTTON_CONTAINER:

		if (GPOINTER_TO_INT (g_object_get_data
				     (object, "button-type-initially-set")) != 0)
		{
			/* Skip this on the initial setting */
			glade_gtk_button_disable_label (gwidget);
			glade_gtk_button_disable_stock (gwidget);
		}
		else
		{
			/* Initially setting container mode after a load is
			 * a delicate dance.
			 */
			glade_widget_property_set (gwidget, "label", NULL);
			
			glade_widget_property_set_sensitive
				(gwidget, "stock", FALSE,
				 _("This only applies with stock type buttons"));

			glade_widget_property_set_sensitive
				(gwidget, "image-position", FALSE,
				 _("This only applies with stock type buttons"));
			
			glade_widget_property_set_sensitive
				(gwidget, "label", FALSE,
				 _("This only applies with label type buttons"));
			
			glade_widget_property_set_sensitive
				(gwidget, "use-underline", FALSE,
				 _("This only applies with label type buttons"));
		}
		glade_widget_property_set (gwidget, "label", NULL);
		glade_gtk_button_restore_container (gwidget);
		break;
	}		
	g_object_set_data (object, "button-type-initially-set", GINT_TO_POINTER (1));
}

static void
glade_gtk_button_set_stock (GObject *object, const GValue *value)
{
	GladeWidget   *gwidget;
	GEnumClass    *eclass;
	GEnumValue    *eval;
	gint           val;

	gwidget = glade_widget_get_from_gobject (object);
	g_return_if_fail (GTK_IS_BUTTON (object));
	g_return_if_fail (GLADE_IS_WIDGET (gwidget));

	/* Exit if we're still loading project objects
	 */
	if (GPOINTER_TO_INT (g_object_get_data
			     (object, "glade-button-post-ran")) == 0)
		return;

	val = g_value_get_enum (value);	
	if (val == GPOINTER_TO_INT (g_object_get_data (G_OBJECT (gwidget), "stock")))
		return;
	g_object_set_data (G_OBJECT (gwidget), "stock", GINT_TO_POINTER (val));

	eclass   = g_type_class_ref (G_VALUE_TYPE (value));
	if ((eval = g_enum_get_value (eclass, val)) != NULL)
	{
		/* setting to "none", ensure an appropriate label */
		if (val == 0)
			glade_widget_property_set (gwidget, "label", NULL);
		else
		{
			if (GTK_BIN (object)->child)
			{
				/* Here we would delete the coresponding GladeWidget from
				 * the project (like we created it and added it), but this
				 * screws up the undo stack, so instead we keep the stock
				 * button insensitive while ther are usefull children in the
				 * button.
				 */
				gtk_container_remove (GTK_CONTAINER (object), 
						      GTK_BIN (object)->child);
			}
			
			/* Here we should remove any previously added GladeWidgets manualy
			 * and from the project, not to leak them.
			 */
			glade_widget_property_set (gwidget, "label", eval->value_nick);
		}
	}
	g_type_class_unref (eclass);
}

void
glade_gtk_button_set_property (GladeWidgetAdaptor *adaptor,
			       GObject            *object, 
			       const gchar        *id,
			       const GValue       *value)
{
	if (!strcmp (id, "glade-type"))
		glade_gtk_button_set_type (object, value);
	else if (!strcmp (id, "stock"))
		glade_gtk_button_set_stock (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor, object,
								  id, value);
}

void
glade_gtk_button_replace_child (GladeWidgetAdaptor  *adaptor,
				GtkWidget           *container,
				GtkWidget           *current,
				GtkWidget           *new_widget)
{
	GladeWidget *gbutton = glade_widget_get_from_gobject (container);

	g_return_if_fail (GLADE_IS_WIDGET (gbutton));

	GWA_GET_CLASS
		(GTK_TYPE_CONTAINER)->replace_child (adaptor, 
						     G_OBJECT (container), 
						     G_OBJECT (current), 
						     G_OBJECT (new_widget));
	
	if (GLADE_IS_PLACEHOLDER (new_widget))
		glade_widget_property_set_sensitive (gbutton, "glade-type", TRUE, NULL);
	else
		glade_widget_property_set_sensitive (gbutton, "glade-type", FALSE,
						     _("You must remove any children before "
						       "you can set the type"));

}

void
glade_gtk_button_add_child (GladeWidgetAdaptor  *adaptor,
			    GObject             *object, 
			    GObject             *child)
{
	GladeWidget *gwidget;

	if (GTK_BIN (object)->child)
		gtk_container_remove (GTK_CONTAINER (object), 
				      GTK_BIN (object)->child);
	
	gtk_container_add (GTK_CONTAINER (object), GTK_WIDGET (child));

	if (GLADE_IS_PLACEHOLDER (child) == FALSE)
	{
		gwidget = glade_widget_get_from_gobject (object);
		glade_widget_property_set_sensitive (gwidget, "glade-type", FALSE,
						     _("You must remove any children before "
						       "you can set the type"));
	}
}

void
glade_gtk_button_remove_child (GladeWidgetAdaptor  *adaptor,
			       GObject             *object, 
			       GObject             *child)
{
	GladeWidget *gwidget = glade_widget_get_from_gobject (object);

	gtk_container_remove (GTK_CONTAINER (object), GTK_WIDGET (child));
	gtk_container_add (GTK_CONTAINER (object), glade_placeholder_new());

	glade_widget_property_set_sensitive (gwidget, "glade-type", TRUE, NULL);
}

/* ----------------------------- GtkImage ------------------------------ */
static void
glade_gtk_image_disable_filename (GladeWidget *gwidget)
{
	glade_widget_property_set (gwidget, "pixbuf", NULL);
	glade_widget_property_set_sensitive (gwidget, "pixbuf", FALSE,
		 	_("This only applies with file type images"));
}

static void
glade_gtk_image_disable_icon_name (GladeWidget *gwidget)
{
	glade_widget_property_reset (gwidget, "icon-name");
	glade_widget_property_set_sensitive (gwidget, "icon-name", FALSE,
		 	_("This only applies to Icon Theme type images"));
}

static void
glade_gtk_image_disable_stock (GladeWidget *gwidget)
{
	glade_widget_property_set (gwidget, "glade-stock", NULL);
	glade_widget_property_set (gwidget, "stock", NULL);
	glade_widget_property_set_enabled (gwidget, "stock", FALSE);
	glade_widget_property_set_sensitive (gwidget, "glade-stock", FALSE,
		 	_("This only applies with stock type images"));
}

static void 
glade_gtk_image_pixel_size_changed (GladeProperty *property,
				    GValue        *old_value,
				    GValue        *value,
				    GladeWidget   *gimage)
{
	gint size = g_value_get_int (value);
	glade_widget_property_set_sensitive 
		(gimage, "icon-size", size < 0 ? TRUE : FALSE,
		 _("Pixel Size takes precedence over Icon Size; "
		   "if you want to use Icon Size, set Pixel size to -1"));
}

static void
glade_gtk_image_parse_finished (GladeProject *project, GladeWidget *gimage)
{
	GladeProperty *property;
	gint size;
	
	if (glade_widget_property_original_default (gimage, "icon-name") == FALSE)
		glade_widget_property_set (gimage, "glade-type", GLADEGTK_IMAGE_ICONTHEME);
	else if (glade_widget_property_original_default (gimage, "stock") == FALSE)
		glade_widget_property_set (gimage, "glade-type", GLADEGTK_IMAGE_STOCK);
	else if (glade_widget_property_original_default (gimage, "pixbuf") == FALSE)
		glade_widget_property_set (gimage, "glade-type", GLADEGTK_IMAGE_FILENAME);
	else 
		glade_widget_property_reset (gimage, "glade-type");

	if ((property = glade_widget_get_property (gimage, "pixel-size")) == NULL)
		return;

	glade_widget_property_get (gimage, "pixel-size", &size);
	
	if (size >= 0)
		glade_widget_property_set_sensitive (gimage, "icon-size", FALSE,
				 _("Pixel Size takes precedence over Icon size"));
	
	g_signal_connect (G_OBJECT (property), "value-changed", 
			  G_CALLBACK (glade_gtk_image_pixel_size_changed),
			  gimage);
}

void
glade_gtk_image_post_create (GladeWidgetAdaptor  *adaptor,
			     GObject             *object, 
			     GladeCreateReason    reason)
{
	GladeWidget *gimage;
	
	if (reason == GLADE_CREATE_LOAD)
	{
		gimage = glade_widget_get_from_gobject (object);

		g_signal_connect (glade_widget_get_project (gimage),
				  "parse-finished",
				  G_CALLBACK (glade_gtk_image_parse_finished),
				  gimage);
	}
}

static void
glade_gtk_image_set_icon_name (GObject *object, const GValue *value)
{
	GladeWidget *gimage;
	gint icon_size;
	
	g_return_if_fail (GTK_IS_IMAGE (object));
	gimage = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gimage));
	
	glade_widget_property_get (gimage, "icon-size", &icon_size);
	
	gtk_image_set_from_icon_name (GTK_IMAGE (object),
				      g_value_get_string (value), 
				      icon_size);
}

static void
glade_gtk_image_refresh (GladeWidget *gwidget, const gchar *property)
{
	gpointer p;
	glade_widget_property_set_sensitive (gwidget, property, TRUE, NULL);
	glade_widget_property_get (gwidget, property, &p);
	glade_widget_property_set (gwidget, property, p);
}

static void
glade_gtk_image_set_type (GObject *object, const GValue *value)
{
	GladeWidget       *gwidget;
	GladeGtkImageType  type;
	
	gwidget = glade_widget_get_from_gobject (object);
	g_return_if_fail (GTK_IS_IMAGE (object));
	g_return_if_fail (GLADE_IS_WIDGET (gwidget));

	/* Exit if we're still loading project objects */
	if (glade_util_object_is_loading (object)) return;
	
	switch ((type = g_value_get_enum (value)))
	{
		case GLADEGTK_IMAGE_STOCK:
			glade_gtk_image_disable_filename (gwidget);
			glade_gtk_image_disable_icon_name (gwidget);
			glade_widget_property_set_enabled (gwidget, "stock", TRUE);
			glade_gtk_image_refresh (gwidget, "glade-stock");
		break;

		case GLADEGTK_IMAGE_ICONTHEME:
			glade_gtk_image_disable_filename (gwidget);
			glade_gtk_image_disable_stock (gwidget);
			glade_gtk_image_refresh (gwidget, "icon-name");
		break;

		case GLADEGTK_IMAGE_FILENAME:
		default:
			glade_gtk_image_disable_stock (gwidget);
			glade_gtk_image_disable_icon_name (gwidget);
			glade_gtk_image_refresh (gwidget, "pixbuf");
		break;
	}
}

/* This basicly just sets the virtual "glade-stock" property
 * based on the image's "stock" property (for glade file loading purposes)
 */
static void
glade_gtk_image_set_stock (GObject *object, const GValue *value)
{
	GladeWidget *gwidget;
	gchar       *str;
	gint         icon_size;

	g_return_if_fail (GTK_IS_IMAGE (object));
	gwidget = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gwidget));

	if ((str = g_value_dup_string (value)) &&
	    glade_util_object_is_loading (object))
	{
		GEnumClass *eclass = g_type_class_ref (GLADE_TYPE_STOCK_IMAGE);
		GEnumValue *eval;
		
		if ((eval = g_enum_get_value_by_nick (eclass, str)) != NULL)
			glade_widget_property_set (gwidget, "glade-stock",
						   eval->value);
		g_type_class_unref (eclass);
	}

	if (str == NULL && glade_widget_superuser ()) return;
	
	/* Set the real property */
	glade_widget_property_get (gwidget, "icon-size", &icon_size);
	gtk_image_set_from_stock (GTK_IMAGE (object), str, icon_size);

	/* Sometimes it gets recursive around here, valgrind says
	 * we should dup a string for this purpose ;-)
	 */
	g_free (str);
}

static void
glade_gtk_image_set_glade_stock (GObject *object, const GValue *value)
{
	GladeWidget   *gwidget;
	GEnumClass    *eclass;
	GEnumValue    *eval;
	gint           val;

	g_return_if_fail (GTK_IS_IMAGE (object));
	gwidget = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gwidget));
	
	/* This is triggered by glade_widget_sync_custom_props () from glade_widget_new_from_widget_info()  
	    which makes "stock" property to reset */
	if (glade_util_object_is_loading (object)) return;
	
	val    = g_value_get_enum (value);	
	eclass = g_type_class_ref (G_VALUE_TYPE (value));
	if ((eval = g_enum_get_value (eclass, val)) != NULL)
	{
		if (val == 0)
			glade_widget_property_reset (gwidget, "stock");
		else
			glade_widget_property_set (gwidget, "stock", eval->value_nick);
			
	}
	g_type_class_unref (eclass);
}

void
glade_gtk_image_set_property (GladeWidgetAdaptor *adaptor,
			      GObject            *object, 
			      const gchar        *id,
			      const GValue       *value)
{
	if (!strcmp (id, "glade-type"))
		glade_gtk_image_set_type (object, value);
	else if (!strcmp (id, "stock"))
		glade_gtk_image_set_stock (object, value);
	else if (!strcmp (id, "glade-stock"))
		glade_gtk_image_set_glade_stock (object, value);
	else if (!strcmp (id, "icon-name"))
		glade_gtk_image_set_icon_name (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_WIDGET)->set_property (adaptor, object,
							       id, value);
}

/* ----------------------------- GtkMenu ------------------------------ */
GObject *
glade_gtk_menu_constructor (GType                  type,
			    guint                  n_construct_properties,
			    GObjectConstructParam *construct_properties)
{
	GladeWidgetAdaptor   *adaptor;
	GObject              *ret_obj;
	
	ret_obj = GWA_GET_OCLASS(GTK_TYPE_CONTAINER)->constructor
		(type, n_construct_properties, construct_properties);

	adaptor = GLADE_WIDGET_ADAPTOR (ret_obj);

	glade_widget_adaptor_action_remove (adaptor, "add_parent");
	glade_widget_adaptor_action_remove (adaptor, "remove_parent");

	return ret_obj;
}


/* ----------------------------- GtkMenuShell ------------------------------ */
void
glade_gtk_menu_shell_add_child (GladeWidgetAdaptor  *adaptor, 
			       GObject             *object, 
			       GObject             *child)
{
	
	g_return_if_fail (GTK_IS_MENU_SHELL (object));
	g_return_if_fail (GTK_IS_MENU_ITEM (child));

	gtk_menu_shell_append (GTK_MENU_SHELL (object), GTK_WIDGET (child));
}


void
glade_gtk_menu_shell_remove_child (GladeWidgetAdaptor  *adaptor,
				  GObject             *object, 
				  GObject             *child)
{
	g_return_if_fail (GTK_IS_MENU_SHELL (object));
	g_return_if_fail (GTK_IS_MENU_ITEM (child));
	
	gtk_container_remove (GTK_CONTAINER (object), GTK_WIDGET (child));
}

static gint
glade_gtk_menu_shell_get_item_position (GObject *container, GObject *child)
{
	gint position = 0;
	GList *list = GTK_MENU_SHELL (container)->children;
	
	while (list)
	{
		if (G_OBJECT (list->data) == child) break;
		
		list = list->next;
		position++;
	}
	
	return position;
}

void
glade_gtk_menu_shell_get_child_property (GladeWidgetAdaptor  *adaptor,
					 GObject             *container,
					 GObject             *child,
					 const gchar         *property_name,
					 GValue              *value)
{
	g_return_if_fail (GTK_IS_MENU_SHELL (container));
	g_return_if_fail (GTK_IS_MENU_ITEM (child));
	
	if (strcmp (property_name, "position") == 0)
	{
		g_value_set_int (value, 
			 glade_gtk_menu_shell_get_item_position (container,
								 child));
	}
	else
		/* Chain Up */
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_get_property (adaptor, 
								  container, 
								  child, 
								  property_name, 
								  value);
}

void
glade_gtk_menu_shell_set_child_property (GladeWidgetAdaptor  *adaptor,
					 GObject             *container,
					 GObject             *child,
					 const gchar         *property_name,
					 GValue              *value)
{
	g_return_if_fail (GTK_IS_MENU_SHELL (container));
	g_return_if_fail (GTK_IS_MENU_ITEM (child));
	g_return_if_fail (property_name != NULL || value != NULL);
	
	if (strcmp (property_name, "position") == 0)
	{
		GladeWidget *gitem;
		gint position;
		
		gitem = glade_widget_get_from_gobject (child);
		g_return_if_fail (GLADE_IS_WIDGET (gitem));
	
		position = g_value_get_int (value);
	
		if (position < 0)
		{
			position = glade_gtk_menu_shell_get_item_position (container, child);
			g_value_set_int (value, position);
		}
	
		g_object_ref (child);
		gtk_container_remove (GTK_CONTAINER (container), GTK_WIDGET (child));
		gtk_menu_shell_insert (GTK_MENU_SHELL (container), GTK_WIDGET (child), position);
		g_object_unref (child);

	}
	else
		/* Chain Up */
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_set_property (adaptor, 
								  container, child,
								  property_name, value);
}

static gchar *
glade_gtk_menu_shell_get_display_name (GladeBaseEditor *editor,
				       GladeWidget *gchild,
				       gpointer user_data)
{
	GObject *child = glade_widget_get_object (gchild);
	gchar *name;
	
	if (GTK_IS_SEPARATOR_MENU_ITEM (child))
		name = _("<separator>");
	else
		glade_widget_property_get (gchild, "label", &name);
	
	return g_strdup (name);
}

static GladeWidget *
glade_gtk_menu_shell_item_get_parent (GladeWidget *gparent, GObject *parent)
{
	GtkWidget *submenu;
	
	if ((submenu = gtk_menu_item_get_submenu (GTK_MENU_ITEM (parent))))
		gparent = glade_widget_get_from_gobject (submenu);
	else
		gparent = glade_command_create (glade_widget_adaptor_get_by_type (GTK_TYPE_MENU),
						gparent, NULL, glade_widget_get_project (gparent));
	return gparent;
}

static GladeWidget *  
glade_gtk_menu_shell_build_child (GladeBaseEditor *editor,
				  GladeWidget *gparent,
				  GType type,
				  gpointer data)
{
	GObject *parent = glade_widget_get_object (gparent);
	GladeWidget *gitem_new;

	if (GTK_IS_SEPARATOR_MENU_ITEM (parent))
		return NULL;
	
	/* Get or build real parent */
	if (GTK_IS_MENU_ITEM (parent))
		gparent = glade_gtk_menu_shell_item_get_parent (gparent, parent);
	
	/* Build child */
	gitem_new = glade_command_create (glade_widget_adaptor_get_by_type (type),
					  gparent, NULL, 
					  glade_widget_get_project (gparent));
	
	if (type != GTK_TYPE_SEPARATOR_MENU_ITEM)
	{
		glade_widget_property_set (gitem_new, "label",
					   glade_widget_get_name (gitem_new));
		glade_widget_property_set (gitem_new, "use-underline", TRUE);
	}
	
	return gitem_new;
}

static gboolean
glade_gtk_menu_shell_delete_child (GladeBaseEditor *editor,
				   GladeWidget *gparent,
				   GladeWidget *gchild,
				   gpointer data)
{
	GObject *item = glade_widget_get_object (gparent);
	GtkWidget *submenu = NULL;
	GList list = {0, };
	gint n_children = 0;
	
	if (GTK_IS_MENU_ITEM (item) &&
	    (submenu = gtk_menu_item_get_submenu (GTK_MENU_ITEM (item))))
	{
		GList *l = gtk_container_get_children (GTK_CONTAINER (submenu));
		n_children = g_list_length (l);
		g_list_free (l);
	}
		
	if (submenu && n_children == 1)
		list.data = glade_widget_get_parent (gchild);
	else
		list.data = gchild;
	
	/* Remove widget */
	glade_command_delete (&list);
	
	return TRUE;
}

static gboolean
glade_gtk_menu_shell_move_child (GladeBaseEditor *editor,
				 GladeWidget *gparent,
				 GladeWidget *gchild,
				 gpointer data)
{
	GObject *parent = glade_widget_get_object (gparent);
	GList list = {0, };
	
	if (GTK_IS_SEPARATOR_MENU_ITEM (parent)) return FALSE;
	
	if (GTK_IS_MENU_ITEM (parent))
		gparent = glade_gtk_menu_shell_item_get_parent (gparent, parent);
	
	if (gparent != glade_widget_get_parent (gchild))
	{
		list.data = gchild;
		glade_command_dnd (&list, gparent, NULL);
	}
	
	return TRUE;
}

static gboolean
glade_gtk_menu_shell_change_type (GladeBaseEditor *editor,
				  GladeWidget *gchild,
				  GType type,
				  gpointer data)
{
	GObject *child = glade_widget_get_object (gchild);
	
	if (type == GTK_TYPE_SEPARATOR_MENU_ITEM &&
	    gtk_menu_item_get_submenu (GTK_MENU_ITEM (child)))
		return TRUE;
		
	return FALSE;
}

static void
glade_gtk_menu_shell_child_selected (GladeBaseEditor *editor,
				     GladeWidget *gchild,
				     gpointer data)
{
	GObject *child = glade_widget_get_object (gchild);
	GType type = G_OBJECT_TYPE (child);
	
	glade_base_editor_add_label (editor, "Menu Item");
	
	glade_base_editor_add_default_properties (editor, gchild);
	
	if (GTK_IS_SEPARATOR_MENU_ITEM (child)) return;
	
	glade_base_editor_add_label (editor, "Properties");
	
	glade_base_editor_add_properties (editor, gchild, FALSE, "label", "tooltip", NULL);

	if (type == GTK_TYPE_IMAGE_MENU_ITEM)
	{
		GtkWidget *image;
		GladeWidget *internal;

		glade_base_editor_add_properties (editor, gchild, FALSE, "stock", NULL);
		
		if ((image = gtk_image_menu_item_get_image (GTK_IMAGE_MENU_ITEM (child))))
		{
			if ((internal = glade_widget_get_from_gobject (image)) &&
				internal->internal)
			{
				glade_base_editor_add_label
					(editor, "Internal Image Properties");
				glade_base_editor_add_properties
					(editor, internal, FALSE, "glade-type", "pixbuf", 
					 "glade-stock", "icon-name", NULL);
			}
		}
	}
	else if (type == GTK_TYPE_CHECK_MENU_ITEM)
		glade_base_editor_add_properties (editor, gchild, FALSE,
						  "active", "draw-as-radio",
						  "inconsistent", NULL);
	else if (type == GTK_TYPE_RADIO_MENU_ITEM)
		glade_base_editor_add_properties (editor, gchild, FALSE,
						  "active", "group", NULL);
}

static void
glade_gtk_menu_shell_launch_editor (GObject *object, gchar *title)
{
	GladeBaseEditor *editor;
	GtkWidget *window;

	/* Editor */
	editor = glade_base_editor_new (object, TRUE,
					_("Normal"), GTK_TYPE_MENU_ITEM,
					_("Image"), GTK_TYPE_IMAGE_MENU_ITEM,
					_("Check"), GTK_TYPE_CHECK_MENU_ITEM,
					_("Radio"), GTK_TYPE_RADIO_MENU_ITEM,
					_("Separator"), GTK_TYPE_SEPARATOR_MENU_ITEM,
					NULL);

	glade_base_editor_add_popup_items (editor,
					  _("Add Item"), GTK_TYPE_MENU_ITEM, FALSE,
					  _("Add Child Item"), GTK_TYPE_MENU_ITEM, TRUE,
					  _("Add Separator"), GTK_TYPE_SEPARATOR_MENU_ITEM, FALSE,
					  NULL);
	
	g_signal_connect (editor, "get-display-name", G_CALLBACK (glade_gtk_menu_shell_get_display_name), NULL);
	g_signal_connect (editor, "child-selected", G_CALLBACK (glade_gtk_menu_shell_child_selected), NULL);
	g_signal_connect (editor, "change-type", G_CALLBACK (glade_gtk_menu_shell_change_type), NULL);
	g_signal_connect (editor, "build-child", G_CALLBACK (glade_gtk_menu_shell_build_child), NULL);
	g_signal_connect (editor, "delete-child", G_CALLBACK (glade_gtk_menu_shell_delete_child), NULL);
	g_signal_connect (editor, "move-child", G_CALLBACK (glade_gtk_menu_shell_move_child), NULL);

	gtk_widget_show (GTK_WIDGET (editor));
	
	window = glade_base_editor_pack_new_window (editor, title,
			    _("<big><b>Tips:</b></big>\n"
			      "  * Right click over the treeview to add items.\n"
			      "  * Press Delete to remove the selected item.\n"
			      "  * Drag &amp; Drop to reorder.\n"
			      "  * Type column is editable."));
	gtk_widget_show (window);
}

void
glade_gtk_menu_shell_action_activate (GladeWidgetAdaptor *adaptor,
				      GObject *object,
				      const gchar *action_path)
{
	if (strcmp (action_path, "launch_editor") == 0)
	{
		if (GTK_IS_MENU_BAR (object))
			glade_gtk_menu_shell_launch_editor (object, _("Edit Menu Bar"));
		else if (GTK_IS_MENU (object))
			glade_gtk_menu_shell_launch_editor (object, _("Edit Menu"));
	}
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->action_activate (adaptor,
								     object,
								     action_path);
}

/* ----------------------------- GtkMenuItem(s) ------------------------------ */
GObject *
glade_gtk_menu_item_constructor (GType                  type,
				 guint                  n_construct_properties,
				 GObjectConstructParam *construct_properties)
{
	GladeWidgetAdaptor   *adaptor;
	GObject              *ret_obj;

	ret_obj = GWA_GET_OCLASS(GTK_TYPE_CONTAINER)->constructor
		(type, n_construct_properties, construct_properties);

	adaptor = GLADE_WIDGET_ADAPTOR (ret_obj);

	glade_widget_adaptor_action_remove (adaptor, "add_parent");
	glade_widget_adaptor_action_remove (adaptor, "remove_parent");

	return ret_obj;
}

GList *
glade_gtk_menu_item_get_children (GladeWidgetAdaptor *adaptor,
				 GObject *object)
{
	GList *list = NULL;
	GtkWidget *child;
	
	g_return_val_if_fail (GTK_IS_MENU_ITEM (object), NULL);
	
	if ((child = gtk_menu_item_get_submenu (GTK_MENU_ITEM (object))))
		list = g_list_append (list, child);
	
	if (GTK_IS_IMAGE_MENU_ITEM (object) &&
	    (child = gtk_image_menu_item_get_image (GTK_IMAGE_MENU_ITEM (object))))
		list = g_list_append (list, child);

	return list;
}

void
glade_gtk_menu_item_add_child (GladeWidgetAdaptor *adaptor,
			       GObject *object, GObject *child)
{
	g_return_if_fail (GTK_IS_MENU_ITEM (object));
	g_return_if_fail (GTK_IS_MENU (child));
	
	if (GTK_IS_SEPARATOR_MENU_ITEM (object))
	{
		g_warning ("You shouldn't try to add a GtkMenu to a GtkSeparatorMenuItem");
		return;
	}
	
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (object), GTK_WIDGET (child));
}

void
glade_gtk_menu_item_remove_child (GladeWidgetAdaptor *adaptor,
				  GObject *object, GObject *child)
{
	g_return_if_fail (GTK_IS_MENU_ITEM (object));
	g_return_if_fail (GTK_IS_MENU (child));
	
	gtk_menu_item_remove_submenu (GTK_MENU_ITEM (object));
}

#define glade_return_if_re_entrancy(o,p,v) \
	if ((v) == GPOINTER_TO_INT (g_object_get_data (G_OBJECT (o), p))) return; g_object_set_data (G_OBJECT (o), p, GINT_TO_POINTER ((v)))
	
void
glade_gtk_menu_item_post_create (GladeWidgetAdaptor *adaptor, 
				 GObject            *object, 
				 GladeCreateReason   reason)
{
	GladeWidget  *gitem, *gimage;

	g_return_if_fail (GTK_IS_MENU_ITEM (object));
	gitem = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gitem));
	
	if (GTK_IS_SEPARATOR_MENU_ITEM (object)) return;
	
	if (gtk_bin_get_child (GTK_BIN (object)) == NULL)
	{
		GtkWidget *label = gtk_label_new ("");
		gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
		gtk_container_add (GTK_CONTAINER (object), label);
	}

	if (GTK_IS_IMAGE_MENU_ITEM (object))
	{
		gboolean use_stock;
	
		glade_widget_property_get (gitem, "use-stock", &use_stock);
		if (use_stock)
		{
			GEnumClass *eclass;
			GEnumValue *eval;
			gchar *label;
			
			glade_widget_property_get (gitem, "label", &label);
			
			eclass = g_type_class_ref (GLADE_TYPE_STOCK);
			eval = g_enum_get_value_by_nick (eclass, label);
			if (eval)
				glade_widget_property_set(gitem, "stock", eval->value);
			
			glade_widget_property_set (gitem, "use-underline", TRUE);
		}
		else
		{
			if (reason == GLADE_CREATE_USER)
			{
				GtkWidget *image = gtk_image_new ();

				gimage = glade_widget_adaptor_create_internal
					(gitem, G_OBJECT (image),
					 "image", "menu-item", FALSE, reason);
				gtk_image_menu_item_set_image
					(GTK_IMAGE_MENU_ITEM (object), image);
			}
		}
	}
}

static void
glade_gtk_menu_item_set_label (GObject *object, const GValue *value)
{
	GladeWidget *gitem;
	GtkWidget *label;
	gboolean use_underline, use_stock;
	const gchar *label_str, *last_label_str;

	g_return_if_fail (GTK_IS_MENU_ITEM (object));
	gitem = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gitem));
	
	if (GTK_IS_SEPARATOR_MENU_ITEM (object)) return;

	label_str = g_value_get_string (value);
	
	last_label_str = g_object_get_data (G_OBJECT (gitem), "label");
	if (last_label_str)
		if (strcmp(label_str, last_label_str) == 0) return;
	g_object_set_data_full (G_OBJECT (gitem), "label", g_strdup(label_str), g_free);

	if (GTK_IS_IMAGE_MENU_ITEM (object))
	{
		glade_widget_property_get (gitem, "use-stock", &use_stock);
		
		if (use_stock)
		{
			GtkWidget *image;
			GEnumClass *eclass;
			GEnumValue *eval;
			
			eclass = g_type_class_ref (GLADE_TYPE_STOCK);
			eval = g_enum_get_value_by_nick (eclass, label_str);
			
			if (eval)
			{
				label_str = eval->value_name;
			
				image = gtk_image_new_from_stock (eval->value_nick, GTK_ICON_SIZE_MENU);
				gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (object), image);
			}
			
			g_type_class_unref (eclass);
		}
	}
	
	label = gtk_bin_get_child (GTK_BIN (object));
	gtk_label_set_text (GTK_LABEL (label), label_str);
	
	glade_widget_property_get (gitem, "use-underline", &use_underline);
	gtk_label_set_use_underline (GTK_LABEL (label), use_underline);
}

static void
glade_gtk_menu_item_set_use_underline (GObject *object, const GValue *value)
{
	GtkMenuItem *item;
	GtkWidget *label;

	g_return_if_fail (GTK_IS_MENU_ITEM (object));

	item = GTK_MENU_ITEM (object);

	if (GTK_IS_SEPARATOR_MENU_ITEM (item)) return;
	
	label = gtk_bin_get_child (GTK_BIN (item));

	gtk_label_set_use_underline (GTK_LABEL (label), g_value_get_boolean (value));
}

GObject *
glade_gtk_image_menu_item_get_internal_child (GladeWidgetAdaptor *adaptor,
					      GObject            *parent,
					      const gchar        *name);

static void
glade_gtk_menu_item_set_stock_item (GObject *object, const GValue *value)
{
	GladeWidget *gitem, *gimage;
	GEnumClass *eclass;
	GEnumValue *eval;
	gint val;
	gchar *label, *icon;
	GObject *image;
	gboolean is_image_item;
	
	g_return_if_fail (GTK_IS_MENU_ITEM (object));

	if ((val = g_value_get_enum (value)) == GNOMEUIINFO_MENU_NONE)
		return;
	
	eclass = g_type_class_ref (G_VALUE_TYPE (value));
	if ((eval = g_enum_get_value (eclass, val)) == NULL)
	{
		g_type_class_unref (eclass);
		return;
	}

	g_type_class_unref (eclass);
	
	/* set use-underline */
	gitem = glade_widget_get_from_gobject (object);
	glade_widget_property_set (gitem, "use-underline", TRUE);
	
	is_image_item = GTK_IS_IMAGE_MENU_ITEM (object);
	
	/* If its a GtkImageMenuItem */
	if (is_image_item && eval->value_nick)
	{
		glade_widget_property_set (gitem, "use-stock", TRUE);
		glade_widget_property_set (gitem, "label", eval->value_nick);		
		return;
	}
	
	icon = NULL;
	switch (val)
	{
		case GNOMEUIINFO_MENU_PRINT_SETUP_ITEM:
			icon = "gtk-print";
			label = _("Print S_etup");
		break;
		case GNOMEUIINFO_MENU_FIND_AGAIN_ITEM:
			icon = "gtk-find";
			label = _("Find Ne_xt");
		break;
		case GNOMEUIINFO_MENU_UNDO_MOVE_ITEM:
			icon = "gtk-undo";
			label = _("_Undo Move");
		break;
		case GNOMEUIINFO_MENU_REDO_MOVE_ITEM:
			icon = "gtk-redo";
			label = _("_Redo Move");
		break;
		case GNOMEUIINFO_MENU_SELECT_ALL_ITEM:
			label =  _("Select _All");
		break;			
		case GNOMEUIINFO_MENU_NEW_GAME_ITEM:
			label =  _("_New Game");
		break;
		case GNOMEUIINFO_MENU_PAUSE_GAME_ITEM:
			label =  _("_Pause game");
		break;
		case GNOMEUIINFO_MENU_RESTART_GAME_ITEM:
			label =  _("_Restart Game");
		break;
		case GNOMEUIINFO_MENU_HINT_ITEM:
			label =  _("_Hint"); 
		break;
		case GNOMEUIINFO_MENU_SCORES_ITEM:
			label =  _("_Scores...");
		break;
		case GNOMEUIINFO_MENU_END_GAME_ITEM:
			label =  _("_End Game");
		break;
		case GNOMEUIINFO_MENU_NEW_WINDOW_ITEM:
			label =  _("Create New _Window");
		break;
		case GNOMEUIINFO_MENU_CLOSE_WINDOW_ITEM:
			label =  _("_Close This Window");
		break;
		case GNOMEUIINFO_MENU_FILE_TREE:
			label =  _("_File");
		break;
		case GNOMEUIINFO_MENU_EDIT_TREE:
			label =  _("_Edit");
		break;
		case GNOMEUIINFO_MENU_VIEW_TREE:
			label =  _("_View");
		break;
		case GNOMEUIINFO_MENU_SETTINGS_TREE:
			label =  _("_Settings");
		break;
		case GNOMEUIINFO_MENU_FILES_TREE:
			label =  _("Fi_les");
		break;
		case GNOMEUIINFO_MENU_WINDOWS_TREE:
			label =  _("_Windows");
		break;
		case GNOMEUIINFO_MENU_HELP_TREE:
			label =  _("_Help");
		break;
		case GNOMEUIINFO_MENU_GAME_TREE:
			label =  _("_Game");
		break;
		default:
			return;
		break;
	}

	if (is_image_item && icon)
	{
		eclass = g_type_class_ref (GLADE_TYPE_STOCK);
		eval = g_enum_get_value_by_nick (eclass, icon);
		g_type_class_unref (eclass);
	
		image = glade_gtk_image_menu_item_get_internal_child
			(gitem->adaptor, object, "image");

		gimage = glade_widget_get_from_gobject (image);
		glade_widget_property_set (gimage, "icon-size", GTK_ICON_SIZE_MENU);
		glade_widget_property_set (gimage, "glade-stock", eval->value);
	}
	
	glade_widget_property_set (gitem, "label", label);
}

void
glade_gtk_menu_item_set_property (GladeWidgetAdaptor *adaptor,
				  GObject            *object, 
				  const gchar        *id,
				  const GValue       *value)
{
	if (!strcmp (id, "use-underline"))
		glade_gtk_menu_item_set_use_underline (object, value);
	else if (!strcmp (id, "label"))
		glade_gtk_menu_item_set_label (object, value);
	else if (!strcmp (id, "stock-item"))
		glade_gtk_menu_item_set_stock_item (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor, object,
								  id, value);
}

GObject *
glade_gtk_image_menu_item_get_internal_child (GladeWidgetAdaptor *adaptor,
					      GObject            *parent,
					      const gchar        *name)
{
	GtkWidget *image;
	GObject   *child = NULL;

	if (GTK_IS_IMAGE_MENU_ITEM (parent) && strcmp (name, "image") == 0)
	{
		image = gtk_image_menu_item_get_image (GTK_IMAGE_MENU_ITEM (parent));
		if (image == NULL)
		{
			GladeWidget  *gitem, *gimage;
			
			gitem = glade_widget_get_from_gobject (parent);
			image = gtk_image_new ();

			gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (parent), image);
			
			gimage = glade_widget_adaptor_create_internal
				(gitem, G_OBJECT (image), "image", "menu-item",
				 FALSE, GLADE_CREATE_LOAD);
		}
		child = G_OBJECT (image);
	}

	return child;
}

static void
glade_gtk_image_menu_item_set_use_stock (GObject *object, const GValue *value)
{
	GladeWidget  *gitem, *gimage;
	gboolean      use_stock;
	GtkWidget    *image;
	
	g_return_if_fail (GTK_IS_IMAGE_MENU_ITEM (object));
	gitem = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gitem));

	use_stock = g_value_get_boolean (value);
	
	glade_return_if_re_entrancy (gitem, "use-stock", use_stock);

	if ((image = gtk_image_menu_item_get_image (GTK_IMAGE_MENU_ITEM (object))))
		if(glade_widget_get_from_gobject (G_OBJECT(image)))
		{
			glade_project_remove_object (glade_widget_get_project (gitem), G_OBJECT(image));
			gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (object), NULL);
		}
	
	if (use_stock)
	{
		glade_widget_property_set_sensitive (gitem, "label", FALSE,
					_("This does not apply with stock items"));
	}
	else
	{
		image = gtk_image_new ();
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (object), image);
		gimage = glade_widget_adaptor_create_internal
			(gitem, G_OBJECT (image), "image", "menu-item", FALSE,
			 GLADE_CREATE_LOAD);
		glade_project_add_object (glade_widget_get_project (gitem), 
					  NULL, G_OBJECT (image));

		glade_widget_property_set_sensitive (gitem, "label", TRUE, NULL);
	}
}

static void
glade_gtk_image_menu_item_set_stock (GObject *object, const GValue *value)
{
	GladeWidget *gitem;
	GEnumClass *eclass;
	GEnumValue *eval;
	gint val;
	
	g_return_if_fail (GTK_IS_IMAGE_MENU_ITEM (object));
	gitem = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gitem));
	
	val = g_value_get_enum (value);
	
	glade_return_if_re_entrancy (gitem, "stock", val);
	
	glade_widget_property_set (gitem, "use-stock", val);
		
	eclass = g_type_class_ref (GLADE_TYPE_STOCK);
	eval = g_enum_get_value (eclass, val);
	if (eval && val)
		glade_widget_property_set (gitem, "label", eval->value_nick);
	
	g_type_class_unref (eclass);
}

void
glade_gtk_image_menu_item_set_property (GladeWidgetAdaptor *adaptor,
					GObject            *object, 
					const gchar        *id,
					const GValue       *value)
{
	if (!strcmp (id, "stock"))
		glade_gtk_image_menu_item_set_stock (object, value);
	else if (!strcmp (id, "use-stock"))
		glade_gtk_image_menu_item_set_use_stock (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_MENU_ITEM)->set_property (adaptor, object,
								  id, value);
}

static void
glade_gtk_radio_menu_item_set_group (GObject *object, const GValue *value)
{
	GObject *val;
	
	g_return_if_fail (GTK_IS_RADIO_MENU_ITEM (object));
	
	if ((val = g_value_get_object (value)))
	{
		GSList *group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (val));
		
		if (! g_slist_find (group, GTK_RADIO_MENU_ITEM (object)))
			gtk_radio_menu_item_set_group (GTK_RADIO_MENU_ITEM (object), group);
	}
}

void
glade_gtk_radio_menu_item_set_property (GladeWidgetAdaptor *adaptor,
					GObject            *object, 
					const gchar        *id,
					const GValue       *value)
{

	if (!strcmp (id, "group"))
		glade_gtk_radio_menu_item_set_group (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_MENU_ITEM)->set_property (adaptor, object,
								  id, value);
}

void
glade_gtk_menu_item_action_activate (GladeWidgetAdaptor *adaptor,
				     GObject *object,
				     const gchar *action_path)
{
	if (strcmp (action_path, "launch_editor") == 0)
	{
		GladeWidget *w = glade_widget_get_from_gobject (object);
		
		while ((w = glade_widget_get_parent (w)))
		{
			GObject *obj = glade_widget_get_object (w);
			if (GTK_IS_MENU_SHELL (obj)) object = obj;
		}
		
		if (GTK_IS_MENU_BAR (object))
			glade_gtk_menu_shell_launch_editor (object, _("Edit Menu Bar"));
		else if (GTK_IS_MENU (object))
			glade_gtk_menu_shell_launch_editor (object, _("Edit Menu"));
	}
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->action_activate (adaptor,
								     object,
								     action_path);
}

/* ----------------------------- GtkMenuBar ------------------------------ */
static GladeWidget * 
glade_gtk_menu_bar_append_new_submenu (GladeWidget *parent, GladeProject *project)
{
	static GladeWidgetAdaptor *submenu_adaptor = NULL;
	GladeWidget *gsubmenu;
	
	if (submenu_adaptor == NULL)
		submenu_adaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_MENU);

	gsubmenu = glade_widget_adaptor_create_widget (submenu_adaptor, FALSE,
						     "parent", parent, 
						     "project", project, 
						     NULL);

	glade_widget_add_child (parent, gsubmenu, FALSE);

	return gsubmenu;
}

static GladeWidget * 
glade_gtk_menu_bar_append_new_item (GladeWidget *parent,
				    GladeProject *project,
				    const gchar *label,
				    gboolean use_stock)
{
	static GladeWidgetAdaptor *item_adaptor = NULL, *image_item_adaptor, *separator_adaptor;
	GladeWidget *gitem;
	
	if (item_adaptor == NULL)
	{
		item_adaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_MENU_ITEM);
		image_item_adaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_IMAGE_MENU_ITEM);
		separator_adaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_SEPARATOR_MENU_ITEM);
	}

	if (label)
	{
		gitem = glade_widget_adaptor_create_widget ((use_stock) ? image_item_adaptor : item_adaptor,
							  FALSE, "parent", parent,
							  "project", project, 
							  NULL);

		glade_widget_property_set (gitem, "use-underline", TRUE);
	
		if (use_stock)
		{
			GEnumClass *eclass;
			GEnumValue *eval;
		
			eclass = g_type_class_ref (GLADE_TYPE_STOCK);
			eval = g_enum_get_value_by_nick (eclass, label);
		
			if (eval)
				glade_widget_property_set (gitem, "stock", eval->value);
		
			g_type_class_unref (eclass);
		}
		else
		{
			glade_widget_property_set (gitem, "label", label);
		}
	}
	else
	{
		gitem = glade_widget_adaptor_create_widget (separator_adaptor,
							  FALSE, "parent", parent,
							  "project", project, 
							  NULL);
	}
	
	glade_widget_add_child (parent, gitem, FALSE);
	
	return gitem;
}

void
glade_gtk_menu_bar_post_create (GladeWidgetAdaptor *adaptor,
				GObject            *object, 
				GladeCreateReason   reason)
{
	GladeProject *project;
	GladeWidget *gmenubar, *gitem, *gsubmenu;
	
	g_return_if_fail (GTK_IS_MENU_BAR (object));
	gmenubar = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gmenubar));
	
	if (reason != GLADE_CREATE_USER) return;
	
	project = glade_widget_get_project (gmenubar);
	
	/* File */
	gitem = glade_gtk_menu_bar_append_new_item (gmenubar, project, _("_File"), FALSE);	
	gsubmenu = glade_gtk_menu_bar_append_new_submenu (gitem, project);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-new", TRUE);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-open", TRUE);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-save", TRUE);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-save-as", TRUE);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, NULL, FALSE);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-quit", TRUE);

	/* Edit */
	gitem = glade_gtk_menu_bar_append_new_item (gmenubar, project, _("_Edit"), FALSE);	
	gsubmenu = glade_gtk_menu_bar_append_new_submenu (gitem, project);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-cut", TRUE);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-copy", TRUE);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-paste", TRUE);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-delete", TRUE);
	
	/* View */
	gitem = glade_gtk_menu_bar_append_new_item (gmenubar, project, _("_View"), FALSE);	
	
	/* Help */
	gitem = glade_gtk_menu_bar_append_new_item (gmenubar, project, _("_Help"), FALSE);	
	gsubmenu = glade_gtk_menu_bar_append_new_submenu (gitem, project);
	glade_gtk_menu_bar_append_new_item (gsubmenu, project, "gtk-about", TRUE);
}

/* ----------------------------- GtkToolBar ------------------------------ */

/* need to unset/reset toolbar style when property is disabled/enabled */
static void
property_toolbar_style_notify_enabled (GladeProperty *property,
				       GParamSpec    *spec,
				       GtkToolbar    *toolbar)
{
	GtkToolbarStyle style;

	if (glade_property_get_enabled (property))
	{
		glade_property_get (property, &style);
		gtk_toolbar_set_style (toolbar, style);
	}
	else
		gtk_toolbar_unset_style (toolbar);
} 

void
glade_gtk_toolbar_post_create (GladeWidgetAdaptor *adaptor,
			       GObject            *object, 
			       GladeCreateReason   reason)
{
	GladeWidget   *widget;
	GladeProperty *toolbar_style_property;	
	
	widget = glade_widget_get_from_gobject (object);
	toolbar_style_property = glade_widget_get_property (widget, "toolbar-style");
	
	g_signal_connect (toolbar_style_property, "notify::enabled",
			  G_CALLBACK (property_toolbar_style_notify_enabled),
			  object);
}

void
glade_gtk_toolbar_get_child_property (GladeWidgetAdaptor *adaptor,
				      GObject            *container,
				      GObject            *child,
				      const gchar        *property_name,
				      GValue             *value)
{
	g_return_if_fail (GTK_IS_TOOLBAR (container));
	if (GTK_IS_TOOL_ITEM (child) == FALSE) return;
	
	if (strcmp (property_name, "position") == 0)
	{
		g_value_set_int (value,
			 gtk_toolbar_get_item_index (GTK_TOOLBAR (container), 
						     GTK_TOOL_ITEM (child)));
	}
	else
	{	/* Chain Up */
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_get_property (adaptor, 
								  container, child,
								  property_name, value);
	}
}

void
glade_gtk_toolbar_set_child_property (GladeWidgetAdaptor *adaptor,
				      GObject            *container,
				      GObject            *child,
				      const gchar        *property_name,
				      GValue             *value)
{
	g_return_if_fail (GTK_IS_TOOLBAR (container));
	g_return_if_fail (GTK_IS_TOOL_ITEM (child));

	g_return_if_fail (property_name != NULL || value != NULL);
	
	if (strcmp (property_name, "position") == 0)
	{
		GtkToolbar *toolbar = GTK_TOOLBAR (container);
		gint position, size;
		
		position = g_value_get_int (value);
		size = gtk_toolbar_get_n_items (toolbar);
		
		if (position >= size) position = size - 1;
		
		g_object_ref (child);
		gtk_container_remove (GTK_CONTAINER (container), GTK_WIDGET (child));
		gtk_toolbar_insert (toolbar, GTK_TOOL_ITEM (child), position);
		g_object_unref (child);
	}
	else
		/* Chain Up */
		GWA_GET_CLASS
			(GTK_TYPE_CONTAINER)->child_set_property (adaptor, 
								  container, child,
								  property_name, value);
}

void
glade_gtk_toolbar_add_child (GladeWidgetAdaptor *adaptor,
			     GObject *object, GObject *child)
{
	GtkToolbar *toolbar;
	GtkToolItem *item;
	
	g_return_if_fail (GTK_IS_TOOLBAR (object));
	g_return_if_fail (GTK_IS_TOOL_ITEM (child));
	
	toolbar = GTK_TOOLBAR (object);
	item = GTK_TOOL_ITEM (child);
	
	gtk_toolbar_insert (toolbar, item, -1);
		
	if (glade_util_object_is_loading (object))
	{
		GladeWidget *gchild = glade_widget_get_from_gobject (child);
		
		/* Packing props arent around when parenting during a glade_widget_dup() */
		if (gchild && gchild->packing_properties)
			glade_widget_pack_property_set (gchild, "position", 
				gtk_toolbar_get_item_index (toolbar, item));
	}
}

void
glade_gtk_toolbar_remove_child (GladeWidgetAdaptor *adaptor, 
				GObject *object, GObject *child)
{
	gtk_container_remove (GTK_CONTAINER (object), GTK_WIDGET (child));
}

static gchar *
glade_gtk_toolbar_get_display_name (GladeBaseEditor *editor,
				    GladeWidget *gchild,
				    gpointer user_data)
{
	GObject *child = glade_widget_get_object (gchild);
	gchar *name;
	
	if (GTK_IS_SEPARATOR_TOOL_ITEM (child))
		name = _("<separator>");
	else
	if (GTK_IS_TOOL_BUTTON (child))
	{
		glade_widget_property_get (gchild, "label", &name);
		if (name == NULL || strlen (name) == 0)
			glade_widget_property_get (gchild, "stock-id", &name);
	}
	else
		name = _("<custom>");
	
	return g_strdup (name);
}

static void
glade_gtk_toolbar_child_selected (GladeBaseEditor *editor,
				  GladeWidget *gchild,
				  gpointer data)
{
	GObject *child = glade_widget_get_object (gchild);
	GType type = G_OBJECT_TYPE (child);
	
	glade_base_editor_add_label (editor, "Tool Item");
	
	glade_base_editor_add_default_properties (editor, gchild);
	
	glade_base_editor_add_label (editor, "Properties");
	
	glade_base_editor_add_properties (editor, gchild, FALSE,
					  "visible-horizontal",
					  "visible-vertical",
					  NULL);
	
	if (type == GTK_TYPE_SEPARATOR_TOOL_ITEM) return;

	if (GTK_IS_TOOL_BUTTON (child))
		glade_base_editor_add_properties (editor, gchild, FALSE,
						  "label", 
						  "glade-type",
						  "icon",
						  "glade-stock",
						  "icon-name",
						  NULL);
	
	if (type == GTK_TYPE_RADIO_TOOL_BUTTON)
		glade_base_editor_add_properties (editor, gchild, FALSE,
						  "group", "active", NULL);	

	glade_base_editor_add_label (editor, "Packing");
	glade_base_editor_add_properties (editor, gchild, TRUE,
					  "expand", "homogeneous", NULL);

}

static void
glade_gtk_toolbar_launch_editor (GladeWidgetAdaptor *adaptor, 
				 GObject            *toolbar)
{
	GladeBaseEditor *editor;
	GtkWidget *window;
	/* Editor */
	editor = glade_base_editor_new (toolbar, FALSE,
					_("Button"), GTK_TYPE_TOOL_BUTTON,
					_("Toggle"), GTK_TYPE_TOGGLE_TOOL_BUTTON,
					_("Radio"), GTK_TYPE_RADIO_TOOL_BUTTON,
					_("Menu"), GTK_TYPE_MENU_TOOL_BUTTON,
					_("Item"), GTK_TYPE_TOOL_ITEM,
					_("Separator"), GTK_TYPE_SEPARATOR_TOOL_ITEM,
					NULL);

	glade_base_editor_add_popup_items (editor,
					  _("Add Tool Button"), GTK_TYPE_TOOL_BUTTON, FALSE, 
					  _("Add Toggle Button"), GTK_TYPE_TOGGLE_TOOL_BUTTON, FALSE, 
					  _("Add Radio Button"), GTK_TYPE_RADIO_TOOL_BUTTON, FALSE, 
					  _("Add Menu Button"), GTK_TYPE_MENU_TOOL_BUTTON, FALSE, 
					  _("Add Tool Item"), GTK_TYPE_TOOL_ITEM, FALSE, 
					  _("Add Separator"), GTK_TYPE_SEPARATOR_TOOL_ITEM, FALSE,
					  NULL);
	
	g_signal_connect (editor, "get-display-name", G_CALLBACK (glade_gtk_toolbar_get_display_name), NULL);
	g_signal_connect (editor, "child-selected", G_CALLBACK (glade_gtk_toolbar_child_selected), NULL);

	gtk_widget_show (GTK_WIDGET (editor));
	
	window = glade_base_editor_pack_new_window (editor, _("Tool Bar Editor"), NULL);
	gtk_widget_show (window);
}

void
glade_gtk_toolbar_action_activate (GladeWidgetAdaptor *adaptor,
				   GObject *object,
				   const gchar *action_path)
{
	if (strcmp (action_path, "launch_editor") == 0)
	{
		glade_gtk_toolbar_launch_editor (adaptor, object);
	}
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->action_activate (adaptor,
								     object,
								     action_path);
}

/* ----------------------------- GtkToolItem ------------------------------ */
GObject *
glade_gtk_tool_item_constructor (GType                  type,
				 guint                  n_construct_properties,
				 GObjectConstructParam *construct_properties)
{
	GladeWidgetAdaptor   *adaptor;
	GObject              *ret_obj;
	
	ret_obj = GWA_GET_OCLASS(GTK_TYPE_CONTAINER)->constructor
		(type, n_construct_properties, construct_properties);

	adaptor = GLADE_WIDGET_ADAPTOR (ret_obj);

	glade_widget_adaptor_action_remove (adaptor, "add_parent");
	glade_widget_adaptor_action_remove (adaptor, "remove_parent");

	return ret_obj;
}

void
glade_gtk_tool_item_post_create (GladeWidgetAdaptor *adaptor, 
				 GObject            *object, 
				 GladeCreateReason   reason)
{
	g_return_if_fail (GTK_IS_TOOL_ITEM (object));
	
	if (GTK_IS_SEPARATOR_TOOL_ITEM (object)) return;
	
	if (reason == GLADE_CREATE_USER &&
	    gtk_bin_get_child (GTK_BIN (object)) == NULL)
		gtk_container_add (GTK_CONTAINER (object),
				   glade_placeholder_new ());
}

/* ----------------------------- GtkToolButton ------------------------------ */
static void
glade_gtk_tool_button_set_type (GObject *object, const GValue *value)
{
	GladeWidget *gbutton;
	
	g_return_if_fail (GTK_IS_TOOL_BUTTON (object));
	gbutton = glade_widget_get_from_gobject (object);
	
	if (glade_util_object_is_loading (object)) return;
	
	glade_widget_property_set_sensitive (gbutton, "icon", FALSE,
				_("This only applies with file type images"));
	glade_widget_property_set_sensitive (gbutton, "glade-stock", FALSE,
				_("This only applies with stock type images"));
	glade_widget_property_set_sensitive (gbutton, "icon-name", FALSE,
				_("This only applies to Icon Theme type images"));
	
	switch (g_value_get_enum (value))
	{
		case GLADEGTK_IMAGE_FILENAME:
			glade_widget_property_set_sensitive (gbutton, "icon",
							     TRUE, NULL);
			glade_widget_property_set (gbutton, "glade-stock", NULL);
			glade_widget_property_set (gbutton, "icon-name", NULL);
		break;
		case GLADEGTK_IMAGE_STOCK:
			glade_widget_property_set_sensitive (gbutton, "glade-stock",
							     TRUE, NULL);
			glade_widget_property_set (gbutton, "icon", NULL);
			glade_widget_property_set (gbutton, "icon-name", NULL);
		break;
		case GLADEGTK_IMAGE_ICONTHEME:
			glade_widget_property_set_sensitive (gbutton, "icon-name",
							     TRUE, NULL);
			glade_widget_property_set (gbutton, "icon", NULL);
			glade_widget_property_set (gbutton, "glade-stock", NULL);
		break;
	}
}

static void
glade_gtk_tool_button_set_label (GObject *object, const GValue *value)
{
	const gchar *label;
	
	g_return_if_fail (GTK_IS_TOOL_BUTTON (object));
	
	label = g_value_get_string (value);
	
	if (label && strlen (label) == 0) label = NULL;
	
	gtk_tool_button_set_label (GTK_TOOL_BUTTON (object), label);
}

static void
glade_gtk_tool_button_set_stock_id (GObject *object, const GValue *value)
{
	GladeWidget *gbutton;
	GEnumClass *eclass;
	GEnumValue *eval;
	const gchar *stock_id;
	
	g_return_if_fail (GTK_IS_TOOL_BUTTON (object));
	gbutton = glade_widget_get_from_gobject (object);
		
	if ((stock_id = g_value_get_string (value)))
	{
		eclass = g_type_class_ref (GLADE_TYPE_STOCK_IMAGE);

		if ((eval = g_enum_get_value_by_nick (eclass, stock_id)) != NULL)
			glade_widget_property_set (gbutton, "glade-stock", eval->value);
		else
		{
			glade_widget_property_set (gbutton, "glade-stock", 
						   "gtk-missing-image");
			g_warning ("Invalid stock-id '%s' found in toolbutton", stock_id);
		}

		glade_widget_property_set (gbutton, "glade-type", GLADEGTK_IMAGE_STOCK);
		
		g_type_class_unref (eclass);
	}
	
	if (stock_id && strlen (stock_id) == 0) stock_id = NULL;
	
	gtk_tool_button_set_stock_id (GTK_TOOL_BUTTON (object), stock_id);
}

static void
glade_gtk_tool_button_set_glade_stock (GObject *object, const GValue *value)
{
	GladeWidget *gbutton;
	GEnumClass *eclass;
	GEnumValue *eval;
	gint val;
	
	g_return_if_fail (GTK_IS_TOOL_BUTTON (object));	
	gbutton = glade_widget_get_from_gobject (object);
	
	val = g_value_get_enum (value);

	if (val)
	{
		eclass = g_type_class_ref (GLADE_TYPE_STOCK_IMAGE);

		if ((eval = g_enum_get_value (eclass, val)) != NULL)
			glade_widget_property_set (gbutton, "stock-id", eval->value_nick);
		else
		{
			glade_widget_property_set (gbutton, "stock-id", "gtk-missing-image");
			g_warning ("Invalid glade stock id '%d' found in toolbutton", val);
		}		
		g_type_class_unref (eclass);
	}
	else
		glade_widget_property_set (gbutton, "stock-id", NULL);
}

static void
glade_gtk_tool_button_set_icon (GObject *object, const GValue *value)
{
	GladeWidget *gbutton;
	GObject *pixbuf;
	GtkWidget *image = NULL;
	
	g_return_if_fail (GTK_IS_TOOL_BUTTON (object));
	gbutton = glade_widget_get_from_gobject (object);
	
	if ((pixbuf = g_value_get_object (value)))
	{
		image = gtk_image_new_from_pixbuf (GDK_PIXBUF (pixbuf));
		gtk_widget_show (image);
		glade_widget_property_set (gbutton, "glade-type", GLADEGTK_IMAGE_FILENAME);
	}
	
	gtk_tool_button_set_icon_widget (GTK_TOOL_BUTTON (object), image);
}

static void
glade_gtk_tool_button_set_icon_name (GObject *object, const GValue *value)
{
	GladeWidget *gbutton;
	const gchar *name;
	
	g_return_if_fail (GTK_IS_TOOL_BUTTON (object));

	if ((name = g_value_get_string (value)))
	{
		gbutton = glade_widget_get_from_gobject (object);
		glade_widget_property_set (gbutton, "glade-type", GLADEGTK_IMAGE_ICONTHEME);
	}
	
	if (name && strlen (name) == 0) name = NULL;
		
	gtk_tool_button_set_icon_name (GTK_TOOL_BUTTON (object), name);
}

void
glade_gtk_tool_button_set_property (GladeWidgetAdaptor *adaptor,
				    GObject            *object, 
				    const gchar        *id,
				    const GValue       *value)
{
	if (!strcmp (id, "glade-type"))
		glade_gtk_tool_button_set_type (object, value);
	else if (!strcmp (id, "glade-stock"))
		glade_gtk_tool_button_set_glade_stock (object, value);
	else if (!strcmp (id, "icon-name"))
		glade_gtk_tool_button_set_icon_name (object, value);
	else if (!strcmp (id, "icon"))
		glade_gtk_tool_button_set_icon (object, value);
	else if (!strcmp (id, "stock-id"))
		glade_gtk_tool_button_set_stock_id (object, value);
	else if (!strcmp (id, "label"))
		glade_gtk_tool_button_set_label (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_TOOL_ITEM)->set_property (adaptor,
								  object,
								  id, value);
}

/* ----------------------------- GtkLabel ------------------------------ */
static void
glade_gtk_label_set_label (GObject *object, const GValue *value)
{
	GladeWidget *glabel;
	gboolean use_markup = FALSE, use_underline = FALSE;

	g_return_if_fail (GTK_IS_LABEL (object));
	glabel = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (glabel));
	
	glade_widget_property_get (glabel, "use-markup", &use_markup);
	
	if (use_markup)
		gtk_label_set_markup (GTK_LABEL (object), g_value_get_string (value));
	else
		gtk_label_set_text (GTK_LABEL (object), g_value_get_string (value));
	
	glade_widget_property_get (glabel, "use-underline", &use_underline);
	if (use_underline)
		gtk_label_set_use_underline (GTK_LABEL (object), use_underline);
}

static void
ensure_label_props (GObject            *label,
		    GladeWidgetAdaptor *adaptor)
{
	GladeWidget   *gwidget = glade_widget_get_from_gobject (label);
	GladeProperty *prop    = glade_widget_get_property (gwidget, "label");

	glade_gtk_label_set_label (label, prop->value);
}

void
glade_gtk_label_post_create (GladeWidgetAdaptor *adaptor, 
			     GObject            *object, 
			     GladeCreateReason   reason)
{
	/* For some reason labels dont show up with markup
	 * and mnemonic underlines in the runtime at load time,
	 * resetting them at realize time fixes this glitch.
	 */
	g_signal_connect_after (G_OBJECT (object), "realize",
				G_CALLBACK (ensure_label_props), adaptor);
}

void
glade_gtk_label_set_property (GladeWidgetAdaptor *adaptor,
			      GObject            *object, 
			      const gchar        *id,
			      const GValue       *value)
{
	if (!strcmp (id, "label"))
		glade_gtk_label_set_label (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_WIDGET)->set_property (adaptor,
							       object,
							       id, value);
}

/* ----------------------------- GtkTextView ------------------------------ */
static void
glade_gtk_text_view_changed (GtkTextBuffer *buffer, GladeWidget *gtext)
{
	const gchar *text_prop;
	GladeProperty *prop;
	gchar *text;
	
	g_object_get (buffer, "text", &text, NULL);
	
	glade_widget_property_get (gtext, "text", &text_prop);
	
	if (strcmp (text, text_prop))
		if ((prop = glade_widget_get_property (gtext, "text")))
			glade_command_set_property (prop, text);
	
	g_free (text);
}

static gboolean
glade_gtk_text_view_stop_double_click (GtkWidget *widget,
				       GdkEventButton *event,
				       gpointer user_data)
{
	/* Return True if the event is double or triple click */
	return (event->type == GDK_2BUTTON_PRESS ||
		event->type == GDK_3BUTTON_PRESS);
}

void
glade_gtk_text_view_post_create (GladeWidgetAdaptor *adaptor,
				 GObject            *object, 
				 GladeCreateReason   reason)
{
	GtkTextBuffer *buffy = gtk_text_buffer_new (NULL);
	GladeWidget *gtext;
	
	g_return_if_fail (GTK_IS_TEXT_VIEW (object));
	gtext = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gtext));
	
	/* This makes gtk_text_view_set_buffer() stop complaing */
	gtk_drag_dest_set (GTK_WIDGET (object), 0, NULL, 0, 0);
		
	gtk_text_view_set_buffer (GTK_TEXT_VIEW (object), buffy);
	g_signal_connect (buffy, "changed",
			  G_CALLBACK (glade_gtk_text_view_changed),
			  gtext);
	
	g_object_unref (G_OBJECT (buffy));

	/* Glade3 hangs when a TextView gets a double click. So we stop them */
	g_signal_connect (object, "button-press-event",
			  G_CALLBACK (glade_gtk_text_view_stop_double_click),
			  NULL);
}

static void
glade_gtk_text_view_set_text (GObject *object, const GValue *value)
{
	GtkTextBuffer *buffy;
	GladeWidget *gtext;
	const gchar *text;

	g_return_if_fail (GTK_IS_TEXT_VIEW (object));
	gtext = glade_widget_get_from_gobject (object);
	g_return_if_fail (GLADE_IS_WIDGET (gtext));
	
	buffy = gtk_text_view_get_buffer (GTK_TEXT_VIEW (object));
	
	if ((text = g_value_get_string (value)) == NULL) return;

	g_signal_handlers_block_by_func (buffy, glade_gtk_text_view_changed, gtext);
	gtk_text_buffer_set_text (buffy, text, -1);
	g_signal_handlers_unblock_by_func (buffy, glade_gtk_text_view_changed, gtext);
}

void
glade_gtk_text_view_set_property (GladeWidgetAdaptor *adaptor,
				  GObject            *object, 
				  const gchar        *id,
				  const GValue       *value)
{
	if (!strcmp (id, "text"))
		glade_gtk_text_view_set_text (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor,
								  object,
								  id, value);
}


/* ----------------------------- GtkComboBox ------------------------------ */
void
glade_gtk_combo_box_post_create (GladeWidgetAdaptor *adaptor,
				 GObject            *object, 
				 GladeCreateReason   reason)
{
	GtkCellRenderer *cell;
	GtkListStore *store;

	g_return_if_fail (GTK_IS_COMBO_BOX (object));

	/* Add store */
	store = gtk_list_store_new (1, G_TYPE_STRING);
	gtk_combo_box_set_model (GTK_COMBO_BOX (object), GTK_TREE_MODEL (store));
	g_object_unref (store);
	
	/* Add cell renderer */
	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (object), cell, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (object), cell,
					"text", 0, NULL);
}

static void
glade_gtk_combo_box_set_items (GObject *object, const GValue *value)
{
	GtkComboBox *combo;
	gchar **split;
	gint    i;

	g_return_if_fail (GTK_IS_COMBO_BOX (object));

	combo = GTK_COMBO_BOX (object);

	/* Empty the combo box */
	gtk_list_store_clear (GTK_LIST_STORE (gtk_combo_box_get_model (combo)));

	/* Refill the combo box */
	split = g_value_get_boxed (value);

	if (split)
		for (i = 0; split[i] != NULL; i++)
			if (split[i][0] != '\0')
				gtk_combo_box_append_text (combo, split[i]);
}

void
glade_gtk_combo_box_set_property (GladeWidgetAdaptor *adaptor,
				  GObject            *object, 
				  const gchar        *id,
				  const GValue       *value)
{
	if (!strcmp (id, "items"))
		glade_gtk_combo_box_set_items (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor,
								  object,
								  id, value);
}

/* ----------------------------- GtkComboBoxEntry ------------------------------ */
void
glade_gtk_combo_box_entry_post_create (GladeWidgetAdaptor *adaptor,
				       GObject            *object, 
				       GladeCreateReason   reason)
{
	GladeWidget *gcombo = glade_widget_get_from_gobject (object);

	/* Chain up */
	GWA_GET_CLASS (GTK_TYPE_CONTAINER)->post_create (adaptor, object, reason);

	glade_widget_adaptor_create_internal
		(gcombo, G_OBJECT (GTK_BIN (object)->child),
		 "entry", "comboboxentry", FALSE, reason);
}

GObject *
glade_gtk_combo_box_entry_get_internal_child (GladeWidgetAdaptor *adaptor,
					      GObject *object, 
					      const gchar *name)
{
	GObject *child = NULL;
	g_return_val_if_fail (GTK_IS_COMBO_BOX_ENTRY (object), NULL);
	
	if (strcmp ("entry", name) == 0)
		child = G_OBJECT (gtk_bin_get_child (GTK_BIN (object)));

	return child;
}

/* ----------------------------- GtkSpinButton ------------------------------ */
static void
glade_gtk_spin_button_set_adjustment (GObject *object, const GValue *value)
{
	GObject *adjustment;
	GtkAdjustment *adj;
	
	g_return_if_fail (GTK_IS_SPIN_BUTTON (object));
		
	adjustment = g_value_get_object (value);
	
	if (adjustment && GTK_IS_ADJUSTMENT (adjustment))
	{
		adj = GTK_ADJUSTMENT (adjustment);
		gtk_spin_button_set_adjustment (GTK_SPIN_BUTTON (object), adj);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (object), adj->value);
	}
}

void
glade_gtk_spin_button_set_property (GladeWidgetAdaptor *adaptor,
				    GObject            *object, 
				    const gchar        *id,
				    const GValue       *value)
{
	if (!strcmp (id, "adjustment"))
		glade_gtk_spin_button_set_adjustment (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_ENTRY)->set_property (adaptor,
							      object,
							      id, value);
}

/* ----------------------------- GtkTreeView ------------------------------ */
void
glade_gtk_tree_view_post_create (GladeWidgetAdaptor *adaptor,
				 GObject            *object, 
				 GladeCreateReason   reason)
{
	GtkWidget *tree_view = GTK_WIDGET (object);
	GtkTreeStore *store;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_STRING);

	gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (store));
	g_object_unref (G_OBJECT (store));

	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes
		("Column 1", renderer, "text", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);

	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes
		("Column 2", renderer, "text", 1, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);
}


/* ----------------------------- GtkCombo ------------------------------ */
void
glade_gtk_combo_post_create (GladeWidgetAdaptor *adaptor,
			     GObject            *object,
			     GladeCreateReason   reason)
{
	GladeWidget  *gcombo, *gentry, *glist;

	g_return_if_fail (GTK_IS_COMBO (object));

	if ((gcombo = glade_widget_get_from_gobject (object)) == NULL)
		return;
	
	gentry = glade_widget_adaptor_create_internal
		(gcombo, G_OBJECT (GTK_COMBO (object)->entry),
		 "entry", "combo", FALSE, reason);

	/* We mark this 'anarchist' since its outside of the hierarchy */
	glist  = glade_widget_adaptor_create_internal
		(gcombo, G_OBJECT (GTK_COMBO (object)->list),
		 "list", "combo", TRUE, reason);

}

GObject *
glade_gtk_combo_get_internal_child (GladeWidgetAdaptor *adaptor,
				    GtkCombo           *combo,
				    const gchar        *name)
{
	GObject *child = NULL;

	g_return_val_if_fail (GTK_IS_COMBO (combo), NULL);
	
	if (strcmp ("list", name) == 0)
		child = G_OBJECT (combo->list);
	else if (strcmp ("entry", name) == 0)
		child = G_OBJECT (combo->entry);

	return child;
}

GList *
glade_gtk_combo_get_children (GtkCombo *combo)
{
	GList *list = NULL;

	g_return_val_if_fail (GTK_IS_COMBO (combo), NULL);

	list = glade_util_container_get_all_children (GTK_CONTAINER (combo));

	/* Ensure that we only return one 'combo->list' */
	if (g_list_find (list, combo->list) == NULL)
		list = g_list_append (list, combo->list);

	return list;
}

/* ----------------------------- GtkListItem ------------------------------ */
void
glade_gtk_list_item_post_create (GladeWidgetAdaptor *adaptor,
				 GObject            *object, 
				 GladeCreateReason   reason)
{
	GtkWidget *label;

	g_return_if_fail (GTK_IS_LIST_ITEM (object));

	label = gtk_label_new ("");
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
	gtk_misc_set_padding (GTK_MISC (label), 0, 1);

	gtk_container_add (GTK_CONTAINER (object), label);
	gtk_widget_show (label);
}

static void
glade_gtk_list_item_set_label (GObject *object, const GValue *value)
{
	GtkWidget *label;

	g_return_if_fail (GTK_IS_LIST_ITEM (object));

	label = GTK_BIN (object)->child;

	gtk_label_set_text (GTK_LABEL (label), g_value_get_string (value));
}

static void
glade_gtk_list_item_get_label (GObject *object, GValue *value)
{
	GtkWidget *label;

	g_return_if_fail (GTK_IS_LIST_ITEM (object));

	label = GTK_BIN (object)->child;

	g_value_set_string (value, gtk_label_get_text (GTK_LABEL (label)));
}

void
glade_gtk_list_item_set_property (GladeWidgetAdaptor *adaptor,
				  GObject            *object, 
				  const gchar        *id,
				  const GValue       *value)
{
	if (!strcmp (id, "label"))
		glade_gtk_list_item_set_label (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor,
								  object,
								  id, value);
}

void
glade_gtk_list_item_get_property (GladeWidgetAdaptor *adaptor,
				  GObject            *object, 
				  const gchar        *id,
				  GValue             *value)
{
	if (!strcmp (id, "label"))
		glade_gtk_list_item_get_label (object, value);
	else
		GWA_GET_CLASS (GTK_TYPE_CONTAINER)->set_property (adaptor,
								  object,
								  id, value);
}


void
glade_gtk_listitem_add_child (GladeWidgetAdaptor  *adaptor,
			      GObject             *object, 
			      GObject             *child)
{
	g_return_if_fail (GTK_IS_CONTAINER (object));
	g_return_if_fail (GTK_IS_WIDGET (child));

	gtk_container_add (GTK_CONTAINER (object), GTK_WIDGET (child));
}

void
glade_gtk_listitem_remove_child (GladeWidgetAdaptor  *adaptor,
				 GObject             *object, 
				 GObject             *child)
{
	g_return_if_fail (GTK_IS_CONTAINER (object));
	g_return_if_fail (GTK_IS_WIDGET (child));

	gtk_container_add (GTK_CONTAINER (object), GTK_WIDGET (child));
}

/* ------------------------------ GtkAssistant ------------------------------ */
static void
glade_gtk_assistant_append_new_page (GladeWidget *parent,
				     GladeProject *project,
				     const gchar *label,
				     GtkAssistantPageType type)
{
	static GladeWidgetAdaptor *adaptor = NULL;
	GladeWidget *page;
	
	if (adaptor == NULL)
		adaptor = glade_widget_adaptor_get_by_type (GTK_TYPE_LABEL);

	page = glade_widget_adaptor_create_widget (adaptor, FALSE,
						   "parent", parent,
						   "project", project, 
						   NULL);

	glade_widget_add_child (parent, page, FALSE);
		
	glade_widget_property_set (page, "label", label);
	glade_widget_pack_property_set (page, "page-type", type);
}
/*
  GtkAssistant is a very weird widget, why is it derived from GtkWindow
  instead of GtkNotebook I do not know!

  If there is no GTK_ASSISTANT_PAGE_CONFIRM, GtkAssistant abort when trying to 
  update its navigation buttons!
*/
static void
glade_gtk_assistant_update_page_type (GtkAssistant *assistant)
{
	gint i, current, pages;
	GtkWidget *page;
	
	current = gtk_assistant_get_current_page (assistant);
	pages = gtk_assistant_get_n_pages (assistant) - 1;
	if (pages < 0) return;

	/* Last Page */
	page = gtk_assistant_get_nth_page (assistant, pages);
	gtk_assistant_set_page_type (assistant, page, GTK_ASSISTANT_PAGE_CONFIRM);
	
	/* First page */
	page = gtk_assistant_get_nth_page (assistant, 0);
	gtk_assistant_set_page_type (assistant, page, GTK_ASSISTANT_PAGE_INTRO);
	
	/* In betwen pages */	
	for (i = 1; i < pages; i++)
	{
		page = gtk_assistant_get_nth_page (assistant, i);
		gtk_assistant_set_page_type (assistant, page, GTK_ASSISTANT_PAGE_CONTENT);

	}
	
	/* Now we have set page-type in every page, force button update */
	for (i = 0; i <= pages; i++)
	{
		page = gtk_assistant_get_nth_page (assistant, i);
		gtk_assistant_set_page_complete (assistant, page, TRUE);
		gtk_assistant_set_current_page (assistant, i);
		gtk_assistant_update_buttons_state (assistant);
	}
	
	if (current >= 0) gtk_assistant_set_current_page (assistant, current);
}

static gint
glade_gtk_assistant_get_page (GtkAssistant *assistant, GtkWidget *page)
{
	gint i, pages = gtk_assistant_get_n_pages (assistant);
		
	for (i = 0; i < pages; i++)
		if (gtk_assistant_get_nth_page (assistant, i) == page)
			return i;
	
	return -1;
}

static void
glade_gtk_assistant_update_position (GtkAssistant *assistant)
{
	gint i, pages = gtk_assistant_get_n_pages (assistant);
		
	for (i = 0; i < pages; i++)
	{
		GtkWidget *page =  gtk_assistant_get_nth_page (assistant, i);
		GladeWidget *gpage = glade_widget_get_from_gobject (G_OBJECT (page));
		if (gpage) glade_widget_pack_property_set (gpage, "position", i);
	}
}

static void
glade_gtk_assistant_parse_finished (GladeProject *project,
				    GObject *object)
{
	GtkAssistant *assistant = GTK_ASSISTANT (object);
	gint pages = gtk_assistant_get_n_pages (assistant);
	
	if (pages)
	{
		gtk_assistant_set_current_page (assistant, 0);
		glade_widget_property_set (glade_widget_get_from_gobject (object),
					   "size", pages);
	}
}

void
glade_gtk_assistant_post_create (GladeWidgetAdaptor *adaptor,
				 GObject *object, 
				 GladeCreateReason reason)
{
	GladeWidget *parent = glade_widget_get_from_gobject (object);
	GladeProject *project = glade_widget_get_project (parent);
	
	if (reason == GLADE_CREATE_LOAD)
	{
		g_signal_connect (project, "parse-finished",
				  G_CALLBACK (glade_gtk_assistant_parse_finished),
				  object);
		return;
	}
	
	if (reason == GLADE_CREATE_USER)
	{
		glade_gtk_assistant_append_new_page (parent, project,
						     _("Introduction page"),
						     GTK_ASSISTANT_PAGE_INTRO);
	
		glade_gtk_assistant_append_new_page (parent, project,
						     _("Content page"),
						     GTK_ASSISTANT_PAGE_CONTENT);
	
		glade_gtk_assistant_append_new_page (parent, project,
						     _("Confirmation page"),
						     GTK_ASSISTANT_PAGE_CONFIRM);
		
		gtk_assistant_set_current_page (GTK_ASSISTANT (object), 0);
		
		glade_widget_property_set (parent, "size", 3);
	}
}

void
glade_gtk_assistant_add_child (GladeWidgetAdaptor *adaptor,
			       GObject *container,
			       GObject *child)
{
	GtkAssistant *assistant = GTK_ASSISTANT (container);
	GtkWidget *widget = GTK_WIDGET (child);
	
	gtk_assistant_append_page (assistant, widget);
}

void
glade_gtk_assistant_remove_child (GladeWidgetAdaptor *adaptor,
				  GObject *container, 
				  GObject *child)
{
	GtkAssistant *assistant = GTK_ASSISTANT (container);
	GladeWidget *gassistant = glade_widget_get_from_gobject (container);
	
	gtk_container_remove (GTK_CONTAINER (container), GTK_WIDGET (child));
	glade_widget_property_set (gassistant, "size",
			   	   gtk_assistant_get_n_pages (assistant));
}

void
glade_gtk_assistant_replace_child (GladeWidgetAdaptor *adaptor,
				   GObject *container,
				   GObject *current,
				   GObject *new_object)
{
	GtkAssistant *assistant = GTK_ASSISTANT (container);
	GtkWidget *page = GTK_WIDGET (new_object), *old_page = GTK_WIDGET (current);
	gint pos = glade_gtk_assistant_get_page (assistant, old_page);
	gboolean set_current = gtk_assistant_get_current_page (assistant) == pos;
	
	gtk_container_remove (GTK_CONTAINER (container), old_page);
		
	gtk_assistant_insert_page (assistant, page, pos);
	glade_gtk_assistant_update_page_type (assistant);
	
	if (set_current) gtk_assistant_set_current_page (assistant, pos);
}

gboolean
glade_gtk_assistant_verify_property (GladeWidgetAdaptor *adaptor,
				     GObject *object, 
				     const gchar *property_name,
				     const GValue *value)
{
	if (strcmp (property_name, "size") == 0)
		return  g_value_get_int (value) >
			gtk_assistant_get_n_pages (GTK_ASSISTANT (object));
	
	/* Chain Up */
	if (GWA_GET_CLASS (GTK_TYPE_WINDOW)->verify_property == NULL)
		return TRUE;
	return GWA_GET_CLASS (GTK_TYPE_WINDOW)->verify_property (adaptor,
								 object, 
								 property_name, 
								 value);
}

void
glade_gtk_assistant_set_property (GladeWidgetAdaptor *adaptor,
				  GObject *object,
				  const gchar *property_name,
				  const GValue *value)
{
	if (strcmp (property_name, "size") == 0)
	{
		GtkAssistant *assistant = GTK_ASSISTANT (object);
		gint size, i;
		
		for (i = gtk_assistant_get_n_pages (GTK_ASSISTANT (object)),
		     size = g_value_get_int (value); i < size; i++)
			gtk_assistant_append_page (assistant, glade_placeholder_new ());

		glade_gtk_assistant_update_page_type (assistant);

		return;
	}
	
	/* Chain Up */
	GWA_GET_CLASS (GTK_TYPE_WINDOW)->set_property (adaptor,
						       object,
						       property_name, 
						       value);
}

void
glade_gtk_assistant_get_property (GladeWidgetAdaptor *adaptor,
				  GObject *object,
				  const gchar *property_name,
				  GValue *value)
{
	if (strcmp (property_name, "size") == 0)
	{
		g_value_set_int (value, 
			gtk_assistant_get_n_pages (GTK_ASSISTANT (object)));
		return;
	}
	
	/* Chain Up */
	GWA_GET_CLASS (GTK_TYPE_WINDOW)->get_property (adaptor,
						       object,
						       property_name, 
						       value);
}

void
glade_gtk_assistant_set_child_property (GladeWidgetAdaptor *adaptor,
					GObject            *container,
					GObject            *child,
					const gchar        *property_name,
					const GValue       *value)
{	
	if (strcmp (property_name, "position") == 0)
	{
		GtkAssistant *assistant = GTK_ASSISTANT (container);
		GtkWidget *widget = GTK_WIDGET (child);
		gint pos, size;
		gboolean set_current;
		
		if ((pos = g_value_get_int (value)) < 0) return;
		if (pos == glade_gtk_assistant_get_page (assistant, widget))
			return;
		set_current = gtk_assistant_get_current_page (assistant) == 
			      glade_gtk_assistant_get_page (assistant, widget);
		
		size = gtk_assistant_get_n_pages (assistant);

		g_object_ref (child);
		gtk_container_remove (GTK_CONTAINER (container), widget);
		gtk_assistant_insert_page (assistant, widget, pos);
		g_object_unref (child);
		
		if (set_current) gtk_assistant_set_current_page (assistant, pos);
			
		glade_gtk_assistant_update_page_type (assistant);

		glade_gtk_assistant_update_position (assistant);
		
		return;
	}
	
	/* Chain Up */
	GWA_GET_CLASS (GTK_TYPE_WINDOW)->child_set_property (adaptor, 
							     container, 
							     child,
							     property_name, 
							     value);
}

void
glade_gtk_assistant_get_child_property (GladeWidgetAdaptor *adaptor,
					GObject            *container,
					GObject            *child,
					const gchar        *property_name,
					GValue       *value)
{
	if (strcmp (property_name, "position") == 0)
	{
		gint pos;
		pos = glade_gtk_assistant_get_page (GTK_ASSISTANT (container),
						    GTK_WIDGET (child));
		if (pos >= 0) g_value_set_int (value, pos);
		return;
	}
	
	/* Chain Up */
	GWA_GET_CLASS (GTK_TYPE_WINDOW)->child_get_property (adaptor, 
							     container, 
							     child,
							     property_name, 
							     value);
}

/*--------------------------- GtkRadioButton ---------------------------------*/
void
glade_gtk_radio_button_set_property (GladeWidgetAdaptor *adaptor,
				     GObject *object,
				     const gchar *property_name,
				     const GValue *value)
{
	if (strcmp (property_name, "group") == 0)
	{
		GtkRadioButton *radio = g_value_get_object (value);
		/* g_object_set () on this property produces a bogus warning,
		 * so we better use the API GtkRadioButton provides.
		 */
		gtk_radio_button_set_group (GTK_RADIO_BUTTON (object), 
					    radio ? gtk_radio_button_get_group (radio) : NULL);
		return;
	}
	
	/* Chain Up */
	GWA_GET_CLASS (GTK_TYPE_CHECK_BUTTON)->set_property (adaptor,
							     object,
							     property_name, 
							     value);
}
