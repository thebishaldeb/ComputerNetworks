/* TCPsERver.c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> /* close */
#include <string.h>

#define MAX_CLIENT 5
#define MAX_MSG 100

int main(int argc, char *argv[])
{
    int count = 0;

    int sd, newSd, cliLen, len;

    struct sockaddr_in cliAddr, servAddr;
    char buf[MAX_MSG];
    int optval = 1;

    /* build socket address data structure */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(7076);

    /* create socket for passive open */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("cannot open socket ");
        exit(1);
    }
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

    if (bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("cannot bind port ");
        exit(1);
    }

    listen(sd, MAX_CLIENT);

    while (1)
    {

        printf("%s :: waiting for connection request on TCP port 7076\n", argv[0]);
        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, &cliLen);
        if (newSd < 0)
        {
            perror("cannot accept connection ");
            exit(1);
        }
        printf("\n Connection request from a Client is accepted\n");
        printf("Incrementing the count\n");
        ++count;
        memset(buf, 0x0, MAX_MSG);
        if (len = read(newSd, buf, sizeof(buf)))
        {
            printf("\n%s received from client.\n", buf);
            sprintf(buf, "%d", count);
            printf("\nSending no of connections to the client: %s\n", buf);
            send(newSd, buf, sizeof(buf), 0);
            printf("\nTask Accompolished!!\n");
        }
        close(newSd);
        printf("\nClosing current connection with Client\n");
        printf("\n--------------------------------------\n\n");
    }
}
