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

#define MAX_MSG 100

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <server IP>  <Server Port>\n", argv[0]);
        exit(1);
    }

    int sd, newSd, cliLen, len;
    char buf[MAX_MSG];

    struct sockaddr_in cliAddr, servAddr;
    bzero((char *)&servAddr, sizeof(servAddr));
    bzero((char *)&cliAddr, sizeof(cliAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("cannot open socket ");
        exit(1);
    }

    if (bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("cannot bind port ");
        exit(1);
    }

    int addr_len = sizeof(struct sockaddr);
    while (1)
    {
        printf("%s :: waiting for data on port %u\n", argv[0], atoi(argv[2]));

        cliLen = sizeof(cliAddr);
        memset(buf, 0x0, MAX_MSG);

        if (len = recvfrom(sd, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *)&cliAddr, &addr_len))
        {
            printf("\n--------------------------------------\n");
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

            len = sendto(sd, buf, strlen(buf) + 1, MSG_CONFIRM, (struct sockaddr *)&cliAddr, addr_len);
            if (len < 0)
            {
                perror("Couldn't send the string");
                exit(1);
            }
            printf("\nTask Accompolished!!\n");
        }
        printf("\n--------------------------------------\n");
    }
}
