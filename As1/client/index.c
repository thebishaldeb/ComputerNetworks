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
        fprintf(stderr, "Usage: %s <server IP>  <Server Port> <FileName> \n", argv[0]);
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

    printf("\n Connected to the server %s at TCP port %s \n", argv[1], argv[2]);

    FILE *fp = fopen(argv[3], "r");
    if (!fp)
    {
        printf("Can't open %s", argv[3]);
        exit(1);
    }
    printf("Transferring line by line of %s from client to server\n%ld\n", argv[3], strlen(argv[3]));

    /* main loop, get and send lines of text */
    send(sd, argv[3], strlen(argv[3]) + 1, 0);
    while (fgets(buf, sizeof(buf), fp))
    {
        buf[MAX_MSG - 1] = '\0';
        len = strlen(buf);
        printf("Sending: %s\n", buf);
        send(sd, buf, len, 0);
    }
    fclose(fp);
    printf("\nData sent.\n");
}
