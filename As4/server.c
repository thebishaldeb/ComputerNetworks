#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAX_CLIENT 5
#define MAX_MSG_LEN 1000

void childProcess(int new_serv_sock)
{
    if (new_serv_sock < 0)
    {
        perror("Cannot accept connection");
        exit(1);
    }
    char buffer[MAX_MSG_LEN];
    memset(buffer, 0x0, MAX_MSG_LEN);
    recv(new_serv_sock, buffer, sizeof(buffer), 0);
    int i = 0;
    while (i < strlen(buffer) / 2)
    {
        char temp = buffer[i];
        buffer[i] = buffer[strlen(buffer) - i - 1];
        buffer[strlen(buffer) - i - 1] = temp;
        i++;
    }
    send(new_serv_sock, buffer, strlen(buffer) + 1, 0);
    close(new_serv_sock);
}

int main(int argc, char *argv[])
{
    int serv_sock_tcp, serv_sock_udp, new_serv_sock;
    int client_addr_len, recv_data_len;
    struct sockaddr_in client_addr, server_addr;
    char buffer[MAX_MSG_LEN];
    if (argc < 4)
    {
        printf("usage: %s <IP Address> <Port_TCP> <Port_UDP>", argv[0]);
        exit(1);
    }
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    serv_sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock_tcp < 0)
    {
        perror("Cannot open socket\n");
        exit(1);
    }
    if (bind(serv_sock_tcp, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Cannot Bind Port");
        exit(1);
    }
    server_addr.sin_port = htons(atoi(argv[3]));
    serv_sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (serv_sock_udp < 0)
    {
        perror("Cannot open socket\n");
        exit(1);
    }
    if (bind(serv_sock_udp, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Cannot Bind Port");
        exit(1);
    }
    listen(serv_sock_tcp, MAX_CLIENT);

    fd_set rfds;
    int max_val = serv_sock_tcp > serv_sock_udp ? serv_sock_tcp + 1 : serv_sock_udp + 1;
    while (1)
    {
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr_len = sizeof(client_addr);
        FD_ZERO(&rfds);
        FD_SET(serv_sock_tcp, &rfds);
        FD_SET(serv_sock_udp, &rfds);
        int ret_val = select(max_val, &rfds, NULL, NULL, NULL);
        if (ret_val == -1)
        {
            perror("Select() failed");
        }
        else if (FD_ISSET(serv_sock_tcp, &rfds))
        {
            new_serv_sock = accept(serv_sock_tcp, (struct sockaddr *)&client_addr, &client_addr_len);
            int pid;
            if ((pid = fork()) == 0)
            {
                close(serv_sock_tcp);
                childProcess(new_serv_sock);
                exit(0);
            }
            close(new_serv_sock);
        }
        else if (FD_ISSET(serv_sock_udp, &rfds))
        {
            memset(buffer, 0x0, MAX_MSG_LEN);
            recvfrom(serv_sock_udp, (char *)buffer, MAX_MSG_LEN, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_addr_len);
            int i = 0;
            while (i < strlen(buffer) / 2)
            {
                char temp = buffer[i];
                buffer[i] = buffer[strlen(buffer) - i - 1];
                buffer[strlen(buffer) - i - 1] = temp;
                i++;
            }
            sendto(serv_sock_udp, (const char *)buffer, strlen(buffer) + 1, MSG_CONFIRM, (struct sockaddr *)&client_addr, (socklen_t)client_addr_len);
        }
    }
}