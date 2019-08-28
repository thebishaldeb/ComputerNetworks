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

    int sd, newSd, cliLen, len;
    pid_t child;

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

    int i = 1;
    while (i == 1)
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

        child = fork();

        if (child < 0)
        {
            perror("Fork Error");
            exit(1);
        }
        if (child == 0)
        {
            printf("\n--------------------------------------\n");
            printf("\nThis is under the child process!\n");
            printf("\nClosing listen socket\n");
            close(sd);
            if (len = read(newSd, buf, sizeof(buf)))
            {
                printf("\nString received from client: %s.\n", buf);
                int i;

                char temp;
                for (i = 0; i <= (strlen(buf) - 1) / 2; i++)
                {
                    temp = buf[i];
                    buf[i] = buf[strlen(buf) - i - 1];
                    buf[strlen(buf) - i - 1] = temp;
                }
                printf("\nSending modified String to the client: %s\n", buf);
                send(newSd, buf, sizeof(buf), 0);
                printf("\nTask Accompolished!!\n");
            }
            close(newSd);
            printf("\nClosing current connection with Client\n");
            i = 0;
            printf("\n--------------------------------------\n");
            exit(0);
        }
        else
        {
            printf("\nThis is under the parent process!\n");
            close(newSd);
            printf("\nClosing current connection with Client\n");
            printf("\n--------------------------------------\n\n");
        }
    }
}
