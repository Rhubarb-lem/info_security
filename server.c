#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include "libs/myauth.h"
#include <string.h>
#define T 7

#define SERVER_PORT 12345
#define DATABASE_FILE "auth_data.txt"

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

void save_to_file(const char *name, uint64_t N, uint64_t v)
{
    FILE *file = fopen(DATABASE_FILE, "a");
    if (!file)
    {
        perror("Error opening database file");
        exit(1);
    }
    fprintf(file, "%s %llu %llu\n", name, N, v);
    fclose(file);
}

int load_from_file(const char *name, uint64_t *N, uint64_t *v)
{
    FILE *file = fopen(DATABASE_FILE, "r");
    if (!file)
    {
        perror("Error opening database file");
        return 0; 
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file))
    {
        char stored_name[10];
        uint64_t stored_N, stored_v;
        if (sscanf(buffer, "%s %llu %llu", stored_name, &stored_N, &stored_v) == 3)
        {
            if (strcmp(stored_name, name) == 0)
            {
                *N = stored_N;
                *v = stored_v;
                fclose(file);
                return 1; 
            }
        }
    }

    fclose(file);
    return 0; 
}

int main()
{
    srand(time(NULL));
    int sockMain, newSock, length;
    struct sockaddr_in servAddr, clientAddr;

    if ((sockMain = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);

    if (bind(sockMain, (const struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Error binding");
        exit(1);
    }

    if (listen(sockMain, 5) < 0)
    {
        perror("Error listening");
        exit(1);
    }

    printf("Waiting for client...\n");
    length = sizeof(clientAddr);

    if ((newSock = accept(sockMain, (struct sockaddr *)&clientAddr, &length)) < 0)
    {
        perror("Error accepting connection");
        exit(1);
    }

    int net_opt;
    if (recv(newSock, &net_opt, sizeof(net_opt), 0) < 0)
    {
        perror("Error receiving option");
        exit(1);
    }
   

    char name[10] = {' '};
    recv(newSock, name, 10, 0);
    printf("Received name: %s\n", name);

    int option = ntohl(net_opt);
    //printf("Option (host order): %d\n", option);

    if (option == 1)
    {
        uint64_t N = server_gen_module();
        //printf("Generated N: %llu\n", N);

        uint64_t net_N = htonll(N);
        if (send(newSock, &net_N, sizeof(net_N), 0) < 0)
        {
            perror("Error sending N");
            exit(1);
        }
        //printf("Sent N (network order): %llu\n", net_N);

        uint64_t net_v;
        if (recv(newSock, &net_v, sizeof(net_v), 0) < 0)
        {
            perror("Error receiving v");
            exit(1);
        }
     

        uint64_t v = ntohll(net_v);
        //printf("Received v (host order): %llu\n", v);

        save_to_file(name, N, v); 
    }
    else
    {
        uint64_t N, v;
        if (!load_from_file(name, &N, &v))
        {
            printf("Client '%s' not found in database!\n", name);
            close(newSock);
            close(sockMain);
            exit(1);
        }
        printf("Loaded N: %llu, v: %llu for client %s\n", N, v, name);

        uint64_t net_N = htonll(N);
        if (send(newSock, &net_N, sizeof(net_N), 0) < 0)
        {
            perror("Error sending N");
            exit(1);
        }
   

        for (int round = 1; round <= T; round++)
        {
            uint64_t net_x;
            if (recv(newSock, &net_x, sizeof(net_x), 0) < 0)
            {
                perror("Error receiving x");
                exit(1);
            }
           

            uint64_t x = ntohll(net_x);
            printf("Round %d: x (host order): %llu\n", round, x);

            int e = server_gen_rand_bit();
            printf("Round %d: Generated e: %d\n", round, e);

            if (send(newSock, &e, sizeof(e), 0) < 0)
            {
                perror("Error sending e");
                exit(1);
            }

            uint64_t net_y;
            if (recv(newSock, &net_y, sizeof(net_y), 0) < 0)
            {
                perror("Error receiving y");
                exit(1);
            }
           // printf("Round %d: Received y (network order): %llu\n", round, net_y);

            uint64_t y = ntohll(net_y);
            //printf("Round %d: y (host order): %llu\n", round, y);

            uint64_t yy = y * y % N;
            uint64_t xv = (e == 0) ? x % N : (x % N * v % N) % N;
            //printf("Round %d: yy: %llu, xv: %llu\n", round, yy, xv);

            if (yy != xv)
            {
                printf("Authentication failed on round %d!\n", round);
                close(newSock);
                close(sockMain);
                exit(1);
            }
            //sleep(10);
        }
        
    }

    printf("Authentication successful!\n");
    close(newSock);
    close(sockMain);
    return 0;
}
