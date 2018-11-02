#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>

//https://pastebin.com/WgH1Nb5h

void *read_request(void *parameter) {
    uint8_t buf[1024];
    int it = 0;
    int sock = (int) parameter;

    while ((int) recv(sock, buf + it, 1, 0) > 0) {
        if (buf[it] == '\r' || buf[it] == '\n') {
            buf[it] = 0;
            break;
        }

        if (it + 1 >= sizeof(buf) / sizeof(buf[0])) {
            break;
        }

        buf[++it] = 0;
    }
    send(sock, buf, (size_t) it, 0);
    fprintf(stdout, "%s\n", buf);
    close(sock);
    pthread_exit(0);
}

int
socket_listen_server(const char *port) {
    struct addrinfo *result = NULL, hints;
    int ret;
    int option = 1;
    int listen_socket;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    ret = getaddrinfo(NULL, port, &hints, &result);
    if (ret != 0) {
        fprintf(stderr, "[server] getaddrinfo failed: %d\n", ret);
        return -1;
    }
    listen_socket = socket(
            result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == -1) {
        fprintf(stderr, "[server] error at socket(): %i\n", errno);
        freeaddrinfo(result);
        return -1;
    }
    setsockopt(listen_socket,
               SOL_SOCKET, SO_REUSEADDR, (const char *) &option, sizeof(option));
    setsockopt(listen_socket,
               SOL_SOCKET, SO_DEBUG, (const char *) &option, sizeof(option));
    ret = bind(listen_socket,
               result->ai_addr, (int) result->ai_addrlen);
    if (ret == -1) {
        fprintf(stderr, "[server] bind failed with error: %d\n", errno);
        freeaddrinfo(result);
        close(listen_socket);
        return -1;
    }
    if (listen(listen_socket, SOMAXCONN) == -1) {
        fprintf(stderr, "[server] listen failed with error: %i\n", errno);
        close(listen_socket);
        return -1;
    }
    freeaddrinfo(result);
    return listen_socket;
}

int main(int argc, char **argv) {
    int client_socket;
    struct sockaddr_in sock_addr_in;
    int sock_addr_len = sizeof(sock_addr_in);
    int listen_socket = socket_listen_server("7860");
    bool running = true;

    while (running) {
        pthread_t t;
        client_socket = accept(listen_socket,
                               (struct sockaddr *) &sock_addr_in,
                               (socklen_t *) &sock_addr_len);
        running = client_socket != -1;
        if (pthread_create(&t, 0, read_request, (void *) client_socket) == 0) {
            pthread_detach(t);
        }
    }
    return EXIT_SUCCESS;
}


