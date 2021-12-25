#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

GtkWidget *window, *grid, *label, *cancel_button, *next_button, *swindow, *view, *agreement_button, *radio1, *radio2, *box, *button_addtopath, *button_addtoapps, *button_addtodesktop, *path_entry, *install_button;
GtkTextBuffer *buffer;

int page, install_method;

void next_page();

void agreement_click(){
    if(gtk_widget_get_sensitive(GTK_WIDGET(next_button))==TRUE){
        gtk_widget_set_sensitive(GTK_WIDGET(next_button), FALSE);
    }
    else{
        gtk_widget_set_sensitive(GTK_WIDGET(next_button), TRUE);
    }
}

void radio_toggled(){
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio1))){
        install_method = 0;
    }
    else{
        install_method = 1;
    };
}

void install(){
    /*
    installation steps:
    copy "resources/config/*" to ~/.config/treebark/
    optional: copy "build/treebark" to path
    optional: copy "Treebark Text Editor" to  /usr/share/applications/
    optional: copy "Treebark Text Editor" to ~/Desktop
    */
    system("mkdir ~/.config/treebark");
    system("cp -r resources/config/* ~/.config/treebark/");

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button_addtopath))){
        char * cmd[255];
        strcpy((char *)cmd,"cp build/treebark ");
        strcat((char *)cmd,(const char *)gtk_entry_get_text(GTK_ENTRY(path_entry)));
        system((const char *)cmd);
    }

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button_addtoapps))){
        system("cp Treebark.desktop /usr/share/applications/");
    }

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button_addtodesktop))){
        system("cp Treebark.desktop ~/Desktop");
    }

    next_page();
};

