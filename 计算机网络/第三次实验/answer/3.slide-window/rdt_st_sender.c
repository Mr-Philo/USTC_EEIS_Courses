#include "net_exp.h"

SLD_WIN sending_window; //滑动窗口数据结构体
int total_send_byte = 0; //记录累计发送的字节数	

void usage( char **argv )
{
	printf( "wrong argument!\n" );
	printf( "usage: %s send_file_name. \n",  argv[0] );
}


/*
	处理接收端发回的ACK，重新设置滑动窗口内数据包的状态
*/
void slide_window_ack( int ack_num )
{	
	pthread_mutex_lock( &sending_window.lock );//将互斥量上锁
	
	printf( "\t[child thread] recv ack# %-8d\n",  ack_num );
	if( ack_num >= sending_window.send_left && ack_num < sending_window.send_right )
	{
		/******************************************************
		 * */
		if( sending_window.rdt_pkts[ack_num % sending_window.win_len].state == RDT_PKT_ST_SENT )
			{
				sending_window.rdt_pkts[ack_num % sending_window.win_len].state = RDT_PKT_ST_ACKED; //set pkt state to acked
				total_send_byte += (sending_window.rdt_pkts[ack_num % sending_window.win_len].pkt_len - RDT_HEADER_LEN);
			}
	}
	else
	{
		if( ack_num < sending_window.send_left )
			printf( "\t[child thread] already acked #pkt %-8d\n", ack_num );
		else
		{
			printf( "\t[child thread] recv wrong acked, #ack[%d] > sending_window.send_right[%d].\n", ack_num, sending_window.send_right );
			sleep(10); //wrong ack, do something?
		}
	}
	
	pthread_mutex_unlock( &sending_window.lock ); //将互斥量上锁
}

/*
	用于监听端口并接收ACK包的线程函数
*/
void *recv_acks_thread( void *arg )
{
	int *sock_fd = ( int *) arg;
	char rdt_pkt[RDT_PKT_LEN];
	int pkt_len;
	
	struct sockaddr_in reply_addr;
	int reply_addr_len;
	int reply_ack_seq;
	int reply_ack_flag;
	memset( &reply_addr, 0, sizeof(reply_addr) );
	reply_addr_len = sizeof( reply_addr );
	
	printf( "\t[child thread] waiting acks..\n" );
	while(1)
	{		
		/*检查是否有数据包到达*/
		pkt_len = recvfrom( *sock_fd, rdt_pkt, RDT_PKT_LEN, MSG_PEEK, 
			(struct sockaddr *)&reply_addr, &reply_addr_len );
			
		if( pkt_len > 0 ) //有数据包到达
		{
			pkt_len = recvfrom( *sock_fd, rdt_pkt, RDT_PKT_LEN, 0, 
				(struct sockaddr *)&reply_addr, &reply_addr_len   );	
			
			unpack_rdt_pkt( NULL, rdt_pkt, pkt_len, &reply_ack_seq, &reply_ack_flag );
			if( reply_ack_flag == RDT_CTRL_ACK ) //有ACK包到达
				slide_window_ack( reply_ack_seq ); //重新设置滑动窗口状态
		}
	}
}



