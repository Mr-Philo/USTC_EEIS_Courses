#ifndef NETEXP_H
#define NETEXP_H

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <ctype.h>


#define		TCP_SERVER_ADDRESS		"127.0.0.1"
#define 	TCP_SERVER_PORT			8000
#define		TCP_BUF_LENGTH			1000
#define 	CONN_NUM				10

#define		UDP_SERVER_ADDRESS		"127.0.0.1"
#define		UDP_RECV_PORT			8001 
#define		UDP_SEND_PORT			8002 
#define		UDP_BUF_LENGTH			1000

#define		RDT_SERVER_ADDRESS		"127.0.0.1"		//RDT服务器端IP
#define 	RDT_RECV_PORT			8003			//RDT接受端端口号
#define		RDT_SEND_PORT			8004			//RDT发送端端口号
#define		RDT_BEGIN_SEQ			1				//RDT数据包初始序列号，（假设数据包序列号不循环）		
#define		RDT_SENDWIN_LEN			10				//发送端窗口大小
#define     RDT_RECVWIN_LEN         10              //接收端窗口大小

#define		RDT_PKT_LOSS_RATE		10			//不可靠数据传输层的丢包率
#define		RDT_TIME_OUT			5000	//数据包超时时限
#define		RDT_HEADER_LEN			(4 + 4)		//RDT头标长度
#define 	RDT_DATA_LEN			1000		//RDT中数据域长度
#define		RDT_PKT_LEN				( RDT_DATA_LEN + RDT_HEADER_LEN )

//RDT包类型
#define		RDT_CTRL_BEGN			0 //初始包
#define		RDT_CTRL_DATA			1 //数据包
#define 	RDT_CTRL_ACK			2 //ACK包
#define		RDT_CTRL_END			3 //结束包

//滑动窗口中数据包的状态
#define		RDT_PKT_ST_INIT			0	//未发送
#define		RDT_PKT_ST_SENT			1	//已发送，未确认
#define		RDT_PKT_ST_ACKED		2	//已确认
#define		RDT_PKT_ST_TMOUT		3	//超时

#define     RDT_PKT_UNACKED         4   //接收端未收到的
#define     RDT_PKT_ACKED           5   //接受段已经接受到的

/*
	滑动窗口中数据包的结构体
*/
typedef struct _STATE_PKT
{
	struct timeval send_time;
	int pkt_seq;
	int pkt_len;
	int state; //标识数据包的状态
	char rdt_pkt[RDT_PKT_LEN];
}STATE_PKT;

/*
	滑动窗口的结构体
*/
typedef struct _SLD_WIN_RECV
{
	int win_len;							//滑动窗口长度
	int recv_left;							//滑动窗口的左端
	int recv_right;							//滑动窗口的右端
	pthread_mutex_t lock; 					//滑动窗口互斥量，保证只有一个线程在访问发送窗口中的数据
	STATE_PKT rdt_pkts[RDT_RECVWIN_LEN]; 	//窗口中的数据包
}SLD_WIN_RECV;

typedef struct _SLD_WIN
{
	int win_len;							//滑动窗口长度
	int send_left;							//滑动窗口的左端
	int send_right;							//滑动窗口的右端
	pthread_mutex_t lock; 					//滑动窗口互斥量，保证只有一个线程在访问发送窗口中的数据
	STATE_PKT rdt_pkts[RDT_RECVWIN_LEN]; 	//窗口中的数据包
}SLD_WIN;

/*
	RDT packet format: |CTRL|SEQ|...DATA...|
	将数据封装成RDT数据包，头部只包含控制域和序列号域，其中控制域用来标识RDT数据包类型,序列号域是此数据包的序列号。
	返回RDT数据包长度
*/
int pack_rdt_pkt( char *data_buf, char *rdt_pkt, int data_len, int seq_num, int flag );



/*
	RDT packet format: |CTRL|SEQ|...DATA...|
	
	将数据包解封装。
	返回RDT包中的数据长度
*/
int unpack_rdt_pkt( char *data_buf, char *rdt_pkt, int pkt_len, int *seq_num, int *flag );



/*
	模拟不可靠数据传输，以一定的概率（RDT_PKT_LOSS_RATE）丢弃数据包
*/
void udt_sendto( int sock_fd, char *pkt, int pkt_len, int flags, struct sockaddr *recv_addr, int addr_len );

/*
	初始化rdt的发送端需要的Socket	
*/
int init_socket_sender(int *sock_fd,struct sockaddr_in *send_addr, struct sockaddr_in *recv_addr);

/*
	初始化rdt的接受端需要的Socket
*/
int init_socket_receiver(int *sock_fd,struct sockaddr_in *receiver_addr);


/*
	检查 (time_1 - time_2) 是否超过设定时限(RDT_TIME_OUT)，
	如果超时返回1，否则返回0。
*/
int time_out( struct timeval time_1, struct timeval time_2 );

#endif

