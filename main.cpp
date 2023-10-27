#include <iostream>
#include <gtk/gtk.h>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

GtkWidget *window;
GtkWidget *botones[10][10], *iter, *yr, *af, *tn;
GtkComboBoxText *obstaculos;
GtkLabel *cantKoban, *advertencias;
int clientSocket, obstype;
int cantK = 9;
char buffer[10];

void on_button_clicked(GtkWidget *widget, gpointer data) {
    switch (obstype) {
        // Verificamos si el widget es un botón
        case 1:
            gtk_button_set_label(GTK_BUTTON(widget), "1");
            obstype = 0;
            gtk_label_set_text(advertencias, " ");
            break;
        case 2:
            gtk_button_set_label(GTK_BUTTON(widget), "2");
            obstype = 0;
            gtk_label_set_text(advertencias, " ");
            break;
        case 3:
            gtk_button_set_label(GTK_BUTTON(widget), "3");
            obstype = 0;
            gtk_label_set_text(advertencias, " ");
            break;
        default:
            gtk_label_set_text(advertencias, "Seleccione un obstáculo primero!");
            std::cout << "Obstáculo no seleccionado\n";
            break;
    }
}
void yari(GtkWidget *widget, gpointer data) {
    obstype = 1;
    std::cout<< "Obstáculo seleccionado: Yari\n";
}
void ayf(GtkWidget *widget, gpointer data) {
    obstype = 2;
    std::cout<< "Obstáculo seleccionado: Arco y flecha\n";
}
void tna(GtkWidget *widget, gpointer data) {
    obstype = 3;
    std::cout<< "Obstáculo seleccionado: Tanegashima\n";
}

int main(int argc, char *argv[]) {
    sprintf(buffer, "%d", cantK);
    GtkBuilder *builder;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/aleprominecraft/Documents/github/Main-ProyectoII/GladeProyectoII.glade", NULL);

    //Declaración de objetos
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window"));


    for (int fila = 0; fila < 10; fila++) {
        for (int columna = 0; columna < 10; columna++) {
            char nombreBoton[20];
            sprintf(nombreBoton, "b%d%d", fila, columna);
            botones[fila][columna] = GTK_WIDGET(gtk_builder_get_object(builder, nombreBoton));
            //std::cout<<nombreBoton;

            g_signal_connect(G_OBJECT(botones[fila][columna]), "clicked", G_CALLBACK(on_button_clicked), NULL);
        }
    }

    iter = GTK_WIDGET(gtk_builder_get_object(builder, "iteracion"));
    g_signal_connect(G_OBJECT(iter), "clicked", G_CALLBACK(on_button_clicked), NULL);

    yr = GTK_WIDGET(gtk_builder_get_object(builder, "yr"));
    g_signal_connect(G_OBJECT(yr), "clicked", G_CALLBACK(yari), NULL);

    af = GTK_WIDGET(gtk_builder_get_object(builder, "af"));
    g_signal_connect(G_OBJECT(af), "clicked", G_CALLBACK(ayf), NULL);

    tn = GTK_WIDGET(gtk_builder_get_object(builder, "tn"));
    g_signal_connect(G_OBJECT(tn), "clicked", G_CALLBACK(tna), NULL);


    cantKoban = GTK_LABEL(gtk_builder_get_object(builder, "cantKoban"));
    advertencias = GTK_LABEL(gtk_builder_get_object(builder, "advertencias"));
    gtk_label_set_text(cantKoban, buffer);

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1) {
        std::cerr << "Error al crear el socket del cliente" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error al conectar al servidor" << std::endl;
        close(clientSocket);
        return 1;
    }

    std::cout << "Conectado al servidor." << std::endl;

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();

    close(clientSocket);

    return 0;
}






