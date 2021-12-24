#include "message.h"
// int confirmation(GtkWindow *parent, char *message,char * button1, char * button2){
//     GtkWidget *dialog, *label, *content_area;
//     GtkDialogFlags flags;
//     gint res;

//     // Create the widgets
//     flags = GTK_DIALOG_DESTROY_WITH_PARENT;
//     dialog = gtk_dialog_new_with_buttons("Confirmation",parent,flags,button1,GTK_RESPONSE_CANCEL,button2,GTK_RESPONSE_ACCEPT,NULL);
//     content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
//     label = gtk_label_new (message);

//     // Ensure that the dialog box is destroyed when the user responds

//     g_signal_connect_swapped (dialog,"response",G_CALLBACK(gtk_widget_destroy),dialog);

//     // Add the label, and show everything we’ve added

//     gtk_box_pack_end(GTK_BOX(content_area), GTK_WIDGET(label),TRUE,FALSE,0);
//     gtk_widget_show(dialog);
//     res = gtk_dialog_run(GTK_DIALOG(dialog));
// }

gint confirmation(GtkWindow *window,char *message) {
    GtkWidget *dialog;
    gint res;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_YES_NO,
                message);
    gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation");
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    return res;
}