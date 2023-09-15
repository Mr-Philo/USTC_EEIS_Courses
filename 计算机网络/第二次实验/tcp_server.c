#include "net.h"

int main() {

    /* 建立服务端套接字 */
    int server_sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int enable = 1;
    Setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    /* 绑定端口 */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(TCP_SERVER_ADDRESS);
    server_addr.sin_port = htons(TCP_SERVER_PORT);

    Bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    /* 监听端口 */
    Listen(server_sockfd, CONNECTION_NUMBER);

    /* 建立tcp连接 */
    struct sockaddr_in client_addr;
    unsigned int client_addr_len = sizeof(struct sockaddr_in);
    int client_sockfd = Accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    printf("Accept client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    /* 接收数据 */
    char buf[TCP_BUF_LENGTH];
    size_t pkt_len;
    while(1){
	pkt_len = Recv(client_sockfd, buf, TCP_BUF_LENGTH, 0);    
	if (pkt_len > 0) {
        	buf[pkt_len] = '\0';
        	printf("Received %zu bytes\n", pkt_len);
		//server send data to client:
		Send(client_sockfd, buf, strlen(buf), 0);
		//pkt_len=0;
    	} else {
		printf("closed\n");
		break;        	

    	}

    }

    /* 关闭套接字 */
    close(client_sockfd);
    close(server_sockfd);

    return 0;
}
