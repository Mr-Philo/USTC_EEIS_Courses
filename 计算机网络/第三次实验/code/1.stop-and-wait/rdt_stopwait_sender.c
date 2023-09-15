#include "net.h"

/**
 * 发送文件
 * @param send_file_name - 待发送的文件名
 * @param sockfd - 发送数据的socket
 */
int deliver_file(char *send_file_name, int sockfd) {
    // 从此文件读取数据，包装成RDT数据包
    FILE *fp = NULL;
    if ((fp = fopen(send_file_name, "r")) == NULL) {
        printf("open file : %s failed.\n", send_file_name);
        return 1;
    }

    // 总共发送的字节数
    int total_send_byte = 0;

    // 当前发送端需要发送的数据包序列号
    int seq_num = RDT_BEGIN_SEQ;

    // 发送RDT数据包，直到文件结束
    int counter = 1;
    while (1) {
        char rdt_data[RDT_DATA_LEN];
        int flag, data_len;
        if (feof(fp)) {
            // 如果已经读到发送文件的结尾，则设置数据包类型为RDT_CTRL_END
            flag = RDT_CTRL_END;
            data_len = 0;
        } else {
            // 设置数据包类型为RDT_CTRL_DATA
            flag = RDT_CTRL_DATA;
            data_len = fread(rdt_data, sizeof(char), RDT_DATA_LEN, fp);
        }

        // step 1. 封装RDT数据包
        char rdt_pkt[RDT_PKT_LEN];
        int pkt_len = pack_rdt_pkt(rdt_data, rdt_pkt, data_len, seq_num, flag);    //* TODO1 根据参数依次配置即可

        // step 2. 发送RDT数据包，重传直到收到ACK
        while (1) {
            // step 2-1. 调用不可靠数据传输发送新的RDT数据包
            printf("[MySender]Packet #%d: %d bytes. Send count #%d\n", seq_num, pkt_len, counter++);
            udt_send(sockfd, rdt_pkt, pkt_len, 0);   //*TODO 2-1 根据参数依次配置即可
            //20:10  这里的flag不是RDT数据包的flag！！！！！！rnm!!!

            // step 2-2. 一直等待到文件描述符集合中某个文件有可读数据，或者到达超时时限： poll()
            struct pollfd pollfd = {sockfd, POLLIN};
            /* TODO 2-2*/
            int ff = poll(&pollfd, 1, 10);    //依据ff数值来判断接收端的接受情况，设置最大等待时间为10ms
            //fds： 要监视的文件描述符集,为一结构体变量； 
            //nfds：nfds_t 类型的字段，指定 fds 数组的长度，用于标记数组fds中的结构体元素的总数量 
            //timeout：指定 poll() 函数等待的最长时间，单位为毫秒。-1 表示永远阻塞等待。0 表示立即返回，不阻塞
            if(ff>0){     //收到ACK了,但不一定是我们需要的ACK（第二个大BUG！！！！！！！我吐了！！！！！！！！！！！！！！！！）
                //接受并解封装ACK数据包
                char ack_pkt[RDT_PKT_LEN];
                int ack_pkt_len = recv(sockfd, ack_pkt, RDT_PKT_LEN, 0);

                char ack_data[RDT_DATA_LEN];
                int ack_seq_num,ack_flag; 
                int ack_data_len = unpack_rdt_pkt(ack_data, ack_pkt, ack_pkt_len, & ack_seq_num, & ack_flag);
                //检查ACK数据包中的序列号
                if (ack_seq_num==seq_num){      //若ACK数据包序列号与目前序列号一致，则接受成功
                    printf("[MySender]Get ACK packet successfully for packet %d, Send next\n", seq_num);
                    break;      //跳出这个等待ACK的while(1)循环
                }
            }
            else{       //没收到
                printf("Time Out! Resend ... ... \n");
            }
        }

        // step 3. 发送成功，更新seq_num和total_send_byte
        /* TODO 3*/
        seq_num ++;
        total_send_byte += data_len;

        if (flag == RDT_CTRL_END) {
            break;
        }
    }       //发送整个文件的while(1)结束

    printf("\n\n");
    printf("Send file %s finished.\n", send_file_name);
    printf("Total send %5d bytes.\n", total_send_byte);

    fclose(fp);
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("wrong argument!\n");
        printf("usage: %s send_file_name. \n", argv[0]);
        exit(0);
    }

    int sockfd = init_socket_sender();
    if (deliver_file(argv[1], sockfd) != 0) {
        printf("deliver file %s failed.\n", argv[1]);
        close(sockfd);
        exit(1);
    }

    close(sockfd);
    return 0;
}
