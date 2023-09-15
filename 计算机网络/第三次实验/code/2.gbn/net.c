#include "net.h"

#include <errno.h>
#include <string.h>
#include <time.h>

int pack_rdt_pkt(char *data_buf, char *rdt_pkt, int data_len, int seq_num, int flag) {
    char *ptr = rdt_pkt;
    uint32_t ctrl_net_order = htonl(flag);
    uint32_t seq_net_order = htonl(seq_num);

    memcpy(ptr, &ctrl_net_order, sizeof(uint32_t));
    ptr += sizeof(uint32_t);
    memcpy(ptr, &seq_net_order, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    if (data_len > 0 && data_buf != NULL)
        memcpy(ptr, data_buf, data_len);
    return (RDT_HEADER_LEN + data_len);
}

int unpack_rdt_pkt(char *data_buf, char *rdt_pkt, int pkt_len, int *seq_num, int *flag) {
    char *ptr = rdt_pkt;
    uint32_t ctrl_net_order, seq_net_order;
    int data_len;

    memcpy(&ctrl_net_order, ptr, sizeof(uint32_t));
    ptr += sizeof(uint32_t);
    *flag = ntohl(ctrl_net_order);

    memcpy(&seq_net_order, ptr, sizeof(uint32_t));
    ptr += sizeof(uint32_t);
    *seq_num = ntohl(seq_net_order);

    data_len = pkt_len - RDT_HEADER_LEN;
    if (data_buf != NULL && data_len > 0)
        memcpy(data_buf, ptr, data_len);
    return data_len;
}

void rdt_send(int sockfd, char *pkt, int pkt_len, int flags) {
    if ((rand() % 100) >= RDT_PKT_LOSS_RATE)
        send(sockfd, pkt, pkt_len, flags);
    else
        printf(" packet lost!\n");
}

int init_socket_sender() {
    srand(time(NULL));

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in send_addr = {0};
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    send_addr.sin_port = htons(RDT_SEND_PORT);
    if (bind(sockfd, (struct sockaddr *)&send_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    struct sockaddr_in recv_addr = {0};
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = inet_addr(RDT_SERVER_ADDRESS);
    recv_addr.sin_port = htons(RDT_RECV_PORT);
    if (connect(sockfd, (struct sockaddr *)&recv_addr, sizeof(struct sockaddr)) < 0) {
        perror("connect error");
        exit(1);
    }
    return sockfd;
}

int init_socket_receiver() {
    srand(time(NULL));

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in recv_addr = {0};
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = INADDR_ANY;
    recv_addr.sin_port = htons(RDT_RECV_PORT);
    if (bind(sockfd, (struct sockaddr *)&recv_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    struct sockaddr_in send_addr = {0};
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = inet_addr(RDT_SERVER_ADDRESS);
    send_addr.sin_port = htons(RDT_SEND_PORT);
    if (connect(sockfd, (struct sockaddr *)&send_addr, sizeof(struct sockaddr)) < 0) {
        perror("connect error");
        exit(1);
    }
    return sockfd;
}