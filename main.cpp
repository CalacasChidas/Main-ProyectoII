#include <iostream>
#include <gtk/gtk.h>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

GtkWidget *window;
GtkWidget *botones[10][10], *botonPrueba;
GtkLabel *cantKoban;
int clientSocket;
int cantK = 1 + rand() % 100;
char buffer[10];


void on_button_clicked(GtkWidget *widget, gpointer data) {
    std::cout<<"Funciona\n";
    const gchar *nombre = gtk_widget_get_name(widget);
    if (nombre != nullptr) {
        // Imprimir el nombre del botón
        std::cout << "Nombre del botón: " << nombre << std::endl;
    }
    /*
    const gchar *nombre = gtk_widget_get_name(widget);
    if (nombre != nullptr && strncmp(nombre, "b", 1) == 0) {
        // Extraer la fila y la columna del nombre del botón
        int fila, columna;
        if (sscanf(nombre + 1, "%d%d", &fila, &columna) == 2) {
            // Imprimir las coordenadas
            std::cout << "Fila: " << fila << ", Columna: " << columna << std::endl;
        }
    }
     */
}
int main(int argc, char *argv[]) {
    sprintf(buffer, "%d", cantK);
    GtkBuilder *builder;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/aleprominecraft/Documents/github/Main-ProyectoII/GladeProyectoII.glade", NULL);

    //Declaración de objetos
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window"));
    GtkWidget *grid = gtk_grid_new();

    for (int fila = 0; fila < 10; fila++) {
        for (int columna = 0; columna < 10; columna++) {
            char nombreBoton[20];
            sprintf(nombreBoton, "b%d%d", fila, columna);
            botones[fila][columna] = GTK_WIDGET(gtk_builder_get_object(builder, nombreBoton));
            std::cout<<nombreBoton;

            g_signal_connect(G_OBJECT(botones[fila][columna]), "clicked", G_CALLBACK(on_button_clicked), NULL);
        }
    }
    botonPrueba = GTK_WIDGET(gtk_builder_get_object(builder, "BotonPrueba"));
    g_signal_connect(G_OBJECT(botonPrueba), "clicked", G_CALLBACK(on_button_clicked), NULL);

    cantKoban = GTK_LABEL(gtk_builder_get_object(builder, "cantKoban"));
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






