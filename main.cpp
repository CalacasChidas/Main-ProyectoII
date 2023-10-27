#include <iostream>
#include <gtk/gtk.h>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <queue>
#include <algorithm>
#include <gtk/gtk.h>
#include <sstream>
#include <iostream>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

GtkWidget *window;
GtkWidget *botones[10][10], *iter, *yr, *af, *tn;
GtkComboBoxText *obstaculos;
GtkLabel *cantKoban, *advertencias;
int clientSocket, obstype;
int cantK = 9, cantiter = 0, tercios = 0, obstavailable = 3;
char buffer[10];

int suma(int a, int b) {
    return a + b;
}

const int matrixSize = 10;

struct Node {
    int x, y;
    int f, g, h;
    Node* parent;

    Node(int x, int y, int g, int h, Node* parent)
            : x(x), y(y), g(g), h(h), parent(parent) {
        f = g + h;
    }
};

// Funcion que calcula la distancia de manhattan para dos nodos
int calculateManhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Funcion que encuentra el camino del samurai a la meta
vector<pair<int, int>> findPath(int matrix[matrixSize][matrixSize], int startX, int startY, int targetX, int targetY) {
    vector<pair<int, int>> path;
    priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> openSet(
            [](Node* a, Node* b) { return a->f > b->f; });

    Node* startNode = new Node(startX, startY, 0, calculateManhattanDistance(startX, startY, targetX, targetY), nullptr);
    openSet.push(startNode);

    while (!openSet.empty()) {
        Node* currentNode = openSet.top();
        openSet.pop();

        if (currentNode->x == targetX && currentNode->y == targetY) {
            // Reconstruye el camino
            while (currentNode != nullptr) {
                path.emplace_back(currentNode->x, currentNode->y);
                currentNode = currentNode->parent;
            }
            reverse(path.begin(), path.end());
            break;
        }

        // Genera nodos vecinos
        const int dx[4] = {1, -1, 0, 0};
        const int dy[4] = {0, 0, 1, -1};
        for (int i = 0; i < 4; ++i) {
            int nextX = currentNode->x + dx[i];
            int nextY = currentNode->y + dy[i];

            if (nextX >= 0 && nextX < matrixSize && nextY >= 0 && nextY < matrixSize && matrix[nextX][nextY] == 0) {
                int g = currentNode->g + 1;
                int h = calculateManhattanDistance(nextX, nextY, targetX, targetY);
                Node* neighbor = new Node(nextX, nextY, g, h, currentNode);
                openSet.push(neighbor);
            }
        }
    }

    // Limpia la memoria
    while (!openSet.empty()) {
        delete openSet.top();
        openSet.pop();
    }

    return path;
}

const int boardSize = 10;
bool visited[boardSize][boardSize];

bool isSafe(int x, int y) {
    return (x >= 0 && y >= 0 && x < boardSize && y < boardSize && !visited[x][y]);
}

bool solveMaze(int x, int y, int destX, int destY, std::vector<std::pair<int, int>>& path) {
    if (x == destX && y == destY) {
        return true; // Llegamos al destino
    }

    if (isSafe(x, y)) {
        visited[x][y] = true;
        path.push_back(std::make_pair(x, y));

        // Movimiento hacia abajo
        if (solveMaze(x + 1, y, destX, destY, path)) {
            return true;
        }

        // Movimiento hacia la derecha
        if (solveMaze(x, y + 1, destX, destY, path)) {
            return true;
        }

        // Si no se encuentra una solución en esta dirección, retrocedemos
        visited[x][y] = false;
        path.pop_back();
    }

    return false;
}

struct Samurai {
    int uniqueId;
    int age;
    double survivalProbability;
    int expectedGenerations;
    int emotionalIntelligence;
    int physicalCondition;
    int upperBodyStrength;
    int lowerBodyStrength;
    double resistance;

    Samurai(int generation) {
        // Generar un identificador único aleatorio de 7 dígitos en cada generación
        uniqueId = rand() % 10000000 + 1000000;

        if (generation == 1) {
            // Inicializar los atributos en la primera generación
            age = 18;
            survivalProbability = 20.0;
            expectedGenerations = 4;
            emotionalIntelligence = 4;
            physicalCondition = 4;
            upperBodyStrength = 4;
            lowerBodyStrength = 4;
            resistance = calculateResistance(); // Calcular la resistencia inicial
        }
        else {
            evolve(); // Evolucionar los atributos en generaciones posteriores
        }
    }

    double calculateResistance() const {
        return age + emotionalIntelligence + physicalCondition + upperBodyStrength + lowerBodyStrength;
    }

