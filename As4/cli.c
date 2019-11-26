#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int max(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        fprintf(stderr, "Port no. not provided\n");
    }

    int sockfd, usockfd, newsockfd, n, portno, i, j, p, k, maxfd, result;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[255], revansw[255];
    fd_set rset;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Binding error takes place");
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    //udp socket
    usockfd = socket(AF_INET, SOCK_DGRAM, 0);
    //for udp socket binding
    if (bind(usockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Binding error takes place");
    }
    FD_ZERO(&rset); //descriptor set to be cleared
    maxfd = max(sockfd, usockfd) + 1;
    while (1)
    {
        FD_SET(sockfd, &rset);
        FD_SET(usockfd, &rset); //setting the descriptors in readset

        result = select(maxfd, &rset, NULL, NULL, NULL);

        //if TCP socket is readable then

        if (FD_ISSET(sockfd, &rset))
        {
            newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
            p = fork();

            if (p == 0)
            {
                close(sockfd);

                printf("connection established\n");
                while (1)
                {
                    bzero(buffer, 255);
                    n = read(newsockfd, buffer, 255);
                    if (n < 0)
                        error("error in reading\n");

                    printf("client:%s\n", buffer);
                    for (i = strlen(buffer) - 1, j = 0; i >= 0; i--, j++)
                    {
                        revansw[j] = buffer[i];
                    }
                    revansw[j] = '\0';

                    n = write(newsockfd, revansw, 255);
                    if (n < 0)
                        error("error in writing\n");
                }
            }
            close(newsockfd);
        }
        if (FD_ISSET(usockfd, &rset))
        {
            int length = sizeof(serv_addr);
            while (1)
            {
                bzero(buffer, sizeof(buffer));
                n = recvfrom(usockfd, buffer, 255, 0, (struct sockaddr *)&cli_addr, &clilen);
                if (n < 0)
                    error("recvfrom");

                printf("connection established\n");

                printf("client:%s\n", buffer);
                for (i = strlen(buffer) - 1, j = 0; i >= 0; i--, j++)
                {
                    revansw[j] = buffer[i];
                }
                revansw[j] = '\0';

                n = sendto(usockfd, revansw, strlen(revansw), 0, (struct sockaddr *)&cli_addr, length);

                if (n < 0)
                {
                    error("sendto");
                }
            }
        }
    }
}
