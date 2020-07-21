#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define IPC_KEY_FILENAME 	"/proc"
#define IPC_KEY_PROJ_ID		'a'

/*  메세지 큐에서 사용할 메시지 정의  */
struct msgbuf {
	long mtype;       /* message type, must be > 0 */
	
#define MSGBUF_SIZE	64
	char string[MSGBUF_SIZE];    /* message data */
};

static void print_usage(const char * progname) {
	printf("%s (send | recv)\n", progname);
}

/*  메세지 큐 생성  */
static int init_msgq(void) {
	int msgq;
	key_t key;

	/*  메세지 key값 생성  */
	key = ftok(IPC_KEY_FILENAME, IPC_KEY_PROJ_ID);
	if (key == -1) {

		perror("ftok()");
		return -1;
	}

	/*  메시지 큐 생성  */
	msgq = msgget(key, 0644 | IPC_CREAT);
	if (msgq == -1) {

		perror("msgget()");
		return -1;
	}

	return msgq;

}

static int do_send(long mtype) {

	int msgq;
	struct msgbuf mbuf;
	int messageSize;

	/*  메세지 큐 생성  */
	msgq = init_msgq();
	if (msgq == -1) {
	
		perror("init_msgq()");
		return -1;
	}

	/*  메세지 구조체에 대한 메모리 및 변수 초기화  */
	memset(&mbuf, 0, sizeof(mbuf));
	mbuf.mtype = mtype;
	
	/*  메세지에 데이터 입력 후 메세지 큐에 Send  */
	snprintf(mbuf.string, sizeof(mbuf.string), 
			"hello world mtype %ld\n", mtype);

	messageSize = msgsnd(msgq, &mbuf, sizeof(mbuf.string), 0);
	if(messageSize == -1) {
	
		perror("msgsnd()");
		return -1;
	}

	return 0;
}

static int do_receive(long mtype) {

        int msgq;
	struct msgbuf mbuf;
	int messageSize;

	/*  메세지 큐 생성  */
	msgq = init_msgq();
	if (msgq == -1) {
	
		perror("init_msgq()");
		return -1;
        }
	
	/*  메세지 구조체에 대한 메모리 및 변수 초기화  */
        memset(&mbuf, 0, sizeof(mbuf));
        messageSize = msgrcv(msgq, &mbuf, sizeof(mbuf.string), mtype, 0);

	if(messageSize == -1) {
	
		perror("msgrcv()");
	        return -1;
	}
	
	printf("received msg: mtype %ld, msg [%s]\n", 
			mbuf.mtype, mbuf.string);

	return 0;
}

int main(int argc, char** argv) {

	int ret;
	long mtype;

	if(argc < 3) {
		print_usage(argv[0]);
		return -1;
	}

	/*  메세지 큐에 들어갈 메시지 타입 매개변수로 받아오기  */
	mtype = strtol(argv[2], NULL, 10);

	/*  메세지 큐에 send를 할지, receive를 할 지 매개변수로 결정  */
	if(!strcmp(argv[1], "send")) {

		if(mtype <= 0) {
			print_usage(argv[0]);
			return -1;
		}

		ret = do_send(mtype);

	} else if(!strcmp(argv[1], "recv")) {

		ret = do_receive(mtype);

	} else {

		print_usage(argv[0]);
		return -1;
	}

	return ret;
}
