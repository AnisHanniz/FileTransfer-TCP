
//Mon addresse IP : 192.168.50.45

//Son addresse IP : 192.168.50.18

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define SERVER_IP "192.168.50.18" // Adresse IP de mon partenaire
#define PORT 3264
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Utilisation: %s <window_size> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int window_size = atoi(argv[1]);
    if (window_size <= 0) {
        fprintf(stderr, "La taille de la fenêtre doit être un entier\n");
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[2];

    int client_socket;
    struct sockaddr_in server_addr;
    FILE *file;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        error("Erreur socket");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Erreur connect");
    }

    if (send(client_socket, &window_size, sizeof(window_size), 0) < 0) {
        error("Erreur send window size");
    }

    if (send(client_socket, filename, strlen(filename) + 1, 0) < 0) { 
        // Envoyer strlen(filename) + 1 octets
        error("Erreur send filename");
    }

    file = fopen(filename, "wb");
    if (file == NULL) {
        error("Erreur fopen");
    }

    ssize_t bytes_received;
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, (size_t)bytes_received, file);
    }

    fclose(file);
    close(client_socket);

    printf("Téléchargement Réussi !\n");

    return 0;
}