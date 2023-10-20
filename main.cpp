#include <iostream>
#include <gtk/gtk.h>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

GtkWidget *window;
GtkWidget *entryMessage1;
GtkWidget *entryMessage2;
GtkWidget *buttonSend;
int clientSocket;

static void send_message(GtkWidget *widget, gpointer data) {
    const char *message1 = gtk_entry_get_text(GTK_ENTRY(entryMessage1));
    const char *message2 = gtk_entry_get_text(GTK_ENTRY(entryMessage2));

    // Enviar la palabra clave "suma" al servidor
    if (send(clientSocket, "suma", strlen("suma"), 0) == -1) {
        std::cerr << "Error al enviar datos al servidor" << std::endl;
        return;
    }

    // Enviar los dos números al servidor
    if (send(clientSocket, message1, strlen(message1), 0) == -1 || send(clientSocket, message2, strlen(message2), 0) == -1) {
        std::cerr << "Error al enviar datos al servidor" << std::endl;
        return;
    }

    char buffer[1024] = {0};
    if (recv(clientSocket, buffer, sizeof(buffer), 0) > 0) {
        // Mostrar el resultado de la suma del servidor
        std::cout << "Resultado de la suma: " << buffer << std::endl;
    }
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/aleprominecraft/Documents/github/Main-ProyectoII/GladeProyectoII.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window"));
    entryMessage1 = GTK_WIDGET(gtk_builder_get_object(builder, "entryMessage1"));
    entryMessage2 = GTK_WIDGET(gtk_builder_get_object(builder, "entryMessage2"));
    buttonSend = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSend"));

    g_signal_connect(buttonSend, "clicked", G_CALLBACK(send_message), NULL);

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





