#ifndef NET_H
#define NET_H

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define RDT_SERVER_ADDRESS "127.0.0.1" //RDT服务器端IP
#define RDT_RECV_PORT 8003             //RDT接受端端口号
#define RDT_SEND_PORT 8004             //RDT发送端端口号
#define RDT_PKT_LOSS_RATE 10           //不可靠数据传输层的丢包率
#define RDT_TIME_OUT 50000             //数据包超时时限

#define RDT_SENDWIN_LEN 10                          //发送端窗口大小
#define RDT_BEGIN_SEQ 1                             //RDT数据包初始序列号，假设数据包序列号不循环
#define RDT_HEADER_LEN (4 + 4)                      //RDT头标长度
#define RDT_DATA_LEN 1000                           //RDT中数据域长度
#define RDT_PKT_LEN (RDT_DATA_LEN + RDT_HEADER_LEN) //RDT数据包长度

//RDT包类型
#define RDT_CTRL_BEGN 0 //初始包
#define RDT_CTRL_DATA 1 //数据包
#define RDT_CTRL_ACK 2  //ACK包
#define RDT_CTRL_END 3  //结束包

//滑动窗口中数据包的状态
#define RDT_PKT_ST_INIT 0  //未发送
#define RDT_PKT_ST_SENT 1  //已发送，未确认
#define RDT_PKT_ST_ACKED 2 //已确认
#define RDT_PKT_ST_TMOUT 3 //超时

/*
	滑动窗口中数据包的结构体
*/
typedef struct _StatePkt {
    struct timeval send_time;
    int pkt_seq;
    int pkt_len;
    int state; //标识数据包的状态
    char rdt_pkt[RDT_PKT_LEN];
} StatePkt;

/*
	滑动窗口的结构体
*/
typedef struct _SendWindow {
    int len;                            //滑动窗口长度
    int left;                           //滑动窗口的左端
    int right;                          //滑动窗口的右端
    pthread_mutex_t lock;               //滑动窗口互斥量，保证只有一个线程在访问发送窗口中的数据
    StatePkt rdt_pkts[RDT_SENDWIN_LEN]; //窗口中的数据包
} SendWindow;

/**
 * 将数据封装成RDT数据包，头部只包含控制域和序列号域，其中控制域用来标识RDT数据包类型,序列号域是包含此数据包的序列号。
 * RDT包格式: |CTRL|SEQ|...DATA...|
 * @param data_buf - 数据缓冲区
 * @param rdt_pkt - RDT数据包
 * @param data_len - 数据缓冲区长度
 * @param seq_num - RDT数据包的序列号
 * @param flag - RDT数据包的类型
 * @return RDT数据包长度
 */
int pack_rdt_pkt(char *data_buf, char *rdt_pkt, int data_len, int seq_num, int flag);

/**
 * 将RDT数据包解封装成数据。
 * RDT包格式: |CTRL|SEQ|...DATA...|
 * @param data_buf - 数据缓冲区
 * @param rdt_pkt - RDT数据包
 * @param pkt_len - RDT数据包长度
 * @param seq_num - RDT数据包的序列号
 * @param flag - RDT数据包的类型
 * @return 数据缓冲区长度
 */
int unpack_rdt_pkt(char *data_buf, char *rdt_pkt, int pkt_len, int *seq_num, int *flag);

/**
 * 模拟不可靠数据传输，以RDT_PKT_LOSS_RATE指定的概率丢弃数据包
 * 用法与socket的send一致
 */
void rdt_send(int sockfd, char *pkt, int pkt_len, int flags);

/**
 * 初始化RDT的发送端的socket
 * @return 初始化的socket
 */
int init_socket_sender();

/**
 * 初始化rdt的接收端的Socket
 * @return 初始化的socket
 */
int init_socket_receiver();

#endif