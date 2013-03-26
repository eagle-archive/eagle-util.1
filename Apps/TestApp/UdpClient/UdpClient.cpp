
/* Sample UDP client */

#include <boost/thread.hpp>
#include <stdio.h>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#define SERVER_IP_ADDR      "221.226.4.246"
#define CLIENT_IP_ADDR      "117.60.58.65"

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
    struct sockaddr_in server_addr, sa;
    char sendline[1000];
    long n = 1;

#ifdef _WIN32
    LoadSocketLib();
#endif
    printf("UdpClient started.\n");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Bind the socket to a specified IP so that it only send the package via this network card
    // Does this work on Linux? If not, try setsockopt() with SO_BINDTODEVICE parameter.
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(CLIENT_IP_ADDR);
    sa.sin_port = 0; // Just specify the IP address, and leave the port open (i.e. 0), so that the system can still chose one.
    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) == -1)
    {
        printf("Bind to Port Number %d ,IP address %s failed\n", PORT, CLIENT_IP_ADDR);
        closesocket(sockfd);
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server_addr.sin_port = htons(PORT);

    while (true)
    {
        sprintf(sendline, "This is a testing UDP package. Package #: %ld", n);
        int sent = sendto(sockfd, sendline, strlen(sendline) ,0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent < 0) {
            printf("send failed\n");
            closesocket(sockfd);
            exit(2);
        } else {
            printf("Package #%ld sent out.\n", n);
        }
        SleepMs(800);
        n++;
    }
}
