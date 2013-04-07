// UdpServer.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <memory.h>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <unistd.h>
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
    if (false == LoadSocketLib()) {
        return 1;
    }
#endif
    printf("UdpServer started.\n");

    int sockfd;
    int n = 1;
    struct sockaddr_in servaddr, cliaddr;
    static char mesg[1024 * 10];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if (-1 == bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))) {
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
        perror("Error: faild to call bind");
        return 2;
    }

    for (;;)
    {
#ifdef _WIN32
        int len;
#else
        socklen_t len;
#endif
        len = sizeof(cliaddr);
        int size = recvfrom(sockfd, mesg, sizeof(mesg), 0, (struct sockaddr *)&cliaddr, &len);
        if (size > 0) {
#ifdef _WIN32
            printf("Received %ld packages from %d.%d.%d.%d:%d, size:%d\n", n++,
                (int)cliaddr.sin_addr.S_un.S_un_b.s_b1, (int)cliaddr.sin_addr.S_un.S_un_b.s_b2,
                (int)cliaddr.sin_addr.S_un.S_un_b.s_b3, (int)cliaddr.sin_addr.S_un.S_un_b.s_b4,
                (int)cliaddr.sin_port,
                size);
#else
            printf("Received %ld packages, port:%d, size:%d\n", n++, (int)cliaddr.sin_port, size);
#endif

#if 1
            {
                char fname[512];
                sprintf(fname, "capture//pack-%06d.dat", n);
                FILE *fp = fopen(fname, "wb");
                if (fp) {
                    fwrite(mesg, 1, size, fp);
                    fclose(fp);
                }
            }
#endif
        } else {
            printf("recvfrom() timeout\n");
        }
    }
    return 0;
}
