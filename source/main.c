/*

    Treebark Text Editor
    Copyright (C) 2021 ImperialOfficer324

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "settings.h"
#include "message.h"

GtkWidget *window, *grid, *save_button,*saveas_button, *newfile_button,*open_file_button;
GtkWidget *view;
GtkWidget *swindow;
GtkTextBuffer *buffer;

char * filename[255];
int untitled = 1;
int saved = 1;

int MAX_LINE_LENGTH = 1000;

int set_windowtitle_with_filename(const char * name,int saved){
    char * title[355];
    strcpy((char * restrict)title,(const char *)name);
    if(!saved) strcat((char * restrict)title,"*");
    strcat((char * restrict)title," - Treebark Editor");
    gtk_window_set_title(GTK_WINDOW(window),(const gchar *)title);
}

void configure_callback(GtkWindow *window, GdkEvent *event, gpointer data) {
          
    int w, h;

    gtk_window_get_size(window, &w, &h);

    gtk_widget_set_size_request(GTK_WIDGET(view), w, h-50);
    gtk_widget_set_size_request(GTK_WIDGET(swindow), w, h-50);
}

void modified(GtkTextBuffer *buffer, GdkEvent *event, gpointer data){
    set_windowtitle_with_filename((const char *)filename,0);
    saved = 0;
}

int save_to_file(const char * path){
    FILE * fp;
    GtkTextIter start,end;

    gtk_text_buffer_get_iter_at_offset (buffer, &start, 0);
    gtk_text_buffer_get_iter_at_offset (buffer, &end, -1);

    fp = fopen(path,"w");

    gchar * buffer_contents = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
    fprintf(fp,(const char *)buffer_contents);

    set_windowtitle_with_filename((const char *)filename,1);

    fclose(fp);
}

void save_file_as(GtkWindow *window, GdkEvent *event, gpointer data){
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Save File As", window,action,"_Cancel",GTK_RESPONSE_CANCEL,"_Save",GTK_RESPONSE_ACCEPT,NULL);
    chooser = GTK_FILE_CHOOSER (dialog);

    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

    gtk_file_chooser_set_current_name (chooser,(const char *)filename);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *selected_filename;

        selected_filename = gtk_file_chooser_get_filename (chooser);
        untitled = 0;
        strcpy((char *)filename,(const char *)selected_filename);
        set_windowtitle_with_filename((const char *)filename,1);
        save_to_file((const char *)filename);
        g_free(selected_filename);
    }

    gtk_widget_destroy (dialog);
}

void save_file(GtkWindow *window, GdkEvent *event, gpointer data){
    if(untitled){
        GtkWidget *dialog;
        GtkFileChooser *chooser;
        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
        gint res;

        dialog = gtk_file_chooser_dialog_new ("Save File", window,action,"_Cancel",GTK_RESPONSE_CANCEL,"_Save",GTK_RESPONSE_ACCEPT,NULL);
        chooser = GTK_FILE_CHOOSER (dialog);

        gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

        gtk_file_chooser_set_current_name (chooser,"Untitled.txt");

        res = gtk_dialog_run (GTK_DIALOG (dialog));
        if (res == GTK_RESPONSE_ACCEPT)
        {
            char *selected_filename;

            selected_filename = gtk_file_chooser_get_filename (chooser);
            untitled = 0;
            strcpy((char *)filename,(const char *)selected_filename);
            set_windowtitle_with_filename((const char *)filename,1);
            save_to_file((const char *)filename);
            g_free(selected_filename);
        }

        gtk_widget_destroy (dialog);
    }
    else{
        save_to_file((const char *)filename);
    }
}

void new_file(GtkWindow *widget, GdkEvent *event, gpointer data){
    if(!saved){
        if(confirmation(GTK_WINDOW(window),"You have unsaved changes to your file. Do you want to continue?")){
            strcpy((char *)filename,"Untitled.txt\0");
            gtk_text_buffer_set_text (buffer, "", -1);
            set_windowtitle_with_filename((const char *)filename,1);
        }
        else return;
    }
}

void do_open_file(const char * path){
    FILE * fp;
    char line[MAX_LINE_LENGTH];
    GtkTextIter end;

    gtk_text_buffer_get_iter_at_offset (buffer, &end, -1);

    strcpy((char *)filename,"\0");
    strcpy((char *)filename,(const char *)path);

    fp = fopen(path,"r");
    
    if(fp == NULL)
        return;

    gtk_text_buffer_set_text (buffer,"",-1);
     
    while(fgets(line, MAX_LINE_LENGTH, fp)){
        gtk_text_buffer_insert(buffer,&end,line,-1);
        gtk_text_buffer_get_iter_at_offset (buffer, &end, -1);
    }

    set_windowtitle_with_filename((const char *)filename,1);
    saved = 1;

    //gtk_text_buffer_set_text(buffer,(const gchar *)text,-1);

    fclose(fp);
}

void open_file(GtkWindow *window, GdkEvent *event, gpointer data){
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Open File",window,GTK_FILE_CHOOSER_ACTION_OPEN,"_Cancel",GTK_RESPONSE_CANCEL,"_Open",GTK_RESPONSE_ACCEPT,NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *open_filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        open_filename = gtk_file_chooser_get_filename (chooser);
        do_open_file((const char *)open_filename);
        g_free (open_filename);
    }

    gtk_widget_destroy (dialog);
}

int styleapp(){
    if(strcmp((const char *)THEME,"none")==0){
        return 0;
    };
    GtkCssProvider *cssProvider;
    GError *error = NULL;
    GtkStyleContext *context_editor, *context_newfile_button, *context_open_file_button, *context_save_button, *context_saveas_button, *window_context;

    /* new css provider */
    cssProvider = gtk_css_provider_new(); 

    /* widget name for css syntax */
    //gtk_widget_set_name(GTK_WIDGET(view), "editor");
    // gtk_widget_set_name(GTK_WIDGET(newfile_button), "toolbar-button");
    // gtk_widget_set_name(GTK_WIDGET(open_file_button), "toolbar-button");
    // gtk_widget_set_name(GTK_WIDGET(save_button), "toolbar-button");
    // gtk_widget_set_name(GTK_WIDGET(saveas_button), "toolbar-button");

    /* load css file */
    gtk_css_provider_load_from_path (cssProvider, (const gchar *)config_css_path, &error); 

    /* get GtkStyleContext from widget   */
    context_editor = gtk_widget_get_style_context(GTK_WIDGET(view));
    context_newfile_button = gtk_widget_get_style_context(GTK_WIDGET(newfile_button));
    context_open_file_button = gtk_widget_get_style_context(GTK_WIDGET(open_file_button));
    context_save_button = gtk_widget_get_style_context(GTK_WIDGET(save_button));
    context_saveas_button = gtk_widget_get_style_context(GTK_WIDGET(saveas_button));

    gtk_style_context_add_class(context_editor, "text-editor");
    gtk_style_context_add_class(context_newfile_button, "toolbar-button");
    gtk_style_context_add_class(context_open_file_button, "toolbar-button");
    gtk_style_context_add_class(context_save_button, "toolbar-button");
    gtk_style_context_add_class(context_saveas_button, "toolbar-button");

    //gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(swindow)),"scrolled-window");

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    return 1;
}