    // Función para aplicar el incremento del 50% en cada atributo en cada generación
    void evolve() {
        age = static_cast<int>(age * 1.2);
        survivalProbability *= 1.5;
        expectedGenerations = static_cast<int>(expectedGenerations - 1);
        emotionalIntelligence = static_cast<int>(emotionalIntelligence * 1.5);
        physicalCondition = static_cast<int>(physicalCondition * 1.5);
        upperBodyStrength = static_cast<int>(upperBodyStrength * 1.5);
        lowerBodyStrength = static_cast<int>(lowerBodyStrength * 1.5);
        resistance = calculateResistance();
    }

    void printAttributes() const {
        cout << "Samurái " << uniqueId << " - Atributos:\n";
        cout << "Edad: " << age << "\n";
        cout << "Probabilidad de supervivencia: " << survivalProbability << "%\n";
        cout << "Generaciones esperadas de supervivencia: " << expectedGenerations << "\n";
        cout << "Inteligencia emocional: " << emotionalIntelligence << "\n";
        cout << "Condición física: " << physicalCondition << "\n";
        cout << "Fuerza en tronco superior: " << upperBodyStrength << "\n";
        cout << "Fuerza en tronco inferior: " << lowerBodyStrength << "\n";
        cout << "Resistencia: " << resistance << "\n";
    }

};

void on_button_clicked(GtkWidget *widget, gpointer data) {
    sprintf(buffer, "%d", cantK);
    if (obstavailable>0){
        switch (obstype) {
            // Verificamos si el widget es un botón
            case 1:
                gtk_button_set_label(GTK_BUTTON(widget), "1");
                obstype = 0;
                cantK--;
                obstavailable--;
                gtk_label_set_text(cantKoban, buffer);
                gtk_label_set_text(advertencias, " ");
                break;
            case 2:
                gtk_button_set_label(GTK_BUTTON(widget), "2");
                obstype = 0;
                cantK = cantK - 2;
                obstavailable--;
                gtk_label_set_text(cantKoban, buffer);
                gtk_label_set_text(advertencias, " ");
                break;
            case 3:
                gtk_button_set_label(GTK_BUTTON(widget), "3");
                obstype = 0;
                cantK = cantK - 3;
                obstavailable--;
                gtk_label_set_text(cantKoban, buffer);
                gtk_label_set_text(advertencias, " ");
                break;
            default:
                gtk_label_set_text(advertencias, "Seleccione un obstáculo primero!");
                std::cout << "Obstáculo no seleccionado\n";
                break;
        }
    }else{
        gtk_label_set_text(advertencias, "Cantidad de objetos máxima alcanzada");
    }
}

gboolean clearLabelCallback(gpointer data) {
    // Esta función se ejecutará después de 2 segundos
    gtk_label_set_text(advertencias, " ");
    return G_SOURCE_REMOVE;  // Indica que se debe eliminar la fuente de temporización
}

void yari(GtkWidget *widget, gpointer data) {
    obstype = 1;
    std::cout<< "Obstáculo seleccionado: Yari\n";
    gtk_label_set_text(advertencias, "YObstáculo seleccionado: Yari");
    g_timeout_add(1000, clearLabelCallback, NULL);
}
void ayf(GtkWidget *widget, gpointer data) {
    obstype = 2;
    std::cout<< "Obstáculo seleccionado: Arco y flecha\n";
    gtk_label_set_text(advertencias, "Obstáculo seleccionado: Arco y flecha");
    g_timeout_add(1000, clearLabelCallback, NULL);
}
void tna(GtkWidget *widget, gpointer data) {
    obstype = 3;
    std::cout<< "Obstáculo seleccionado: Tanegashima\n";
    gtk_label_set_text(advertencias, "Obstáculo seleccionado: Tanegashima");
    g_timeout_add(1000, clearLabelCallback, NULL);
}

