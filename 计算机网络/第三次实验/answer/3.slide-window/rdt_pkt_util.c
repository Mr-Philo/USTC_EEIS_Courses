#include "net_exp.h"

/*
	将数据封装成RDT数据包：即在数据前加上RDT数据包头部
*/
int pack_rdt_pkt( char *data_buf, char *rdt_pkt, int data_len, int seq_num, int flag )
{
	char *ptr = rdt_pkt;
	uint32_t ctrl_net_order = htonl( flag );
	uint32_t seq_net_order = htonl( seq_num );

	memcpy( ptr, &ctrl_net_order, sizeof(uint32_t) );
	ptr += sizeof(uint32_t);
	memcpy( ptr, &seq_net_order, sizeof(uint32_t) );
	ptr += sizeof(uint32_t);
	
	if( data_len > 0 && data_buf != NULL )
		memcpy( ptr, data_buf, data_len );
	return (RDT_HEADER_LEN+data_len);
}

/*
	将RDT数据包解封装
*/
int unpack_rdt_pkt( char *data_buf, char *rdt_pkt, int pkt_len, int *seq_num, int *flag )
{
	char *ptr = rdt_pkt;
	uint32_t ctrl_net_order, seq_net_order;
	int data_len;
	
	memcpy( &ctrl_net_order, ptr, sizeof(uint32_t) );
	ptr += sizeof(uint32_t);
	*flag = ntohl( ctrl_net_order );
	
	memcpy( &seq_net_order, ptr, sizeof(uint32_t) );
	ptr += sizeof(uint32_t);
	*seq_num = ntohl( seq_net_order );
	
	data_len = pkt_len - RDT_HEADER_LEN;
	if( data_buf != NULL && data_len > 0 )
		memcpy( data_buf, ptr, data_len );
	return data_len;
}

/*
	模拟不可靠数据传输，以一定的概率（RDT_PKT_LOSS_RATE）丢弃数据包
*/
void udt_sendto( int sock_fd, char *pkt, int pkt_len, int flags, struct sockaddr *recv_addr, int addr_len )
{
	int seed =  rand() % 100;
	if( seed >= RDT_PKT_LOSS_RATE )
		sendto( sock_fd, pkt, pkt_len, flags, recv_addr, addr_len );
	else //pkt lost
		printf( "emulate packet[next] lost!\n" );
}

/*
	检查数据包是否出现了超时情况。
*/
int time_out( struct timeval time_1, struct timeval time_2 ) //time_1 > time_2
{
	int elapse_time = (time_1.tv_sec - time_2.tv_sec)*1000000 + (time_1.tv_usec - time_2.tv_usec);
	if( elapse_time > RDT_TIME_OUT )
	{	
		//printf( "[main thread] pkt timeout, elapsed_time : %d\n", elapse_time );
		return 1;
	}
	return 0;
}

/*
	直接初始化rdt的发送端需要的Socket	
*/
int init_socket_sender(int *sock_fd,struct sockaddr_in *send_addr,struct sockaddr_in  *recv_addr)
{
	/*
	输入参数：
		sock_fd: socket描述符
		send_addr :待初始化的发送端地址结构体，用于bind自己
		recv_addr :待初始化的接收端地址结构体，用于得知发给谁
			上述二者均使用net_exp.h中定义的RDT发送端口、接受端口等进行了初始化，默认地址为本机地址。
	
	输出参数：整数
		0 ：出错，具体错误由print展示
		1 ：没出错

	======================================================================================
	使用方法：
		这个函数应该且只应该在rdt_stopwait_sender.c或者rdt_gbn_sender.c中调用。
		
		只需要在main函数中对sock_fd, send_addr, recv_addr进行定义，再直接调用该函数即可进行通信。
		
		e.g.
		int main( int argc, char **argv )
		{
			struct sockaddr_in recv_addr, send_addr;
			int sock_fd;
			init_socket_sender(&sock_fd,&send_addr,&recv_addr);
			然后就可以开始使用sock_fd来进行文件的发送了。
			}
	=======================================================================================
	*/

	if( ( *sock_fd = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 )
	{
		printf( "error! information: %s\n", strerror(errno) );
		exit(0);	
	}
	memset( send_addr, 0, sizeof(*send_addr) );
	(*send_addr).sin_family = AF_INET;
	(*send_addr).sin_addr.s_addr = htonl( INADDR_ANY );
	(*send_addr).sin_port = htons( RDT_SEND_PORT );
	
	if( ( *sock_fd = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 )
	{
		printf( "error! information: %s\n", strerror(errno) );
		exit(0);	
	}
	
	if( bind( *sock_fd, (struct sockaddr *)send_addr, sizeof(*send_addr) ) == -1 )
	{
		close( *sock_fd );
		printf( "error! information: %s\n", strerror(errno) );
		exit(0);	
	}
			
	memset( recv_addr, 0, sizeof(*recv_addr) );
	(*recv_addr).sin_family = AF_INET;
	(*recv_addr).sin_addr.s_addr = inet_addr( RDT_SERVER_ADDRESS );
	(*recv_addr).sin_port = htons( RDT_RECV_PORT );
	return 1;
}

/*
	直接初始化rdt的接收端需要的Socket	
*/
int init_socket_receiver(int *sock_fd,struct sockaddr_in *recv_addr){
/*
	输入参数：
		sock_fd: socket描述符
		recv_addr :待初始化的发送端地址结构体，用于bind自己
			上述二者均使用net_exp.h中定义的RDT发送端口、接受端口等进行了初始化，默认地址为本机地址。
	
	输出参数：整数
		0 ：出错，具体错误由print展示
		1 ：没出错

	======================================================================================
	使用方法：
	这个函数应该且只应该在rdt_stopwait_receiver.c或者rdt_gbn_receiver.c中调用
	
	只需要在main函数中对sock_fd, recv_addr进行定义，再直接调用该函数即可进行通信。
		e.g.
		int main( int argc, char **argv )
		{
			struct sockaddr_in recv_addr;
			int sock_fd;
			init_socket_receiver(&sock_fd,&send_addr,&recv_addr);
			然后就可以开始使用sock_fd来进行文件的发送了。
			}
	=======================================================================================
*/
	memset( recv_addr, 0, sizeof(*recv_addr) );
	(*recv_addr).sin_family = AF_INET;
	(*recv_addr).sin_addr.s_addr = htonl( INADDR_ANY );
	(*recv_addr).sin_port = htons( RDT_RECV_PORT );
	
	if( ( *sock_fd = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 )
	{
		printf( "error! information: %s\n", strerror(errno) );
		exit(0);	
	}
	
	if( bind( *sock_fd, (struct sockaddr *)recv_addr, sizeof(*recv_addr) ) == -1 )
	{
		close( *sock_fd );
		printf( "error! information: %s\n", strerror(errno) );
		exit(0);	
	}
	return 1;
}
