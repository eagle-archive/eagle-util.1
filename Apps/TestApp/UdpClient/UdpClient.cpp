
/* Sample UDP client */

#include <boost/thread.hpp>
#include <stdio.h>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

//#define IP_ADDR     "221.226.4.245"
//#define IP_ADDR     "127.0.0.1"
#define IP_ADDR     "192.168.1.200"
#define PORT        40001

void SleepMs(long ms)
{
    boost::this_thread::sleep( boost::posix_time::milliseconds(ms) );
}

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
    SOCKET sockfd;
    struct sockaddr_in servaddr;
    char sendline[1000];
    long n = 1;

#ifdef _WIN32
    LoadSocketLib();
#endif
    printf("UdpClient started.\n");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDR);
    servaddr.sin_port = htons(PORT);

    while (true)
    {
        sprintf(sendline, "This is a testing UDP package. Package #: %ld", n);
        sendto(sockfd, sendline, strlen(sendline) ,0,
            (struct sockaddr *)&servaddr,sizeof(servaddr));
        printf("Package #%ld sent out.\n", n);

        SleepMs(800);
        n++;
    }
}
