//
// Created by yata on 24.12.15.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <netdb.h>
#include "multicastClient.h"

int startMulticastClient(char *ip) {
    int sock;       // Socket
    char *recvBuf;  // Buffer for received data

    recvBuf = malloc(MULTICAST_BUFFER_LENGTH * sizeof(char));
    if(recvBuf == NULL){
        perror("malloc");
        return -1;
    }

    struct sockaddr_in6 reply_sin; // It will contain the server information
    socklen_t sizeReplySin = sizeof reply_sin;
    memset(&reply_sin, 0, sizeReplySin);

    sock = mcast_recv_socket(MULTICAST_IP, MULTICAST_PORT, MULTICAST_SO_RCVBUF);
    if (sock < 0) {
        return -1;
    }

    printf("[multicast client] Looking for the server...\n");
    // Receive a single datagram from the server
    if (recvfrom(sock, recvBuf, MULTICAST_BUFFER_LENGTH, 0, (struct sockaddr *) &reply_sin, &sizeReplySin) < 0) {
        perror("recvfrom() failed");
        free(recvBuf);
        close(sock);
        return -1;
    }
    printf("Trouvé\n");

    inet_ntop(AF_INET6, &(reply_sin.sin6_addr), ip, INET6_ADDRSTRLEN);

    free(recvBuf);
    close(sock);

    return 0;
}

int mcast_recv_socket(char *multicastIP, char *multicastPort, int multicastRecvBufSize) {
    int sock;
    struct addrinfo hints;
    struct addrinfo *localAddr = 0;         /* Local address to bind to */
    struct addrinfo *multicastAddr = 0;     /* Multicast Address */
    int yes = 1;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        // Allow IPv4 or IPv6
    hints.ai_flags = AI_NUMERICHOST;    // For numeric IP address

    int status;
    if ((status = getaddrinfo(multicastIP, NULL, &hints, &multicastAddr)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        goto error;
    }

    /*
       Get a local address with the same family (IPv4 or IPv6) as our multicast group
       This is for receiving on a certain port.
    */
    hints.ai_family = multicastAddr->ai_family;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // Return an address we can bind to
    if (getaddrinfo(NULL, multicastPort, &hints, &localAddr) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        goto error;
    }

    // Create socket for receiving datagrams
    if ((sock = socket(localAddr->ai_family, localAddr->ai_socktype, 0)) < 0) {
        perror("socket() failed");
        goto error;
    }



    /*
     * Enable SO_REUSEADDR to allow multiple instances of this
     * application to receive copies of the multicast datagrams.
     */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        goto error;
    }

    /* Bind the local address to the multicast port */
    if (bind(sock, localAddr->ai_addr, localAddr->ai_addrlen) != 0) {
        perror("bind() failed");
        goto error;
    }

    /* get/set socket receive buffer */
    int optval = 0;
    socklen_t optval_len = sizeof(optval);

    optval = multicastRecvBufSize;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *) &optval, sizeof(optval)) != 0) {
        perror("setsockopt");
        goto error;
    }
    if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *) &optval, &optval_len) != 0) {
        perror("getsockopt");
        goto error;
    }

    printf("tried to set socket receive buffer to %d, got %d\n", multicastRecvBufSize, optval);




    /* Join the multicast group. We do this separately depending on whether we
     * are using IPv4 or IPv6.
     */
    if (multicastAddr->ai_family == PF_INET &&
        multicastAddr->ai_addrlen == sizeof(struct sockaddr_in)) /* IPv4 */
    {
        struct ip_mreq multicastRequest;  /* Multicast address join structure */

        /* Specify the multicast group */
        memcpy(&multicastRequest.imr_multiaddr,
               &((struct sockaddr_in *) (multicastAddr->ai_addr))->sin_addr,
               sizeof(multicastRequest.imr_multiaddr));

        /* Accept multicast from any interface */
        multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);

        /* Join the multicast address */
        if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &multicastRequest, sizeof(multicastRequest)) !=
            0) {
            perror("setsockopt() failed");
            goto error;
        }
    }
    else if (multicastAddr->ai_family == PF_INET6 &&
             multicastAddr->ai_addrlen == sizeof(struct sockaddr_in6)) /* IPv6 */
    {
        struct ipv6_mreq multicastRequest;  /* Multicast address join structure */

        /* Specify the multicast group */
        memcpy(&multicastRequest.ipv6mr_multiaddr,
               &((struct sockaddr_in6 *) (multicastAddr->ai_addr))->sin6_addr,
               sizeof(multicastRequest.ipv6mr_multiaddr));

        /* Accept multicast from any interface */
        multicastRequest.ipv6mr_interface = 0;

        /* Join the multicast address */
        if (setsockopt(sock, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, (char *) &multicastRequest, sizeof(multicastRequest)) !=
            0) {
            perror("setsockopt() failed");
            goto error;
        }
    }
    else {
        perror("Neither IPv4 or IPv6");
        goto error;
    }


    if (localAddr)
        freeaddrinfo(localAddr);
    if (multicastAddr)
        freeaddrinfo(multicastAddr);

    return sock;

    error:
    if (localAddr)
        freeaddrinfo(localAddr);
    if (multicastAddr)
        freeaddrinfo(multicastAddr);

    return -1;
}
