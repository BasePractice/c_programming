#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#if defined(WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#define closesocket close
#endif

static inline int get_network_error() {
#if defined(WIN32)
    return (int)WSAGetLastError();
#else
    return errno;
#endif
}

#define ECHO_REQUEST "Hi. I am echo client\n"
#define ECHO_REQUEST_SIZE (sizeof(ECHO_REQUEST)/sizeof(char))
void echo_request(int s) {
    int ret;

    size_t n = strlen(ECHO_REQUEST);
    ret = (int) send(s, ECHO_REQUEST, n, 0);
    if (ret != -1) {
        char buffer[ECHO_REQUEST_SIZE];

        fprintf(stdout, "Sent %d bytes, need %lu bytes\n", ret, n);
        ret = (int)recv(s, buffer, sizeof(buffer)/ sizeof(buffer[0]), 0);
        if (ret > 0) {
            buffer[ret] = 0;
            fprintf(stdout, "%s", buffer);
        }
    } else {
        fprintf(stderr, "Can't send http request %d\n", get_network_error());
    }
}

int main(int argc, char **argv) {
    int s;
    char *hostname = "144.76.109.199";
    char *port = "7860";

#if defined(WIN32)
    WSADATA wsa_data;
    int ret;

    ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (ret != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", ret);
    }
#endif
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s > 0) {
        int ret;
        struct addrinfo *result = NULL, hints = {};

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        ret = getaddrinfo(hostname, port, &hints, &result);
        if (ret == 0) {
            ret = connect(s, result->ai_addr, (int) result->ai_addrlen);
            if (ret != -1) {
                echo_request(s);
            } else {
                fprintf(stderr, "Can't connect to hostname %s. Error: %d.\n", hostname, get_network_error());
            }
            freeaddrinfo(result);
        } else {
            fprintf(stderr, "Can't resolve hostname %s. Error: %d.\n", hostname, get_network_error());
        }
        closesocket(s);
    } else {
        fprintf(stderr, "Can't create socket. Error: %d.\n", get_network_error());
    }
#if defined(WIN32)
    WSACleanup();
#endif
    return EXIT_SUCCESS;
}
