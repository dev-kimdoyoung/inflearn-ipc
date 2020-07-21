#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>

/*
 *  [개요]
 *  POSIX에 제공하는 API를 통해 메세지 큐를 생성하여 다른 프로세스 간
 *  데이터 통신이 이루어지는지 확인
 */


#define POSIX_MQ_NAME		"/testlip"
#define MSGBUF_STR_SIZE		8192

static void print_usage(const char * progname) {
	printf("%s (send PRIORITY | recv)\n", progname);
}

/*  메세지 큐 생성  */
static int init_msgq(void) {
	
	mqd_t mqd;
	struct mq_attr attr;	// POSIX에서 제공하는 메세지 큐 속성
	int isSuccess;

	/* POSIX 메세지 큐 생성  */
	mqd = mq_open(POSIX_MQ_NAME, O_RDWR | O_CREAT, 0644, NULL);

	if(mqd == -1) {

		perror("mq_open()");
		return -1;
	}

	memset(&attr, 0, sizeof(attr));

	/*  메세지 큐 속성 값 attr에 저장  */
	isSuccess = mq_getattr(mqd, &attr);
	if(isSuccess == -1) {
		close(mqd);
		return -1;
	}

	/*  메세지 큐 속성 값 출력  */
	printf("mq_flags 	= %ld\n", attr.mq_flags);
	printf("mq_maxmsg	= %ld\n", attr.mq_maxmsg);
	printf("mq_msgsize	= %ld\n", attr.mq_msgsize);
	printf("mq_curmsgs	= %ld\n", attr.mq_curmsgs);

	return mqd;
}

/*  큐에 메세지 입력  */
static int do_send(unsigned int priority) {

	int mqd;
	int isSuccess;
	char buf[MSGBUF_STR_SIZE];

	/*  메세지 큐 생성  */
	mqd = init_msgq();
	if (mqd == -1) {
	
		perror("init_msgq()");
		return -1;
	}

	
	/*  메세지에 데이터 입력 후 메세지 큐에 Send  */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), 
			"hello world from pid %d", getpid());
	
	isSuccess = mq_send(mqd, buf, sizeof(buf), priority);
	if(isSuccess == -1) {
		perror("mq_send()");
		return -1;
	}

	return 0;
}

/*  큐에서 읽어온 메세지 출력  */
static int do_receive(void) {

	int mqd;
        int isSuccess;
        char buf[MSGBUF_STR_SIZE];
	unsigned priority;

        /*  메세지 큐 생성  */
        mqd = init_msgq();
        if (mqd == -1) {

                perror("init_msgq()");
                return -1;
        }

	/*  큐에 저장된 메세지 읽어오기  */
	memset(buf, 0, sizeof(buf));
	isSuccess = mq_receive(mqd, buf, sizeof(buf), &priority);
        if(isSuccess == -1) {

                perror("mq_send()");
		close(mqd);
                return -1;
        }

	/*  읽어온 메세지 출력  */
	printf("priority : %u, msg [%s]\n", priority, buf);

        return 0;
}

int main(int argc, char** argv) {

	int ret;

	if(argc < 2) {
		print_usage(argv[0]);
		return -1;
	}

	/*  메세지 큐에 send를 할지, receive를 할 지 매개변수로 결정  */
	if(!strcmp(argv[1], "send")) {

		unsigned int priority;
		
		if(argc < 3) {
		
			print_usage(argv[0]);
			return -1;
		}

		priority = (unsigned int)strtoul(argv[2], NULL, 10);
		ret = do_send(priority);
	} else if(!strcmp(argv[1], "recv")) {

		ret = do_receive();
	} else {

		print_usage(argv[0]);
		return -1;
	}

	return ret;
}