static void activate (GtkApplication *app, gpointer user_data){
    //Create Grid and Window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_add_events(GTK_WIDGET(window), GDK_CONFIGURE);
    gtk_window_set_title(GTK_WINDOW(window), "Treebark Editor");
    set_windowtitle_with_filename((const char *)filename,1);
    gtk_window_resize(GTK_WINDOW(window), 600, 600);
    //signals
    g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(configure_callback), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    g_signal_new("save-file",GTK_TYPE_TEXT_VIEW, G_SIGNAL_RUN_FIRST|G_SIGNAL_ACTION,0,NULL,NULL,NULL,G_TYPE_NONE,0);
    g_signal_new("new-file",GTK_TYPE_TEXT_VIEW, G_SIGNAL_RUN_FIRST|G_SIGNAL_ACTION,0,NULL,NULL,NULL,G_TYPE_NONE,0);
    g_signal_new("open-file",GTK_TYPE_TEXT_VIEW, G_SIGNAL_RUN_FIRST|G_SIGNAL_ACTION,0,NULL,NULL,NULL,G_TYPE_NONE,0);
    g_signal_new("save-file-as",GTK_TYPE_TEXT_VIEW, G_SIGNAL_RUN_FIRST|G_SIGNAL_ACTION,0,NULL,NULL,NULL,G_TYPE_NONE,0);

    //toolbar
    //new file button
    newfile_button = gtk_button_new_from_icon_name("document-new",GTK_ICON_SIZE_LARGE_TOOLBAR);
    g_signal_connect(G_OBJECT(newfile_button),"clicked",G_CALLBACK(new_file),NULL);
    gtk_grid_attach(GTK_GRID(grid),newfile_button,0,0,1,1);
    //open button
    open_file_button = gtk_button_new_from_icon_name("document-open",GTK_ICON_SIZE_LARGE_TOOLBAR);
    g_signal_connect(G_OBJECT(open_file_button),"clicked",G_CALLBACK(open_file),NULL);
    gtk_grid_attach(GTK_GRID(grid),open_file_button,1,0,1,1);
    //save button
    save_button = gtk_button_new_from_icon_name("document-save",GTK_ICON_SIZE_LARGE_TOOLBAR);
    g_signal_connect(G_OBJECT(save_button),"clicked",G_CALLBACK(save_file),NULL);
    gtk_grid_attach(GTK_GRID(grid),save_button,2,0,1,1);

    //save as button
    saveas_button = gtk_button_new_from_icon_name("document-save-as",GTK_ICON_SIZE_LARGE_TOOLBAR);
    g_signal_connect(G_OBJECT(saveas_button),"clicked",G_CALLBACK(save_file_as),NULL);
    gtk_grid_attach(GTK_GRID(grid),saveas_button,3,0,1,1);

    //Text Field

    view = gtk_text_view_new ();
    //g_signal_connect(G_OBJECT(window), "<Control>s", G_CALLBACK(save_file), NULL);

    gtk_widget_set_size_request(GTK_WIDGET(view), 600, 600);
    //gtk_widget_set_size_request(GTK_WIDGET(grid), 600, 600);

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
    g_signal_connect(buffer, "changed", G_CALLBACK(modified), NULL);
    g_signal_connect(G_OBJECT(view), "save-file", G_CALLBACK(save_file), NULL);
    g_signal_connect(G_OBJECT(view), "new-file", G_CALLBACK(new_file), NULL);
    g_signal_connect(G_OBJECT(view), "open-file", G_CALLBACK(open_file), NULL);
    g_signal_connect(G_OBJECT(view), "save-file-as", G_CALLBACK(save_file_as), NULL);
    gtk_text_buffer_set_text (buffer, "", -1);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);

    swindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(swindow), 600, 600);

    gtk_container_add(GTK_CONTAINER(swindow), view);

    gtk_grid_attach(GTK_GRID(grid), swindow, 0, 1, 10, 5);

    //g_signal_connect(GTK_TEXT_BUFFER(buffer), "changed", G_CALLBACK(checkEndLine), NULL);

    styleapp();
    //Showing widgets 
    gtk_widget_show_all(window);
    gtk_main();
}

int main(int argc, char **argv){

    printf("    Treebark Text Editor Copyright (C) 2021  ImperialOfficer324\n\
    This program comes with ABSOLUTELY NO WARRANTY'.\n\
    This is free software, and you are welcome to redistribute it\n\
    under certain conditions;\n");
    GtkApplication *app;
    int status;

    gtk_init(&argc,&argv);

    if(argc>1){
        strcpy((char *)filename,(const char *)argv[1]);
        untitled = 0;
    }
    else{
        strcpy((char *)filename,"Untitled.txt");
        untitled = 1;
    };

    load_settings();

    app = gtk_application_new("treebark.editor", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}