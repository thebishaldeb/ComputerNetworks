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

    /* build socket address data structure */
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(7076);

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

    printf("\nConnected to the server 127.0.0.1 at TCP port 7076 \n");

    printf("\nSending Hello to the server\n");

    char *hello = "Hello";
    send(sd, hello, strlen(hello) + 1, 0);

    if (len = read(sd, buf, sizeof(buf)))
    {
        printf("\nConnection count received from server: %s.\n", buf);
    }
    printf("\nTask Accompolished!!\n");
}
