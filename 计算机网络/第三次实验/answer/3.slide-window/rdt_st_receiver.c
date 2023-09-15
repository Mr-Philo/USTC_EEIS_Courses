#include "net_exp.h"

void usage( char **argv )
{
	printf( "wrong argument!\n" );
	printf( "usage: %s save_file_name\n",  argv[0] );
}

int slide_window(SLD_WIN_RECV *ptr_recv_window,FILE *fp, int data_len)
{	
	int i;
	int recv_byte= 0;
	int max_acked = ptr_recv_window->recv_left; 
	for(i = ptr_recv_window->recv_left;i<ptr_recv_window->recv_right;i++)
	{
		STATE_PKT *ptr_pkt = &ptr_recv_window->rdt_pkts[i % ptr_recv_window->win_len];
		if(ptr_pkt->state == RDT_PKT_ACKED){
			fwrite(ptr_pkt->rdt_pkt,sizeof(char),data_len,fp);
			recv_byte+= data_len;
			max_acked ++;
		}
		else{
			break;
		}
	}
	ptr_recv_window->recv_left = max_acked;
	
	for(i = ptr_recv_window->recv_right;i<(ptr_recv_window->win_len+ptr_recv_window->recv_right);i++)
	{
		STATE_PKT *ptr_pkt_new = &ptr_recv_window->rdt_pkts[i %ptr_recv_window->win_len];
		ptr_pkt_new->pkt_seq = i;
		ptr_pkt_new->state = RDT_PKT_UNACKED;
	}
	ptr_recv_window->recv_right = ptr_recv_window->recv_left + ptr_recv_window->win_len;
	printf("Slide Window,to [%d,%d]\n",ptr_recv_window->recv_left,ptr_recv_window->recv_right);
	return recv_byte;
}

int initrecvwindow(SLD_WIN_RECV *ptr_recv_window){
	ptr_recv_window->win_len = RDT_RECVWIN_LEN;
	ptr_recv_window->recv_left = RDT_BEGIN_SEQ;
	ptr_recv_window->recv_right = RDT_BEGIN_SEQ;
	int i;
	for(i = ptr_recv_window->recv_right;i<(ptr_recv_window->win_len+ptr_recv_window->recv_right);i++)
	{
		STATE_PKT *ptr_pkt_new = &ptr_recv_window->rdt_pkts[i % (ptr_recv_window->win_len)];
		ptr_pkt_new->pkt_seq = i;
		ptr_pkt_new->state = RDT_PKT_UNACKED;
	}
	ptr_recv_window->recv_right = i;
	return 0;
}
/*
	Go-Back-N 协议接收端接受函数
	输入参数：
		save_file_name： 保存文件名
		sock_fd：接受数据的socket
*/
int receive_file( char *save_file_name, int sock_fd )
{
	char reply_pkt_buf[RDT_PKT_LEN];
	int reply_pkt_len;
	
	char rdt_pkt[RDT_PKT_LEN];
	char rdt_data[RDT_DATA_LEN];
	uint32_t seq_net_order;
	int seq_num;
	int flag;
	int exp_seq_num;
	
	int total_recv_byte = 0;
	
	struct sockaddr_in client_addr;
	int i, j, sin_len, pkt_len, data_len;
	STATE_PKT *ptr_pkt;
	SLD_WIN_RECV recv_window;
	int counter = 1;
	FILE *fp;
	
	if( (fp = fopen( save_file_name, "w" )) == NULL )
	{
		printf( "open file : %s failed.\n",  save_file_name );
		return 1;
	}
	
	memset( &client_addr, 0, sizeof(client_addr) );
	sin_len = sizeof( client_addr );


	initrecvwindow(&recv_window);

	
	//TODO
	while(1) //接收RDT数据包，直到所有数据全部接收完毕
	{			
		/*
			step 1. 接收RDT数据包 :	recvfrom()
			step 2. 解封装RDT数据包 : unpack_rdt_pkt()
			step 3. 检查此数据包是否为期待的数据包 : seq_num==exp_seq_num
			step 4. 封装一个新的RDT数据包(ACK包) : pack_rdt_pkt()
			step 5. 调用不可靠数据传输发送新的RDT数据包(ACK包): udt_sendto()
		*/
		//********************************************************************************************
		//以下内容由学生完成，此处为参考性代码
		//********************************************************************************************
		pkt_len = recvfrom(sock_fd, rdt_pkt, RDT_PKT_LEN,0,(struct sockaddr *) &client_addr,&sin_len);
        data_len = unpack_rdt_pkt(rdt_data,rdt_pkt,pkt_len,&seq_num,&flag);
		printf("[Window]=[%d,%d), receive %d\n",recv_window.recv_left,recv_window.recv_right,seq_num);
		if(seq_num <= recv_window.recv_right)
		{
			reply_pkt_len = pack_rdt_pkt(NULL,reply_pkt_buf,0,seq_num,RDT_CTRL_ACK);
			udt_sendto(sock_fd,reply_pkt_buf,reply_pkt_len,0,(struct sockaddr *)&client_addr,sin_len);
			if(seq_num >=recv_window.recv_left)
			{
				recv_window.rdt_pkts[seq_num % recv_window.win_len].state = RDT_PKT_ACKED;
				recv_window.rdt_pkts[seq_num % recv_window.win_len].pkt_len = pkt_len;
				//recv_window.rdt_pkts[seq_num % recv_window.win_len].rdt_pkt = rdt_data;
				memcpy(recv_window.rdt_pkts[seq_num % recv_window.win_len].rdt_pkt,rdt_data,data_len);
				total_recv_byte += slide_window(&recv_window,fp,data_len);
			}	
		}
		else{
			printf("Left state = %d\n",recv_window.rdt_pkts[recv_window.recv_left % recv_window.win_len].state);
		}	
		if(flag == RDT_CTRL_END)
		{
			break;
		}
	}        													
	printf( "\n\nreceive file succeed. write to file %s\ntotal recv %d byte\n", 
				save_file_name, total_recv_byte );
		
	fflush( fp );
	fclose( fp );
	return 0;
}

int main( int argc, char **argv )
{
	struct sockaddr_in recv_addr;
	int sin_len;
	int sock_fd;
	int pkt_len;
	
	if( argc != 2 )
	{
		usage( argv );
		exit(0);
	}
	
	srand ( time(NULL) );
	
	if(init_socket_receiver(&sock_fd,&recv_addr) ==0)
	{
		printf("Init socket error.\n");
		close(sock_fd);
		exit(0);
	}
	
	if( receive_file( argv[1], sock_fd ) != 0 )
	{
		printf( "receive file %s failed.\n", argv[1] );
		close( sock_fd );
		exit(1);
	}
	
	close( sock_fd );
	return 0;
}


