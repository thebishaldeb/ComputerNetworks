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
    int sd, rc, len;
    struct sockaddr_in localAddr, servAddr;
    char buf[MAX_MSG];
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s <server IP>  <Server Port> <String> \n", argv[0]);
        exit(1);
    }

    /* build socket address data structure */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    /* create socket, active open */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("cannot open socket ");
        exit(1);
    }

    /* connect to server */
    rc = connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));

    if (rc < 0)
    {
        perror("cannot connect ");
        exit(1);
    }

    printf("\nConnected to the server %s at TCP port %s \n", argv[1], argv[2]);

    printf("\nSending string to the server: %s\n", argv[3]);

    send(sd, argv[3], strlen(argv[3]) + 1, 0);

    if (len = read(sd, buf, sizeof(buf)))
    {
        printf("\nModified String received from server: %s.\n", buf);
    }
    printf("\nTask Accompolished!!\n");
}
