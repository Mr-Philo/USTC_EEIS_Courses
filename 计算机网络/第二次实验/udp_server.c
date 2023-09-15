#include "net.h"

int main() {

    /* 建立套接字 */
    int sockfd = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int enable = 1;
    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    /* 绑定端口 */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(UDP_SERVER_ADDRESS);
    server_addr.sin_port = htons(UDP_SERVER_PORT);

    Bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    while(1){		//while is used for multipul receiving
    /* 接收数据 */
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    char buf[UDP_BUF_LENGTH];
    size_t pkt_len = Recvfrom(sockfd, buf, UDP_BUF_LENGTH, 0, (struct sockaddr *)&client_addr, &client_addr_len);

    buf[pkt_len] = '\0';
    //printf("Message received: %s[%zu bytes]\n", buf, pkt_len);
    printf("Message received from: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));		//print the client address and port
    }

    /* 关闭套接字 */
    close(sockfd);

    return 0;
}