void load_page(){
    switch(page){
        case 0:
        {
            gtk_label_set_markup (GTK_LABEL(label),"<span style=\"italic\" size=\"large\">\%s</span>");
            gtk_label_set_text(GTK_LABEL(label),"Welcome to the Treebark Installer! You are about to install the Treebark Text Editor on your system. \nThis installation process will move some files from the installation to your .config directory. \nIt will also, if you wish, create a desktop shortcut");
            break;
        }
        case 1:
        {
            gtk_label_set_text(GTK_LABEL(label),"IMPORTANT!\nMake sure you run this installer from the root folder of your installation.\nIf you are not there, please click \"Cancel\" and run the installer again from the right location");
            break;
        }
        case 2:
        {
            gtk_label_set_text(GTK_LABEL(label),"Please read through this license and make sure you agree to the terms of use.");
            gtk_widget_set_sensitive(GTK_WIDGET(next_button), FALSE);
            view = gtk_text_view_new ();
            gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
            gtk_widget_set_size_request(GTK_WIDGET(label), 600, 100);

            buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
            gtk_text_buffer_set_text (buffer, "", -1);
            gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);

            swindow = gtk_scrolled_window_new(NULL, NULL);
            gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
            //gtk_widget_set_size_request(GTK_WIDGET(swindow), 600, 600);

            gtk_container_add(GTK_CONTAINER(swindow), view);

            gtk_grid_attach(GTK_GRID(grid), swindow, 0, 5, 5, 1);

            gtk_widget_set_size_request(GTK_WIDGET(swindow), 600, 300);

            gtk_widget_set_size_request(GTK_WIDGET(view), 600, 600);

            FILE * fp;
            char line[255];
            GtkTextIter end;

            gtk_text_buffer_get_iter_at_offset (buffer, &end, -1);

            fp = fopen("COPYING","r");
            
            if(fp == NULL)
                return;

            gtk_text_buffer_set_text (buffer,"",-1);
            
            while(fgets(line, 255, fp)){
                gtk_text_buffer_insert(buffer,&end,line,-1);
                gtk_text_buffer_get_iter_at_offset (buffer, &end, -1);
            }

            //gtk_text_buffer_set_text(buffer,(const gchar *)text,-1);

            fclose(fp);

            agreement_button = gtk_check_button_new_with_label("I agree to the terms");
            gtk_grid_attach(GTK_GRID(grid), agreement_button, 0, 6, 1, 1);

            g_signal_connect(GTK_WIDGET(agreement_button),"clicked",G_CALLBACK(agreement_click), NULL);

            gtk_widget_show_all(window);

            break;
        }
        case 3: {
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_insert_row(GTK_GRID(grid),5);
            gtk_grid_insert_row(GTK_GRID(grid),5);
            gtk_grid_insert_row(GTK_GRID(grid),5);
            gtk_widget_set_size_request(GTK_WIDGET(label), 600, 300);
            gtk_window_resize(GTK_WINDOW(window), 600, 300);

            gtk_label_set_text(GTK_LABEL(label),"Treebark can be installed from a full download or a download of the installer only.\nIf you have downloaded just the installer, git is required on your system.\nPlease select the method of installation you will be using.");

            radio1 = gtk_radio_button_new_with_label(NULL,"Install from predownloaded resources");
            radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1),"Install with Git");

            box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
            gtk_box_pack_start (GTK_BOX (box), radio1,TRUE,FALSE,0);
            gtk_box_pack_start (GTK_BOX (box), radio2,TRUE,FALSE,0);

            g_signal_connect(GTK_WIDGET(radio1),"toggled",G_CALLBACK(radio_toggled), NULL);
            g_signal_connect(GTK_WIDGET(radio2),"toggled",G_CALLBACK(radio_toggled), NULL);

            gtk_grid_attach (GTK_GRID(grid),box,0,5,5,1);

            gtk_widget_show_all(window);

            break;
        }
        case 4:{
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_insert_row(GTK_GRID(grid),5);
            gtk_grid_insert_row(GTK_GRID(grid),5);
            gtk_widget_set_size_request(GTK_WIDGET(label), 600, 300);
            gtk_window_resize(GTK_WINDOW(window), 600, 300);
            gtk_widget_set_sensitive(GTK_WIDGET(next_button), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(install_button), TRUE);
            if(install_method==1){
                system("git clone https://github.com/ImperialOfficer324/treebark-text-editor.git");
                //system("cd treebark-text-editor");
                system("mv treebark-text-editor/* ./");
                system("mkdir build");
                system("cd source && make");
                system("cd ../installer && make");
                system("cd ../");
            }
            
            /*
                installation steps:
                copy "resources/config/*" to ~/.config/treebark/
                optional: copy "build/editor" to path
                optional: copy "Treebark Text Editor" to  /usr/share/applications/
                optional: copy "Treebark Text Editor" to ~/Desktop
            */
            gtk_label_set_text(GTK_LABEL(label),"Please select your installation preferences:");
            button_addtopath = gtk_check_button_new_with_label("Add to Path:");
            gtk_grid_attach(GTK_GRID(grid), button_addtopath, 0, 6, 1, 1);

            path_entry = gtk_entry_new ();
            gtk_grid_attach(GTK_GRID(grid), path_entry,1,6,2,1);
            gtk_entry_set_text(GTK_ENTRY(path_entry),"~/.local/bin/");

            button_addtoapps = gtk_check_button_new_with_label("Add to Applications");
            gtk_grid_attach(GTK_GRID(grid), button_addtoapps, 0, 7, 5, 1);

            button_addtodesktop = gtk_check_button_new_with_label("Add to Desktop");
            gtk_grid_attach(GTK_GRID(grid), button_addtodesktop, 0, 8, 5, 1);

            gtk_widget_show_all(window);
            break;
        }
        case 5:
        {
            gtk_label_set_text(GTK_LABEL(label),"Treebark has been installed on your system. You can now exit this installer");
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_remove_row(GTK_GRID(grid),5);
            gtk_grid_insert_row(GTK_GRID(grid),5);
            gtk_grid_insert_row(GTK_GRID(grid),5);
            gtk_grid_insert_row(GTK_GRID(grid),5);
            gtk_widget_set_sensitive(GTK_WIDGET(next_button), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(install_button), TRUE);
            gtk_button_set_label(GTK_BUTTON(install_button),"Exit");
            g_signal_connect(GTK_WIDGET(install_button),"clicked",G_CALLBACK(gtk_main_quit),NULL);
            break;
        }
        default:
            gtk_label_set_text(GTK_LABEL(label),"Invalid Page. Please restart the installer");
    }
}

void next_page(){
    page++;
    load_page();
}

static void activate (GtkApplication *app, gpointer user_data){
    page = 0;
    //Create Grid and Window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Treebark Installer");
    gtk_window_resize(GTK_WINDOW(window), 600, 300);
    //gtk_window_set_resizable(GTK_WINDOW(window),0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Error. Invalid Page");
    gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(label),0,0,5,5);
    gtk_widget_set_size_request(GTK_WIDGET(label), 600, 300);
    gtk_label_set_markup (GTK_LABEL(label),"<span style=\"italic\">\%s</span>");
    load_page();

    cancel_button = gtk_button_new_with_label("Cancel");
    gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(cancel_button),0,10,1,1);
    g_signal_connect(cancel_button,"clicked",G_CALLBACK(gtk_main_quit),NULL);

    next_button = gtk_button_new_with_label("Next>");
    gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(next_button),3,10,1,1);
    g_signal_connect(next_button,"clicked",G_CALLBACK(next_page),NULL);

    install_button = gtk_button_new_with_label("Install");
    gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(install_button),4,10,1,1);
    g_signal_connect(install_button,"clicked",G_CALLBACK(install),NULL);
    gtk_widget_set_sensitive(GTK_WIDGET(install_button), FALSE);

    gtk_widget_show_all(window);
    gtk_main();
}

int main(int argc, char **argv){
    GtkApplication *app;
    int status;

    app = gtk_application_new("treebark.install", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}