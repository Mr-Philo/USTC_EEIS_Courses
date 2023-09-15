#include "net.h"

SendWindow send_window;  //滑动窗口数据结构体
int total_send_byte = 0; //记录累计发送的字节数

/* 用于监听端口并接收ACK包的线程函数 */
void *receive_acks(void *arg) {
    int sockfd = *(int *)arg;
    printf("\t[child thread] waiting acks..\n");

    while (1) {
        char rdt_pkt[RDT_PKT_LEN];
        int pkt_len = recv(sockfd, rdt_pkt, RDT_PKT_LEN, 0);

        int seq, flag;
        unpack_rdt_pkt(NULL, rdt_pkt, pkt_len, &seq, &flag);

        if (flag == RDT_CTRL_ACK) {
            // 处理接收端发回的ACK，重新设置滑动窗口内数据包的状态
            printf("\t[child thread] recv ack# %-8d\n", seq);

            pthread_mutex_lock(&send_window.lock);
            if (seq >= send_window.left && seq < send_window.right) {
                for (int i = send_window.left; i <= seq; i++) {
                    StatePkt *ptr_pkt = &send_window.rdt_pkts[i % send_window.len];
                    if (ptr_pkt->state == RDT_PKT_ST_SENT) {
                        ptr_pkt->state = RDT_PKT_ST_ACKED;
                        total_send_byte += ptr_pkt->pkt_len - RDT_HEADER_LEN;
                    }
                }
            } else if (seq < send_window.left) {
                printf("\t[child thread] already acked #pkt %-8d\n", seq);
            } else {
                printf("\t[child thread] recv wrong acked, #pkt%d > #window_right%d.\n", seq, send_window.right);
            }
            pthread_mutex_unlock(&send_window.lock);
        }
    }
}

/**
 * 检查是否超过设定时限(RDT_TIME_OUT)，
 * @return 如果超时返回1，否则返回0。
 */
int time_out(struct timeval then) {
    struct timeval now;
    gettimeofday(&now, NULL);
    int elapse_time = (now.tv_sec - then.tv_sec) * 1000000 + (now.tv_usec - then.tv_usec);
    if (elapse_time > RDT_TIME_OUT) {
        printf("[main thread] pkt timeout, elapsed_time : %d\n", elapse_time);
        return 1;
    }
    return 0;
}

/**
 *  准备发送窗口，滑动并填充数据，同时检查超时。
 * @return 是否有数据包需要发送
 */
int prepare_send_window(FILE *fp) {
    pthread_mutex_lock(&send_window.lock);

    if (send_window.left == send_window.right) {
        printf("[main thread] begin sending, slide window[%d,%d).\n", send_window.left, send_window.right);
    }

    StatePkt *ptr_pkt_left = &send_window.rdt_pkts[send_window.left % send_window.len];

    // step 1. 如果滑动窗口最左端的包已经收到ACK，则将滑动窗口滑动到下一个没收到ACK的包的位置。
    if (ptr_pkt_left->state == RDT_PKT_ST_ACKED) {
        int max_acked = send_window.left;
        while (max_acked < send_window.right &&
               (send_window.rdt_pkts[max_acked % send_window.len].state == RDT_PKT_ST_ACKED))
            max_acked++;
        send_window.left = max_acked;
    }

    // step 2. 将新数据包装入滑动窗口右端空缺位置。
    int pkt_to_send = 0; // 需要发送的数据包个数
    for (int i = send_window.right; i < send_window.left + send_window.len; i++) {
        if (feof(fp))
            break;
        char data_buf[RDT_DATA_LEN];
        int data_len = fread(data_buf, sizeof(char), RDT_DATA_LEN, fp);

        StatePkt *ptr_pkt = &send_window.rdt_pkts[i % send_window.len];
        int rdt_pkt_len = pack_rdt_pkt(data_buf, ptr_pkt->rdt_pkt, data_len, i, RDT_CTRL_DATA);
        ptr_pkt->pkt_seq = i;
        ptr_pkt->pkt_len = rdt_pkt_len;
        ptr_pkt->state = RDT_PKT_ST_INIT;

        pkt_to_send++;
    }
    send_window.right += pkt_to_send;

    if (pkt_to_send == 0) {
        // 如果滑动窗口最左端的包还没有收到数据包，则检查其是否超时，如果超时，则重新发送所有滑动窗口内的数据包。否则继续等待。
        // 在选择重传协议中，如果最左边的包超时，那么需要遍历所有包检查其超时情况，对超时的包进行重新发送。

        if (time_out(ptr_pkt_left->send_time)) {
            // 第一个数据包超时,重新发送窗口内所有数据包
            for (int i = send_window.left; i < send_window.right; i++) {
                StatePkt *ptr_pkt = &send_window.rdt_pkts[i % send_window.len];
                ptr_pkt->state = RDT_PKT_ST_TMOUT;
                pkt_to_send++;
            }
            printf("[main thread] slide window[%d,%d), first pkt time out.\n", send_window.left, send_window.right);
        }
    }

    pthread_mutex_unlock(&send_window.lock);

    if (pkt_to_send > 0)
        printf("[main thread] #%d pkts to send.\n", pkt_to_send);

    return pkt_to_send > 0;
}

