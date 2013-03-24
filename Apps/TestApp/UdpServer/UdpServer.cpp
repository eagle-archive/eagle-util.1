// UdpServer.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#define PORT        40001

#ifdef _WIN32
bool LoadSocketLib()
{
    WSADATA wsaData;
    int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (NO_ERROR != nResult)
    {
        printf("Error: failed to init Winsock!\n");
        return false;
    }

    return true;
}
#endif

int main()
{
#ifdef _WIN32
    LoadSocketLib();
#endif

    SOCKET sockfd;
    int n = 1;
    struct sockaddr_in servaddr,cliaddr;
    int len;
    char mesg[1000];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    for (;;)
    {
        len = sizeof(cliaddr);
        recvfrom(sockfd,mesg, 1000, 0, (struct sockaddr *)&cliaddr, &len);
        printf("Received %ld packages.\n", n++);
    }
    return 0;
}
