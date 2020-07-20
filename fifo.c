#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_FILENAME "./testfifo"

/*
 *	[개요]
 *	Named Pipe(FIFO)와 open()을 별도 생성 후 
 *	FIFO가 정상 작동하는지 read(), write()를 사용하여 확인 
 *
 */
static void print_usage(char *progName) {
	printf("%s (w|r)\n", progName);
	return;

}

static int do_reader(void) {
	int fd;
	char buf[128];
	
	/*  FIFO_FILENAME에 정의된 파일 이름을 읽기(Read) 전용으로 Open  */
	fd = open(FIFO_FILENAME, O_RDONLY);
	if(fd < 0) {
		/*  파일 Open 실패 시  */
		perror("open()");
		return -1;
	}

	/*  Named PIPE에 해당 파일 명으로 저장된 데이터를 read  */
	read(fd, buf, sizeof(buf));
	printf("writer said... %s\n", buf);

	close(fd);

	return 0;
}

static int do_writer(void) {
	int fd;
	char buf[128];

	printf("make fifo\n");
	/* FIFO_FILENAME에 정의된 파일 이름과 동일한 파일 제거 */
	unlink(FIFO_FILENAME);

	/*  Named Pipe(FIFO) 생성  */
	int isCreatedFifo = mkfifo(FIFO_FILENAME, 0644);
	if(isCreatedFifo) {
		/*  FIFO 생성 실패 시  */
		perror("mkfifo()");
		return -1;
	}

	printf("call open()\n");
	/*  FIFO_FILENAME에 정의된 파일 이름을 쓰기(Write) 전용으로 Open  */
	fd = open(FIFO_FILENAME, O_WRONLY);
	if(fd < 0) {
		/*  파일 open 실패 시  */
		perror("open()");
		return -1;
	}

	/*  Named PIPE에 해당 파일 명을 ID로 하여 데이터 write  */
	strncpy(buf, "hello", sizeof(buf));
	write(fd, buf, strlen(buf));

	close(fd);

	return 0;
}

int main(int argc, char **argv) {

	if(argc < 2) {
		print_usage(argv[0]);
		return -1;
	}

	if(!strcmp(argv[1], "r")) {
		do_reader();
	} else if(!strcmp(argv[1], "w")) {
		do_writer();
	} else {
		print_usage(argv[0]);
		return -1;
	}

	return 0;
}
