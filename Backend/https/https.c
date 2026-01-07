#ifndef HTTPS_C
#define HTTPS_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


#include <dirent.h>
#include <sys/stat.h>
#include <signal.h>




//#define CERT_FILE "/etc/letsencrypt/live/opaq.co.il/fullchain.pem"
//#define  KEY_FILE "/etc/letsencrypt/live/opaq.co.il/privkey.pem"

//#define ITL_C     "/home/itamar/Documents/Workspace/itl/code.itl"
#define BACKLOG 10


#include "ITL/itl.h"

#include "SSL.h"
#include "io.h"  // depends -> helpers, file


void* handle_http(void *arg) {

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in https;
    https.sin_family = AF_INET;
    https.sin_port = htons(80);
    https.sin_addr.s_addr = inet_addr(bind_addr); //INADDR_ANY;

    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt failed");
        close(s);
        exit(EXIT_FAILURE);
    }

    if (listen(s, BACKLOG) < 0) {
        perror("Listen failed");
        close(s);
        exit(EXIT_FAILURE);
    }


  while (1) {
        struct sockaddr_storage addr;

        socklen_t addr_len = sizeof(addr);

        int client_fd = accept(s, (struct sockaddr*)&addr, &addr_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        // Always redirect HTTP → HTTPS
        char *redirect = malloc(1024*(sizeof(char)));
        sprintf(redirect,"HTTP/1.1 301 Moved Permanently\r\nLocation: https://%s/\r\nConnection: close\r\n\r\n", domain);
         //sprintf(redirect, DOMAIN);
        send(client_fd, redirect, strlen(redirect), 0);
        close(client_fd);
    }

    return NULL;
}

    
    
    

void https_start(){

    load_program();

    files = getAllFilePaths(HOME);

    
    DUMP_PATHS for (int i = 0; i < files.count; i++)
      fprintf(stdout,"\e[94m%s\e[0m\n", files.paths[i]);
    
    SSL_CTX *ctx = create_ssl_context();
    configure_ssl_context(ctx);

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Socket creation failed");
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(https_port);
    addr.sin_addr.s_addr = inet_addr(bind_addr); //INADDR_ANY;

    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt failed");
        close(s);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }


    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(s);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    if (listen(s, BACKLOG) < 0) {
        perror("Listen failed");
        close(s);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout,"\e[0;93mHTTPS Server is listening on \e[96m%s\e[97m:\e[92m%d\e[0m\n", bind_addr, https_port);
    
       pthread_t thread;
    if (pthread_create(&thread, NULL, handle_http, NULL) != 0) {
      perror("Thread creation failed");
    } else {
      pthread_detach(thread);
    }

    while (1) {
        connection_t *connection = malloc(sizeof(connection_t));
        if (!connection) {
            perror("Memory allocation failed");
            continue;
        }

        socklen_t client_len = sizeof(connection->address);
        connection->sock = accept(s, (struct sockaddr *)&connection->address, &client_len);
        connection->ctx = ctx;

        if (connection->sock < 0) {
            perror("Accept failed");
            free(connection);
            continue;
        }

        // Spawn a new thread for handling the client
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, connection) != 0) {
            perror("Thread creation failed");
            close(connection->sock);
            free(connection);
        } else {
            pthread_detach(thread); // Detach the thread to avoid memory leaks
        }
    }

    fprintf(stdout,"SERVER CRASHED\n");
    close(s);
    SSL_CTX_free(ctx);
    for (int i = 0; i < files.count; i++)
        free(files.paths[i]);
    free(files.paths);
}
#endif
