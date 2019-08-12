/* TCPsERver.c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> /* close */
#include <string.h>

#define MAX_CLIENT 5
#define MAX_MSG 100

int main(int argc, char *argv[])
{

    int sd, newSd, cliLen, len;

    struct sockaddr_in cliAddr, servAddr;
    char buf[MAX_MSG];
    int optval = 1;
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <server IP>  <Server Port>\n", argv[0]);
        exit(1);
    }

    /* build socket address data structure */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    /* servAddr.sin_addr.s_addr = htonl(INADDR_ANY); */
    servAddr.sin_port = htons(atoi(argv[2]));

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

        printf("%s :: waiting for connection request on TCP port %u\n", argv[0], atoi(argv[2]));

        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, &cliLen);
        if (newSd < 0)
        {
            perror("cannot accept connection ");
            exit(1);
        }

        printf("\n Connection request from a Client is accepted\n");
        /* init buf */
        memset(buf, 0x0, MAX_MSG);
        FILE *fp;

        if (len = recv(newSd, buf, sizeof(buf), 0))
        {
            fp = fopen(buf, "w");
            if (!fp)
            {
                printf("Can't open %s!", buf);
                exit(1);
            }
            printf("\nTransferring %s from client to server\n", buf);
            printf("\nCopying one line at a time from client to server\n");
            /* receive segments */
            int i;
            while (len = recv(newSd, buf, sizeof(buf), 0))
            {
                i = 0;
                while (len > 0)
                {
                    fprintf(fp, "%c", buf[i]);
                    i++;
                    len--;
                }
            }
        }
        printf("\nData recieved.\n");
        fclose(fp);
        close(newSd);
        printf("\nClosing current connection with Client\n\n");
    }
}