void iteraciones(GtkWidget *widget, gpointer data) {
    sprintf(buffer, "%d", cantK);
    if(tercios == 3){
        cantiter++;
        tercios = 0;
        std::cout<<"Iterando, se han cumplido 3 iteraciones\n";
        gtk_label_set_text(advertencias, "+9 konan!");
        cantK = cantK + 9;
        gtk_label_set_text(cantKoban, buffer);
        g_timeout_add(2000, clearLabelCallback, NULL);
        for (int fila = 0; fila < 10; fila++) {
            for (int columna = 0; columna < 10; columna++) {
                gtk_button_set_label(GTK_BUTTON(botones[fila][columna]), " ");
            }
        }

    }else{
        cantiter++;
        tercios++;
        obstavailable = obstavailable+3;
        std::cout<<"Iterando\n";
        int matrix[10][10]; // Declara la matrix 10x10
        int cont = 0;
        srand(time(0));

        // Inicia la matriz con ceros
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                matrix[i][j] = 0;
            }
        }

        for (int i = 0; i < 9; i++) {
            matrix[rand() % 10][rand() % 10] = 1;
        }

        // Defina el punto de inicio y de llegada de los samurai
        int startX = 0;
        int startY = 0;
        int targetX = 9;
        int targetY = 9;

        vector<pair<int, int>> path = findPath(matrix, startX, startY, targetX, targetY);

        // Enseña el camino del samurai
        cout << "Camino del samurai (usando pathfinding): ";
        for (const auto& point : path) {
            cout << "(" << point.first << ", " << point.second << ") ";
        }
        cout << endl;

        cout << "Camino del samurai (usando backtracking): ";
        for (const auto& point : path) {
            cout << "(" << point.first << ", " << point.second << ") ";
        }
        cout << endl;

        std::srand(static_cast<unsigned>(std::time(0)));




    }
}

int main(int argc, char *argv[]) {
    sprintf(buffer, "%d", cantK);
    GtkBuilder *builder;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/tomeito/CLionProjects/Main-ProyectoII/GladeProyectoII.glade", NULL);

    //Declaración de objetos
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window"));


    for (int fila = 0; fila < 10; fila++) {
        for (int columna = 0; columna < 10; columna++) {
            char nombreBoton[20];
            sprintf(nombreBoton, "b%d%d", fila, columna);
            botones[fila][columna] = GTK_WIDGET(gtk_builder_get_object(builder, nombreBoton));

            //std::cout<<nombreBoton;
            gtk_button_set_label(GTK_BUTTON(botones[fila][columna]), " ");
            g_signal_connect(G_OBJECT(botones[fila][columna]), "clicked", G_CALLBACK(on_button_clicked), NULL);
        }
    }

    iter = GTK_WIDGET(gtk_builder_get_object(builder, "iteracion"));
    g_signal_connect(G_OBJECT(iter), "clicked", G_CALLBACK(iteraciones), NULL);

    yr = GTK_WIDGET(gtk_builder_get_object(builder, "yr"));
    g_signal_connect(G_OBJECT(yr), "clicked", G_CALLBACK(yari), NULL);

    af = GTK_WIDGET(gtk_builder_get_object(builder, "af"));
    g_signal_connect(G_OBJECT(af), "clicked", G_CALLBACK(ayf), NULL);

    tn = GTK_WIDGET(gtk_builder_get_object(builder, "tn"));
    g_signal_connect(G_OBJECT(tn), "clicked", G_CALLBACK(tna), NULL);


    cantKoban = GTK_LABEL(gtk_builder_get_object(builder, "cantKoban"));
    advertencias = GTK_LABEL(gtk_builder_get_object(builder, "advertencias"));
    gtk_label_set_text(cantKoban, buffer);

    int matrix[10][10]; // Declara la matrix 10x10
    int cont = 0;
    srand(time(0));

    // Inicia la matriz con ceros
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < 9; i++) {
        matrix[rand() % 10][rand() % 10] = 1;
    }

    // Defina el punto de inicio y de llegada de los samurai
    int startX = 0;
    int startY = 0;
    int targetX = 9;
    int targetY = 9;

    vector<pair<int, int>> path = findPath(matrix, startX, startY, targetX, targetY);

    // Enseña el camino del samurai
    cout << "Camino del samurai (usando pathfinding): ";
    for (const auto& point : path) {
        cout << "(" << point.first << ", " << point.second << ") ";
    }
    cout << endl;

    std::srand(static_cast<unsigned>(std::time(0)));

    int inputX = 0;
    int inputY = 0;
    int destX = boardSize - 1;
    int destY = boardSize - 1;
    std::vector<std::pair<int, int>> path1;

    auto start = std::chrono::high_resolution_clock::now(); // Iniciar el temporizador

    if (solveMaze(inputX, inputY, destX, destY, path1)) {
        std::cout << "Camino encontrado (usando backtracking):" << std::endl;
        for (const auto& step : path1) {
            std::cout << "(" << step.first << ", " << step.second << ")" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Retraso de 1 segundo entre movimientos
        }
    } else {
        std::cout << "No se encontró un camino." << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now(); // Detener el temporizador
    std::chrono::duration<double> duration = end - start;

    std::cout << "Tiempo de ejecución: " << duration.count() << " segundos" << std::endl;

    /* LO DEL SOCKET
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
    */
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();

    close(clientSocket);

    return 0;
}






