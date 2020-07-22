#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/**
 *
 *  [개요]
 *  Unix Domain 소켓 기반으로 같은 호스트 내에서 Client - Server 간
 *  Byte stream 데이터를 주고받는 것을 확인
 *
 **/

/*  Server 소켓 주소로 사용할 파일 이름 지정  */
#define SOCK_PATH	"sock_stream_un"

static void print_usage(const char *progname) {
	printf("%s (server | client)\n)", progname);
}

static int do_server(void) {
	int sock;
	int isBindSuccess;
	struct sockaddr_un addr;
        int peer;
	int ret;
	char buf[128];

	/*  Unix domain 방식 & Byte Stream 메세지 format으로 소켓 오픈  */
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock == -1) {

		perror("socket()");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));


	/*  Domain 방식 및 SOCK_PATH에 지정된 파일 명으로 소켓 주소 지정  */
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

	/*  소켓에 위에서 지정한 구조체를 address로 부여  */
	isBindSuccess = bind(sock, (struct sockaddr *)&addr, 
			     sizeof(struct sockaddr_un));

	if(isBindSuccess == -1) {
	
		perror("bind()");
		close(sock);
		return -1;

	}

	/*  socket 통신 대기 상태  */
	listen(sock, 5);

	/*  client에게 통신 요청이 들어오면 accept 후
         *  요청한 client의 socket descriptor 값 저장  */
	peer = accept(sock, NULL, NULL);

	if(peer < 0) {

		perror("accept()");
		close(sock);
		return -1;
	}

	memset(buf, 0, sizeof(buf));

	/*  데이터 수신 및 출력  */
	ret = recv(peer, buf, sizeof(buf), 0);
	
	if(ret == -1) {
	
		perror("recv()");
		close(sock);
		return -1;
	}

	printf("client said [%s]\n", buf);

	close(peer);
	close(sock);

	return 0;
}

static int do_client(void) {
	int sock;
	struct sockaddr_un addr;
	char buf[128];
	int ret;
	int isConnectSuccess;

	/*  Unix domain 방식 & Byte Stream 메세지 format으로 소켓 오픈  */
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock < 0) {

		perror("socket()");
		return -1;
	}
	memset(&addr, 0, sizeof(addr));

	/*  Domain 방식 및 SOCK_PATH에 지정된 파일 명으로 소켓 주소 지정
*/
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) -1);

	/*  서버 소켓에 접속 시도  */
	isConnectSuccess = connect(sock, (struct sockaddr *) &addr, 
			   sizeof(struct sockaddr_un));
	if(isConnectSuccess == -1) {

		perror("connect()");
		close(sock);
		return -1;
	}

	memset(buf, 0, sizeof(buf));

	snprintf(buf, sizeof(buf), "this is msg from sock_stream");

	/*  서버로 데이터 전송  */
	ret = send(sock, buf, sizeof(buf), 0);

	if(ret < 0) {

		perror("send()");
		close(sock);
		return -1;
	}

	close(sock);
	return 0;
}

int main(int argc, char** argv) {
	int ret;

	if(argc < 2) {
		print_usage(argv[0]);
		return -1;
	}

	if(!strcmp(argv[1], "server")) {
	
		ret = do_server();
	} else if(!strcmp(argv[1], "client")) {
         
	 	ret = do_client();
        } else {
		print_usage(argv[0]);
		return -1;
	}

	return ret;
}
