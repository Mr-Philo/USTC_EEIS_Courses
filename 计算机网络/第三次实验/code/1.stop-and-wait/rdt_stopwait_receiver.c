#include "net.h"

/**
 * 接收端接收函数
 * @param save_file_name - 保存文件名
 * @param sockfd - 接收数据的socket
 */
int receive_file(char *save_file_name, int sockfd) {
    // 将收到的RDT数据包按顺序写到此文件中
    FILE *fp = NULL;        //马的傻逼！这个示例代码没写==NULL，然后就段错误了！！！SB啊啊啊啊啊
    if ((fp = fopen(save_file_name, "w")) == NULL) {
        printf("open file : %s failed.\n", save_file_name);
        return 1;
    }

    // 总共接收的字节数
    int total_recv_byte = 0;

    // 当前接收端需要的数据包序列号
    int exp_seq_num = RDT_BEGIN_SEQ;

    printf("Prepared.\n");
    //接收RDT数据包，直到所有数据全部接收完毕
    while (1) {
        // step 1. 接收RDT数据包
        char rdt_pkt[RDT_PKT_LEN];
        //int flag;   //我觉得这个flag应该放在前面定义？
        int pkt_len = recv(sockfd, rdt_pkt, RDT_PKT_LEN, 0);    //*TODO 1
        //函数"recv"原始定义为: recv (int __fd, void *__buf, size_t __n, int __flags)

        // step 2. 解封装RDT数据包
        char rdt_data[RDT_DATA_LEN];
        //int seq_num; 
        int seq_num, flag; 
        int data_len = unpack_rdt_pkt(rdt_data, rdt_pkt, pkt_len, & seq_num, & flag);       //*TODO 2
        //函数"unpack_rdt_pkt"原始定义为: int unpack_rdt_pkt(char *data_buf, char *rdt_pkt, int pkt_len, int *seq_num, int *flag)
        //printf("%s\n",rdt_data);

        // step 3. 检查此数据包是否为期待的数据包 : seq_num==exp_seq_num
        /* TODO 3 */
        if (seq_num==exp_seq_num){      //此数据包是否为期待的数据包,接受成功
            printf("[MyReceiver]Packet #%d received, send ACK, now exp_num is %d\n", exp_seq_num, exp_seq_num+1);
            exp_seq_num ++;
            total_recv_byte += data_len;
            rdt_data[data_len] = '\0';      //否则每段结束会有乱码
            fputs(rdt_data, fp);      //将接受到的数据写入文件
        }
        //调试日志11.17-18：21  一个大bug：第一个两边都没问题；接受方接到了第二个，返回ACK，但这个ACK丢了，发送方以为第二个没发过去，一直重复发第二个，但接受方只要第三个，死循环
        //解决方法：接受方多确认一次
        else if(seq_num==(exp_seq_num-1)){  //该数据包是因为上一次ACK丢了，发送方又发来一个重复的，丢弃之，但仍返回上一个序号对应的ACK确认包
            //啥也不做就好，但不continue，进入后面的ACK发送
            //成功了！！！！！！！ 18：30
        }
        else{   //该数据包不是预期数据包，丢弃之
            continue;   
        }

        // step 4. 封装一个新的RDT数据包(ACK包)
        char reply_rdt_pkt[RDT_PKT_LEN];
        int reply_pkt_len = pack_rdt_pkt(rdt_data, reply_rdt_pkt, RDT_PKT_LEN, seq_num, RDT_CTRL_ACK);       //*TODO 4
        //函数"pack_rdt_pkt"原始定义为: int pack_rdt_pkt(char *data_buf, char *rdt_pkt, int data_len, int seq_num, int flag)

        // step 5. 调用不可靠数据传输发送新的RDT数据包(ACK包)
        udt_send(sockfd, reply_rdt_pkt, reply_pkt_len, RDT_CTRL_ACK);        //*TODO 5
        //这里最后一个参数flag设为2(RDT_CTRL_ACK)?因为是ACK包吗?

        if (flag == RDT_CTRL_END) {
            break;
        }
    }   //while循环结束，表明传输全过程已经完成

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
