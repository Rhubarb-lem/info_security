#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include "libs/myauth.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFLEN 10
#define T 7



uint64_t htonll(uint64_t value)
{
    if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    {
        return value;
    }
    else
    {
        return ((uint64_t)htonl(value & 0xFFFFFFFF) << 32) | htonl(value >> 32);
    }
}

uint64_t ntohll(uint64_t value)
{
    if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    {
        return value;
    }
    else
    {
        return ((uint64_t)ntohl(value & 0xFFFFFFFF) << 32) | ntohl(value >> 32);
    }
}

void clear_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main()
{
    srand(time(NULL));
    int sock;
    struct sockaddr_in servAddr;
    char buf[BUFLEN];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servAddr.sin_port = htons(SERVER_PORT);

    if (connect(sock, (const struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Error connecting to server");
        exit(1);
    }

    int option;
    while (1)
    {
        printf("Select an option:\n 1) Register\n 2) Pass authentication\n");
        scanf("%d", &option);
        clear_stdin(); // Очистка буфера после ввода
        if ((option == 1) || (option == 2))
            break;
        printf("Invalid option\n");
    }

    int net_opt = htonl(option);


    if (send(sock, &net_opt, sizeof(net_opt), 0) < 0)
    {
        perror("Error sending number opt");
        exit(1);
    }

    char name[10];
    printf("Enter login: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; 
    printf("Login: %s\n", name);

    if (send(sock, name, strlen(name), 0) < 0)
    {
        perror("Error sending name");
        exit(1);
    }

    uint64_t received_N;
    if (recv(sock, &received_N, sizeof(received_N), 0) < 0)
    {
        perror("Error receiving number N");
        exit(1);
    }
   

    uint64_t N = ntohll(received_N);
    //printf("N (host order): %llu\n", N);

    if (option == 1)
    {
        uint64_t s, v;
        client_gen_keys(N, &s, &v);
        printf("Generated keys: s = %llu, v = %llu\n", s, v);

        uint64_t net_V = htonll(v);
   

        if (send(sock, &net_V, sizeof(net_V), 0) < 0)
        {
            perror("Error sending v");
            exit(1);
        }
    }
    else
    {
        long int s;
        printf("Enter secret key: ");
        scanf("%ld", &s);
        clear_stdin(); // 
        //printf("Using secret key: %ld\n", s);

        for (int round = 1; round <= T; round++)
        {
            long r;
            uint64_t x = client_proof_rand_x(N, &r);
            //printf("Round %d: Generated x = %llu (r = %ld)\n", round, x, r);

            uint64_t net_x = htonll(x);
            

            if (send(sock, &net_x, sizeof(net_x), 0) < 0)
            {
                perror("Error sending x");
                exit(1);
            }

            int e;
            if (recv(sock, &e, sizeof(e), 0) < 0)
            {
                perror("Error receiving e");
                exit(1);
            }
            //printf("Round %d: Received e: %d\n", round, e);

            uint64_t y = client_proof_rand_y(N, r, s, e);
            //printf("Round %d: Calculated y = %llu\n", round, y);

            uint64_t net_y = htonll(y);

            if (send(sock, &net_y, sizeof(net_y), 0) < 0)
            {
                perror("Error sending y");
                exit(1);
            }
            // sleep(10);
        }
        printf("Authentication successful!\n");
    }

    

    close(sock);
    return 0;
}
