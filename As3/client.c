#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h> /* close */
#include <string.h>
#define MAX_MSG 100
int main(int argc, char *argv[])
{
    int sd, len;
    struct sockaddr_in servAddr;

    char buf[MAX_MSG];

    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s <server IP>  <Server Port> <String> \n", argv[0]);
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("cannot open socket ");
        exit(1);
    }

    printf("\nSending string to the server: %s\n", argv[3]);

    int addr_len = sizeof(struct sockaddr);

    len = sendto(sd, argv[3], strlen(argv[3]) + 1, MSG_CONFIRM, (struct sockaddr *)&servAddr, addr_len);
    if (len < 0)
    {
        perror("Couldn't send the string");
        exit(1);
    }

    if (len = recvfrom(sd, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *)&servAddr, &addr_len))
    {
        printf("\nModified String received from server: %s.\n", buf);
    }

    printf("\nTask Accompolished!!\n");
    close(sd);
}
