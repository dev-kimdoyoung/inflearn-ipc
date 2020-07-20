#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 *	[개요]
 *	Parent Process에서 PIPE에 메시지를 보내면 Chlid Process에서 PIPE에 
 *	저장된 메시지를 읽어오는 것을 확인
 *
 */
int main(int argc, char **argv) {

	int pipe_fds[2];
	pid_t pid;
	char buf[1024];
	int wstatus;

	/*  PIPE 생성  */
	int isCreatedPipe = pipe(pipe_fds);

	printf("[%d] start of function \n", getpid());
	memset(buf, 0, sizeof(buf));

	if(isCreatedPipe != 0) {
		perror("pipe()");
		return -1;
	}

	/*  프로세스 생성  */
	pid = fork();
	if(pid == 0) {
		/*  child process  */
		close(pipe_fds[1]);

		read(pipe_fds[0], buf, sizeof(buf));
		printf("[%d] parent said ..  %s\n", getpid(), buf);

		close(pipe_fds[0]);

	} else if (pid > 0) {
		/*  parent process  */
		close(pipe_fds[0]);
		
		strncpy(buf, "hello child~", sizeof(buf) - 1);
		write(pipe_fds[1], buf, strlen(buf));

		close(pipe_fds[1]);

		pid = wait(&wstatus);

	} else {
		/*  process 생성 실패 시  */
		perror("fork()");
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return -1;
	}

	return 0;
}
