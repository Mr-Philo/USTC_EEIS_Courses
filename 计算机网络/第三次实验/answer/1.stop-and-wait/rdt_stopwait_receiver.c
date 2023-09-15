#include "net.h"

/**
 * 接收端接收函数
 * @param save_file_name - 保存文件名
 * @param sockfd - 接收数据的socket
 */
int receive_file(char *save_file_name, int sockfd) {
    // 将收到的RDT数据包按顺序写到此文件中
    FILE *fp;
    if ((fp = fopen(save_file_name, "w")) == NULL) {
        printf("open file : %s failed.\n", save_file_name);
        return 1;
    }

    // 总共接收的字节数
    int total_recv_byte = 0;

    // 当前接收端需要的数据包序列号
    int exp_seq_num = RDT_BEGIN_SEQ;

    //接收RDT数据包，直到所有数据全部接收完毕
    while (1) {
        // step 1. 接收RDT数据包
        char rdt_pkt[RDT_PKT_LEN];
        int pkt_len = recv(sockfd, rdt_pkt, RDT_PKT_LEN, 0);

        // step 2. 解封装RDT数据包
        char rdt_data[RDT_DATA_LEN];
        int seq_num, flag;
        int data_len = unpack_rdt_pkt(rdt_data, rdt_pkt, pkt_len, &seq_num, &flag);

        // step 3. 检查此数据包是否为期待的数据包 : seq_num==exp_seq_num
        if (seq_num < exp_seq_num) {
            printf("[Receiver]Packet #%d received, send ACK, already received.\n", seq_num);
        } else if (seq_num == exp_seq_num) {
            fwrite(rdt_data, sizeof(char), data_len, fp);
            total_recv_byte += data_len;
            exp_seq_num++;
            printf("[Receiver]Packet #%d received, send ACK, now exp_num is %d\n", seq_num, exp_seq_num);
        } else {
            continue;
        }

        // step 4. 封装一个新的RDT数据包(ACK包)
        char reply_rdt_pkt[RDT_PKT_LEN];
        int reply_pkt_len = pack_rdt_pkt(NULL, reply_rdt_pkt, 0, seq_num, RDT_CTRL_ACK);

        // step 5. 调用不可靠数据传输发送新的RDT数据包(ACK包)
        udt_send(sockfd, reply_rdt_pkt, reply_pkt_len, 0);

        if (flag == RDT_CTRL_END) {
            break;
        }
    }

    printf("\n\n");
    printf("Receive finished. Write to file %s.\n", save_file_name);
    printf("Total recv %d byte\n", total_recv_byte);

    fflush(fp);
    fclose(fp);
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("wrong argument!\n");
        printf("usage: %s save_file\n", argv[0]);
        exit(0);
    }

    int sockfd = init_socket_receiver();
    if (receive_file(argv[1], sockfd) != 0) {
        printf("receive file %s failed.\n", argv[1]);
        close(sockfd);
        exit(1);
    }

    close(sockfd);
    return 0;
}
