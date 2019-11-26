#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define erProb 4
char sigMsg[14];

void error(const char *msg)
{
        perror(msg);
        exit(1);
}

void getInput(char sigMsg[])
{
        printf("Enter 8 bit msg signal :    ");
        char msg[13];
        scanf("%s", msg);
        int i = 0, ii;
        sigMsg[0] = '0';
        for (ii = 1; ii < 13; ii++)
        {
                if (ceil(log(ii) / log(2)) != floor(log(ii) / log(2)))
                        sigMsg[ii] = msg[i++];
                else
                        sigMsg[ii] = '0';
        }
        sigMsg[13] = '\0';
}
void fillParityBits(char sigMsg[])
{
        // filling up parity bits 0  1 2 3 4 5 6 7 8 9 10 11 12
        //r1
        sigMsg[1] = ((char)(sigMsg[3] - '0') ^ (int)(sigMsg[5] - '0') ^ (int)(sigMsg[7] - '0') ^ (int)(sigMsg[9] - '0') ^ (int)(sigMsg[11] - '0') + '0');
        //r2
        sigMsg[2] = ((char)(sigMsg[3] - '0') ^ (int)(sigMsg[6] - '0') ^ (int)(sigMsg[7] - '0') ^ (int)(sigMsg[10] - '0') ^ (int)(sigMsg[11] - '0') + '0');
        //r4
        sigMsg[4] = ((char)(sigMsg[12] - '0') ^ (int)(sigMsg[5] - '0') ^ (int)(sigMsg[6] - '0') ^ (int)(sigMsg[7] - '0') + '0');
        //r8
        sigMsg[8] = ((int)(sigMsg[9] - '0') ^ (int)(sigMsg[10] - '0') ^ (int)(sigMsg[11] - '0') ^ (int)(sigMsg[12] - '0') + '0');
        printf("MsgSignal:\t\t%s\n", sigMsg + 1);
}
void setError(char sigMsg[])
{
        int rNum = rand() % 10;
        if (rNum < erProb)
        {
                int ebit = rand() % 11 + 1;
                if (sigMsg[ebit] == '0')
                        sigMsg[ebit] = '1';
                else
                        sigMsg[ebit] = '0';
                printf("\nError is set\n");
        }
        else
                printf("\nNo Error was set\n");
}
void sendMsg(char sigMsg[])
{
        int TCPsockfd, newsockfd;
        struct sockaddr_in addr, rec_addr;
        socklen_t clilen;

        //address initialization
        bzero((char *)&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(9054);
        clilen = sizeof(rec_addr);

        //binding tcp socket
        TCPsockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (TCPsockfd < 0)
                error("ERROR opening TCP socket");
        if (bind(TCPsockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
                error("ERROR on binding");
        listen(TCPsockfd, 5);

        //
        newsockfd = accept(TCPsockfd, (struct sockaddr *)&rec_addr, &clilen);
        if (newsockfd < 0)
                error("ERROR on accept\n");
        int n = write(newsockfd, sigMsg, strlen(sigMsg));
        if (n < 0)
                error("ERROR writing to socket");
        printf("Sent:   \t   \t%s\n", sigMsg + 1);
        close(newsockfd);
        close(TCPsockfd);
}
int main()
{
        char sigMsg[14];
        getInput(sigMsg);
        fillParityBits(sigMsg);
        setError(sigMsg);
        sendMsg(sigMsg);
        return 0;
}
