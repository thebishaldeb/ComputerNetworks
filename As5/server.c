#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define BUFFER_SIZE 14
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void errorCorrection(char sigMsg[])
{
    int incorrectbit = 0;

    if (((int)(sigMsg[1] - '0') + (int)(sigMsg[3] - '0') + (int)(sigMsg[5] - '0') + (int)(sigMsg[7] - '0') + (int)(sigMsg[9] - '0') + (int)(sigMsg[11] - '0')) % 2 != 0)
        incorrectbit += 1; //ie 2^0

    if (((int)(sigMsg[2] - '0') + (int)(sigMsg[3] - '0') + (int)(sigMsg[6] - '0') + (int)(sigMsg[7] - '0') + (int)(sigMsg[10] - '0') + (int)(sigMsg[11] - '0')) % 2 != 0)
        incorrectbit += 2; //ie 2^1
    if (((int)(sigMsg[4] - '0') + (int)(sigMsg[5] - '0') + (int)(sigMsg[6] - '0') + (int)(sigMsg[7] - '0') + (int)(sigMsg[12] - '0')) % 2 != 0)
        incorrectbit += 4; //ie 2^2
    if (((int)(sigMsg[8] - '0') + (int)(sigMsg[9] - '0') + (int)(sigMsg[10] - '0') + (int)(sigMsg[11] - '0') + (int)(sigMsg[12] - '0')) % 2 != 0)
        incorrectbit += 8; // ie 2^3
    printf("The incorrect bit was:%d\nTherefore ", incorrectbit);
    if (incorrectbit == 0)
        printf("No errorn\n");
    else
    {
        printf("Error is present\nCorrecting it\n");
        if (sigMsg[incorrectbit] == '0')
            sigMsg[incorrectbit] = '1';
        else
            sigMsg[incorrectbit] = '0';
        printf("Corrected msg:%s\n", sigMsg + 1);
    }
}
int main(int argc, char *argv[])
{

    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error Opening Socket");
    server = gethostbyname("127.0.0.1");
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host ");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    //serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port = htons(9054);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection Failed");
    n = read(sockfd, buffer, BUFFER_SIZE);
    if (n < 0)
        error("Error on reading");
    printf("Server: %s\n", buffer + 1);
    close(sockfd);
    // printf("\n\n%c",buffer[13]);
    errorCorrection(buffer);
    return 0;
}