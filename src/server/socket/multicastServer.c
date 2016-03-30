//
// Created by yata on 24.12.15.
//

#include "multicastServer.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

void *startMulticastServer(void *quit) {
    int sock;       // Socket
    char *sendString; // Buffer for received data
    struct addrinfo *multicastAddr;

    quit_var_with_mutex *quit_struct = (quit_var_with_mutex*) quit;

    sendString = calloc(MULTICAST_BUFFER_LENGTH, sizeof(char));
    if (sendString == NULL) {
        perror("calloc");
        pthread_exit((void *)-1);
    }
    snprintf(sendString, MULTICAST_BUFFER_LENGTH, "Hello ! I'm looking for a Raspberry Pi...");

    sock = mcast_send_socket(MULTICAST_IP, MULTICAST_PORT, MULTICAST_TTL, &multicastAddr);
    if (sock < 0) {
        fprintf(stderr, "[multicast server] mcast_send_socket() failed");
        free(sendString);
        pthread_exit((void *)-1);
    }

    printf("[multicast server] Sending multicast data...\n");
    while (1) {
        if (sendto(sock,
                   sendString,
                   MULTICAST_BUFFER_LENGTH,
                   0,
                   multicastAddr->ai_addr,
                   multicastAddr->ai_addrlen
        ) != MULTICAST_BUFFER_LENGTH) {
            fprintf(stderr, "[multicast server] sendto() sent a different number of bytes than expected\n");
        }

        // Check if the caller wants the thread to quit
        pthread_mutex_lock(&quit_struct->mutex);
        if(quit_struct->quit) {
            pthread_mutex_unlock(&quit_struct->mutex);
            break;
        }
        pthread_mutex_unlock(&quit_struct->mutex);

        // Send multicast data each 2 seconds
        sleep(2);
    }

    free(sendString);

    close(sock);
    printf("[multicast server] Socket closed\n");

    pthread_exit(NULL);
}


int mcast_send_socket(char *multicastIP, char *multicastPort, int multicastTTL, struct addrinfo **multicastAddr) {
    int sock;
    struct addrinfo hints;

    // Resolve destination address for multicast datagrams
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_NUMERICHOST;    /* For numerical IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int status;
    if ((status = getaddrinfo(multicastIP, multicastPort, &hints, multicastAddr)) != 0) {
        fprintf(stderr, "[multicast server] getaddrinfo : %s\n", gai_strerror(status));
    }

    // Create socket
    if ((sock = socket((*multicastAddr)->ai_family, (*multicastAddr)->ai_socktype, 0)) < 0) {
        perror("socket()");
        freeaddrinfo(*multicastAddr);
        return -1;
    }

    //   Set TTL of multicast packet
    if (setsockopt(sock,
                   (*multicastAddr)->ai_family == AF_INET6 ? IPPROTO_IPV6 : IPPROTO_IP,
                   (*multicastAddr)->ai_family == AF_INET6 ? IPV6_MULTICAST_HOPS : IP_MULTICAST_TTL,
                   (char *) &multicastTTL, sizeof(multicastTTL)) != 0) {
        perror("[multicast server] setsockopt() failed");
        freeaddrinfo(*multicastAddr);
        return -1;
    }

    // set the sending interface
    if ((*multicastAddr)->ai_family == PF_INET) { // IPv4
        in_addr_t iface = INADDR_ANY;
        if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char *) &iface, sizeof(iface)) != 0) {
            perror("[multicast server] interface setsockopt() sending interface");
            freeaddrinfo(*multicastAddr);
            return -1;
        }
    }
    if ((*multicastAddr)->ai_family == PF_INET6) { // IPv6
        unsigned int ifindex = 0; // 0 means 'default interface'
        if (setsockopt(sock,
                       IPPROTO_IPV6,
                       IPV6_MULTICAST_IF,
                       (char *) &ifindex, sizeof(ifindex)) != 0) {
            perror("[multicast server] interface setsockopt() sending interface");
            freeaddrinfo(*multicastAddr);
            return -1;
        }
    }

    return sock;
}
