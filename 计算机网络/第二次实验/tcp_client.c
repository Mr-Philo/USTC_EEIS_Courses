#include "net.h"

int main() {

    /* 建立套接字 */
    int sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* 建立tcp连接 */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(TCP_SERVER_ADDRESS);
    server_addr.sin_port = htons(TCP_SERVER_PORT);

    Connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    /* 发送数据 */
    char msg[TCP_BUF_LENGTH];		//store send message
    char buf[TCP_BUF_LENGTH];		//store receive message
    size_t pkt_len;
    
    while(1){	//continous receiving, press ctrl+C to quit
        if(gets(msg)==NULL){	
		break;
	}
    	Send(sockfd, msg, strlen(msg), 0);

    	/*client receive data from server*/
    	pkt_len = Recv(sockfd, buf, TCP_BUF_LENGTH, 0);
    	if (pkt_len > 0) {
        	buf[pkt_len] = '\0';
        	printf("%s\n", buf);
    	} else {
        	printf("Connection closed\n");
    	}
    }

    /* 关闭套接字 */
    close(sockfd);

    return 0;
}