/*
	检查滑动窗口内是否有数据包需要发送，返回窗口内待发送的数据包数。
*/
int pre_sending_rdt_pkt( FILE *fp )
{
	int pkt_to_send = 0;
	
	char read_buf[RDT_DATA_LEN];
	int send_base_acked;
	
	pthread_mutex_lock( &sending_window.lock );
	
	STATE_PKT *ptr_pkt_left = &sending_window.rdt_pkts[ sending_window.send_left % sending_window.win_len];
	send_base_acked = (ptr_pkt_left->state == RDT_PKT_ST_ACKED) ? 1 : 0;
	
	
	/*
		如果滑动窗口最左端的包已经收到ACK，则将滑动窗口滑动到下一个没收到ACK的包的位置，
		同时将新数据包装入滑动窗口右端空缺位置。
	*/
	if( send_base_acked == 1 ) 
	{
		int new_pkt_seq;
		int max_acked;
		max_acked = sending_window.send_left;
		
		while( max_acked < sending_window.send_right &&
				(sending_window.rdt_pkts[max_acked % sending_window.win_len].state == RDT_PKT_ST_ACKED) )
			max_acked++;
		sending_window.send_left = max_acked;	

		for( 	new_pkt_seq = sending_window.send_right; 
				new_pkt_seq < (sending_window.send_left + sending_window.win_len); 
				new_pkt_seq++ )
		{
				if( feof(fp) ) //检查是否已到达发送文件结尾
					break;
					
				int read_len, rdt_pkt_len;
				STATE_PKT *ptr_pkt_new = &sending_window.rdt_pkts[new_pkt_seq % sending_window.win_len];
				
				read_len = fread( read_buf, sizeof(char), RDT_DATA_LEN, fp ); //读取发送文件中的数据
				rdt_pkt_len = pack_rdt_pkt( read_buf, 
											ptr_pkt_new->rdt_pkt, 
											read_len, 
											new_pkt_seq, 
											RDT_CTRL_DATA ); //封装为RDT数据包
				
				//初始化新RDT数据包状态
				ptr_pkt_new->pkt_seq = new_pkt_seq;
				ptr_pkt_new->pkt_len = rdt_pkt_len;
				ptr_pkt_new->state = RDT_PKT_ST_INIT; 
				memset( &(ptr_pkt_new->send_time), 0, sizeof(struct timeval) );
				
				pkt_to_send++;
		}
		sending_window.send_right = new_pkt_seq;
		printf( "[main thread] slide window: [%d,%d).\n", sending_window.send_left, sending_window.send_right );		
	}
	/*
		如果滑动窗口最左端的包还没有收到数据包，则检查其是否超时，如果超时，则重新发送所有滑动窗口内的数据包。否则继续等待。
		在选择重传协议中，如果最左边的包超时，那么需要遍历所有包检查其超时情况，对超时的包进行重新发送。
	*/
	else 
	{
		struct timeval time_now;
		gettimeofday( &time_now, NULL );
		
		/*
			如果是第一次发送数据，将滑动窗口初始化，填满待发送的数据
		*/
		if( sending_window.send_left == sending_window.send_right )
		{
			int new_pkt_seq;
			for( 	new_pkt_seq = sending_window.send_right; 
					new_pkt_seq < (sending_window.send_left + sending_window.win_len); 
					new_pkt_seq++ )
			{
					int read_len, rdt_pkt_len;
					if( feof(fp) ) //检查是否已到达发送文件结尾
						break;
					STATE_PKT *ptr_pkt_new = &sending_window.rdt_pkts[new_pkt_seq % sending_window.win_len];
					read_len = fread( read_buf, sizeof(char), RDT_DATA_LEN, fp );
					rdt_pkt_len = pack_rdt_pkt( read_buf, 
												ptr_pkt_new->rdt_pkt, 
												read_len, 
												new_pkt_seq, 
												RDT_CTRL_DATA );
					ptr_pkt_new->pkt_seq = new_pkt_seq;
					ptr_pkt_new->pkt_len = rdt_pkt_len;
					ptr_pkt_new->state = RDT_PKT_ST_INIT; //set pkt state to init state
					memset( &(ptr_pkt_new->send_time), 0, sizeof(struct timeval) );
					pkt_to_send++;
			}
			sending_window.send_right = new_pkt_seq;
			printf( "[main thread] begin sending, slide window[%d,%d).\n", 
							sending_window.send_left, sending_window.send_right );
		}	
		/*学生修改的代码部分=========================================================================
		=======选择重传协议代码参考==================================================================
		*/

		else{
			int i;
			for( i = sending_window.send_left; i< sending_window.send_right; i++)
			{
				STATE_PKT *ptr_pkt = &sending_window.rdt_pkts[i % sending_window.win_len];
				if(time_out(time_now,ptr_pkt->send_time) && ptr_pkt->state == RDT_PKT_ST_SENT)
				{
					printf("[mainthread]Packet #%d timeout.\n",ptr_pkt->pkt_seq);
					ptr_pkt->state = RDT_PKT_ST_TMOUT;
					pkt_to_send ++;

				}
			}
		}
		/*学生写的代码到此结束=======================================================================
		==========================================================================================*/

	}
		
	pthread_mutex_unlock( &sending_window.lock );
	
	return pkt_to_send;
}


/*
	Go-Back-N 协议发送端函数
	输入参数：
		send_file_name: 待发送的文件名
		sock_fd：发送数据的socket (同时从该socket发送数据包和接受数据包ACK)
		recv_addr_ptr: 接收端的地址
	说明：
		创建一个子线程用于接收监听端口并接受ACK
		而主线程用于维护滑动窗口并发送数据包
*/

