/* include standard i/o files and gtk */
#include <stdio.h>
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

/* function declarations */
void end_program();
void file_new();
void file_open();
void file_save();
void file_save_as();

/* global gtk variables */
GtkWidget *win;
GtkSourceBuffer *buffer;
GtkSourceFile *source_file;

/* display window */
int main(int argc, char *argv[])
{
	/* initialize gtk */
	gtk_init(&argc, &argv);
	gtk_source_init();

	/* window */
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);
	gtk_window_set_title(GTK_WINDOW(win), "Cody");
	
	/* menu bar */
	GtkWidget *menu_bar = gtk_menu_bar_new();

	/* file menu */
	GtkWidget *file = gtk_menu_item_new_with_label("File");
	GtkWidget *file_menu = gtk_menu_new();
	GtkWidget *file_menu_new = gtk_menu_item_new_with_label("New File");
	GtkWidget *file_menu_open = gtk_menu_item_new_with_label("Open File");
	GtkWidget *file_menu_save = gtk_menu_item_new_with_label("Save");
	GtkWidget *file_menu_save_as = gtk_menu_item_new_with_label("Save As");
	GtkWidget *file_menu_quit = gtk_menu_item_new_with_label("Quit");

	/* add file menu to menu bar */
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_new);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_open);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_save);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_save_as);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_quit);
	

	/* code widget */
	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	buffer = gtk_source_buffer_new(NULL);
	GtkWidget *code = gtk_source_view_new_with_buffer(buffer);
	gtk_container_add(GTK_CONTAINER(scrolled_window), code);
	PangoFontDescription *font_desc = pango_font_description_from_string("Mono 10");
	gtk_widget_modify_font(code, font_desc);
	gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(code), TRUE);
	gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(code), TRUE);

	/* menu item signals */
	g_signal_connect(file_menu_new, "activate", G_CALLBACK(file_new), NULL);
	g_signal_connect(file_menu_open, "activate", G_CALLBACK(file_open), NULL);
	g_signal_connect(file_menu_save, "activate", G_CALLBACK(file_save), NULL);
	g_signal_connect(file_menu_save_as, "activate", G_CALLBACK(file_save_as), NULL);
	g_signal_connect(file_menu_quit, "activate", G_CALLBACK(end_program), NULL);
	g_signal_connect(win, "delete_event", G_CALLBACK(end_program), NULL);
	
	/* display widgets */
	GtkWidget *box = gtk_vbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(box), menu_bar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(win), box);
	gtk_widget_show_all(win);
	
	/* program loop */
	gtk_main();
	
	/* exit */
	gtk_source_finalize();
	return 0;
}

/* end program handler */
void end_program(GtkWidget *widget, gpointer data)
{
	/* end gtk main loop */
	gtk_main_quit();
}

/* new file handler */
void file_new(GtkWidget *widget, gpointer data)
{
	printf("New File!\n");
}

/* open file handler */
void file_open(GtkWidget *widget, gpointer data)
{
	/* open file dialog */
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Open file", GTK_WINDOW(win), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL); 
	
	/* open new file */
	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		source_file = gtk_source_file_new();
		char *chooser = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		GFile *path = g_file_new_for_path(chooser);
		gtk_source_file_set_location(source_file, path);
		GtkSourceFileLoader *loader = gtk_source_file_loader_new(buffer, source_file);
		gtk_source_file_loader_load_async(loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL, NULL, NULL);
		g_free(chooser);		
	}

	/* destroy dialog */
	gtk_widget_destroy(dialog);
}

/* save file handler */
void file_save(GtkWidget *widget, gpointer data)
{
	/* save file */
	GtkSourceFileSaver *saver = gtk_source_file_saver_new(buffer, source_file);
	gtk_source_file_saver_save_async(saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL, NULL, NULL);
	gtk_source_file_saver_save_finish(saver, NULL, NULL);
}

/* save file as handler */
void file_save_as(GtkWidget *widget, gpointer data)
{
	/* save file dialog */
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Save file", GTK_WINDOW(win), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE_AS, GTK_RESPONSE_ACCEPT, NULL);

	/* save file */
	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		char *chooser = gtk_file_chooser_get_current_name(GTK_FILE_CHOOSER(dialog));
		GFile *path = g_file_new_for_path(chooser);
		GtkSourceFileSaver *saver = gtk_source_file_saver_new_with_target(buffer, source_file, path);
		gtk_source_file_saver_save_async(saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL, NULL, NULL);
		gtk_source_file_saver_save_finish(saver, NULL, NULL);
		g_free(chooser);		
	}

	/* destroy dialog */
	gtk_widget_destroy(dialog);
}
