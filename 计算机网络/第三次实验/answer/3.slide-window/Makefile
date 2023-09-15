
PROGS = rdt_st_receiver rdt_st_sender

CC = cc		
CFLAGS = -g #-Wall
		
all: ${PROGS}
		
rdt_st_receiver: rdt_st_receiver.o rdt_pkt_util.c net_exp.h
	${CC} ${CFLAGS} -pthread -o $@ rdt_st_receiver.o rdt_pkt_util.c
	
rdt_st_sender: rdt_st_sender.o rdt_pkt_util.c net_exp.h
	${CC} ${CFLAGS} -pthread -o $@ rdt_st_sender.o rdt_pkt_util.c
	
.PHONY: clean	

clean:
	rm -f *.o *.exe ${PROGS} exp_code_st.tar.gz

tar:
	tar czf exp_code_st.tar.gz rdt_st_receiver.c rdt_st_sender.c \
	rdt_pkt_util.c net_exp.h README Makefile