int deliver_file( char *send_file_name, int sock_fd,  struct sockaddr_in *recv_addr_ptr )
{
	char recv_pkt_buf[RDT_PKT_LEN];
	int seq_num = RDT_BEGIN_SEQ;
	
	struct sockaddr_in reply_addr;
	int reply_addr_len;
	int reply_ack_seq;
	int reply_ack_flag;
	pthread_t worker_thread;
	int reply_thread;
	
	FILE *fp;
	int i, j, read_len, pkt_len;
	int counter = 1;
	STATE_PKT *ptr_pkt;

	if( (fp = fopen( send_file_name, "r" )) == NULL )
	{
		printf( "open file : %s failed.\n",  send_file_name );
		return 1;
	}
	
	memset( &reply_addr, 0, sizeof(reply_addr) );
	
	//初始化互斥量
	pthread_mutex_init( &sending_window.lock, NULL );
	
	//初始化滑动窗口
	sending_window.win_len = RDT_SENDWIN_LEN;
	sending_window.send_left = RDT_BEGIN_SEQ;
	sending_window.send_right = RDT_BEGIN_SEQ;	//[slide window]= [RDT_BEGIN_SEQ, RDT_BEGIN_SEQ)
	ptr_pkt = &sending_window.rdt_pkts[sending_window.send_left % sending_window.win_len];
	ptr_pkt->pkt_seq = RDT_BEGIN_SEQ;	
	ptr_pkt->state = 0;

	//创建子线程用于接收ACK
	reply_thread = pthread_create( &worker_thread, NULL, recv_acks_thread, (void *)&sock_fd );
	if( reply_thread != 0 )
	{
		perror( "pthread_create failed.\n" );
		exit( 1 );
	}
	
	while(1)
	{
		int pkt_to_send;
		
		/*检查滑动窗口内是否有数据包需要发送*/
		pkt_to_send = pre_sending_rdt_pkt( fp );
		
		if( pkt_to_send > 0 )
		{
			printf( "[main thread] #%d pkts to send.\n", pkt_to_send );
			for( i = sending_window.send_left; i < sending_window.send_right; i++ )
			{
				STATE_PKT *ptr_pkt;
				ptr_pkt = &sending_window.rdt_pkts[ i % sending_window.win_len];
				if( ptr_pkt->state == RDT_PKT_ST_INIT ||  ptr_pkt->state == RDT_PKT_ST_TMOUT )
				{
					udt_sendto( sock_fd, ptr_pkt->rdt_pkt, ptr_pkt->pkt_len, 0,
							(struct sockaddr *)recv_addr_ptr, sizeof(*recv_addr_ptr) );
					
					ptr_pkt->state = RDT_PKT_ST_SENT;
					gettimeofday( &ptr_pkt->send_time, NULL );
					printf( "[main thread] send count #%-8d rdt_pkt #%-8d %-10d bytes.\n", counter++, ptr_pkt->pkt_seq, ptr_pkt->pkt_len );
				}
			}
			
		}
		else
		{
			//检查文件是否发送完
			if( sending_window.send_left == sending_window.send_right && feof(fp) ) //data transfer finished
			{
				printf( "[main thread] finished sending! slide window: [%d,%d).\n", sending_window.send_left, sending_window.send_right );
				reply_thread = pthread_cancel( worker_thread ); /*删除子线程*/
				if( reply_thread != 0 )
				{
					printf( "[main thread] pthread_cancel failed.\n" );
					exit(1);
				}
				break;			
			}
		}
	}
	
	//删除互斥量
	pthread_mutex_destroy( &sending_window.lock );
	
	//结束发送过程，给接受端发送结束数据包(包类型为RDT_CTRL_END)
	if( feof(fp) )
	{
		char rdt_pkt[RDT_PKT_LEN];
		int rdt_pkt_len;
		int new_pkt_seq = sending_window.send_left;
		rdt_pkt_len = pack_rdt_pkt( NULL, 
							rdt_pkt, 
							0, 
							new_pkt_seq, 
							RDT_CTRL_END );
		sendto( sock_fd, rdt_pkt, rdt_pkt_len, 0, (struct sockaddr *)recv_addr_ptr, sizeof(*recv_addr_ptr) );
	}
	printf( "\n\nsend file %s finished\ntotal send %d bytes.\n", send_file_name, total_send_byte );
	fclose( fp );
	return 0;
}

int main( int argc, char **argv )
{
	struct sockaddr_in recv_addr, send_addr;
	int sock_fd;
	
	if( argc != 2 )
	{
		usage( argv );
		exit(0);
	}
	
	srand ( time(NULL) );
	
	if( init_socket_sender(&sock_fd,&send_addr,&recv_addr) == 0)
	{
		printf("Init socket failed.\n");
		close(sock_fd);
		exit(1);
	}
	
	//调用传输文件的函数
	if( deliver_file( argv[1], sock_fd, &recv_addr ) != 0 )
	{
		printf( "deliver file %s failed.\n", argv[1] );
		close( sock_fd );
		exit(1);
	}
	
	close( sock_fd );
	return 0;
}