/**
 * 发送文件
 * 创建一个子线程用于接收监听端口并接受ACK
 * 主线程用于维护滑动窗口并发送数据包
 * @param send_file_name - 待发送的文件名
 * @param sockfd - 发送数据的socket
 */
int deliver_file(char *send_file_name, int sockfd) {
    //初始化滑动窗口
    send_window.len = RDT_SENDWIN_LEN;
    send_window.left = RDT_BEGIN_SEQ;
    send_window.right = RDT_BEGIN_SEQ;

    //初始化互斥量
    pthread_mutex_init(&send_window.lock, NULL);

    //创建子线程用于接收ACK
    pthread_t worker_thread;
    if (pthread_create(&worker_thread, NULL, receive_acks, (void *)&sockfd) != 0) {
        perror("pthread_create failed.\n");
        exit(1);
    }

    FILE *fp;
    if ((fp = fopen(send_file_name, "r")) == NULL) {
        printf("open file : %s failed.\n", send_file_name);
        return 1;
    }

    int counter = 1;
    while (1) {
        /*检查滑动窗口内是否有数据包需要发送*/
        int has_pkt_to_send = prepare_send_window(fp);
        if (has_pkt_to_send) {
            for (int i = send_window.left; i < send_window.right; i++) {
                StatePkt *ptr_pkt = &send_window.rdt_pkts[i % send_window.len];
                if (ptr_pkt->state == RDT_PKT_ST_INIT || ptr_pkt->state == RDT_PKT_ST_TMOUT) {
                    rdt_send(sockfd, ptr_pkt->rdt_pkt, ptr_pkt->pkt_len, 0);

                    ptr_pkt->state = RDT_PKT_ST_SENT;
                    gettimeofday(&ptr_pkt->send_time, NULL);
                    printf("[main thread] send count #%-8d rdt_pkt #%-8d %-10d bytes.\n", counter++, ptr_pkt->pkt_seq, ptr_pkt->pkt_len);
                }
            }
            printf("[main thread] slide window: [%d,%d).\n", send_window.left, send_window.right);
        } else if (send_window.left == send_window.right && feof(fp)) {
            printf("[main thread] finished sending! slide window: [%d,%d).\n", send_window.left, send_window.right);
            break;
        }
    }

    //结束发送过程，给接受端发送结束数据包(包类型为RDT_CTRL_END)
    if (feof(fp)) {
        char rdt_pkt[RDT_PKT_LEN];
        int new_pkt_seq = send_window.left;
        int rdt_pkt_len = pack_rdt_pkt(NULL, rdt_pkt, 0, new_pkt_seq, RDT_CTRL_END);
        send(sockfd, rdt_pkt, rdt_pkt_len, 0);
    }

    printf("\n\n");
    printf("Send file %s finished.\n", send_file_name);
    printf("Total send %5d bytes.\n", total_send_byte);

    fclose(fp);

    //销毁子线程
    if (pthread_cancel(worker_thread) != 0) {
        printf("[main thread] pthread_cancel failed.\n");
        exit(1);
    }

    //删除互斥量
    pthread_mutex_destroy(&send_window.lock);
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