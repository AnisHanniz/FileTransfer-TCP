
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
#include <fcntl.h>

#define PORT 3264
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void serve_file(const char *directory) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    FILE *file;
    char buffer[BUFFER_SIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        error("Erreur socket");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Erreur bind");
    }

    if (listen(server_socket, 1) < 0) {
        error("Erreur listen");
    }

    printf("Le serveur écoute le port %d\n", PORT);

    while (1) {
        // Accept connection
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            error("Erreur connexion");
        }

        printf("Connexion établie avec %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        int window_size;
        if (recv(client_socket, &window_size, sizeof(window_size), 0) < 0) {
            error("Erreur taille de fenêtre");
        }

        char filename[BUFFER_SIZE];
        if (recv(client_socket, filename, BUFFER_SIZE, 0) < 0) {
            error("Erreur fichier");
        }

        size_t filename_len = strlen(filename);
        if (filename_len > 0 && filename[filename_len - 1] == '\n') {
            filename[filename_len - 1] = '\0';
        }

        printf("Taille fenêtre: %d\n", window_size);
        printf("Fichier: %s\n", filename);

        char file_path[1025];
        snprintf(file_path, sizeof(file_path), "%s/%s", directory, filename);
        file = fopen(file_path, "rw+");
        if (file == NULL) {
            perror("Erreur ouverture fichier");
            send(client_socket, buffer, 0, 0);
            send(client_socket, "Fichier inconnu ou permissions manquantes", strlen("Fichier inconnu ou permissions manquantes"), 0);
            close(client_socket);
            continue;
        }

        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
            send(client_socket, buffer, bytes_read, 0);
        }

        if (bytes_read == 0) {
            send(client_socket, buffer, 0, 0);
        }

        fclose(file);
        close(client_socket);

        FILE *log_file = fopen("/tmp/fileserver.log", "a");
        if (log_file != NULL) {
            fprintf(log_file, "Addresse: %s, Port: %d, Fichier: %s, Taille fenêtre: %d\n",
                    inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), filename, window_size);
            fclose(log_file);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *directory = argv[1];
    serve_file(directory);

    return 0;
}
