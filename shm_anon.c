#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

/**
 *	[개요]
 *	부모 프로세스에서 익명 파일의 데이터를 공유 메모리에 할당 후
 *	다수의 자식 프로세스가 해당 메모리에 할당된 데이터를 성공적으로 공유하는지 확인
 */
struct login_info {
	int pid;
	int counter;
};

static int do_monitoring(struct login_info *info) {
	int n;
	struct login_info local;
	
	memset(&local, 0, sizeof(local));
	n = 0;
	
	while(1) {

		/*  공유 메모리 상의 데이터와 local 데이터가 다른지 비교  */
		if(memcmp(info, &local, sizeof(struct login_info))) {
		/*  데이터가 서로 다를 경우  */

			/*  공유 메모리 데이터 출력  */
			printf("pid = %d, counter = %d\n", 
					info->pid, info->counter);
			/*  local 구조체에 공유 메모리 데이터 복사  */
			memcpy(&local, info, sizeof(struct login_info));
			n++;
			if(n == 5) 
				break;
		}
		/*  microseconds(ms)초 sleep  */
		usleep(100000);		// 0.1초 sleep
	}

	/*  공유 메모리 할당 해제  */
	munmap(info, sizeof(struct login_info));
	
	return 0;
}

/*  자식 프로세스에서 공유 메모리 데이터 변경  */
static int do_login(int index, struct login_info *info) {
	sleep(index + 1);

	info->pid = getpid();
	info->counter++;

	return 0;
}


/*  커널에게 login_info 자료형에 대한 Shared Memory 할당 요청  */
struct login_info *login_info_init(void) {
	struct login_info *info;

	/*  커널에게 익명 파일에 대한 메모리 할당 요청  */
	info = mmap(NULL, sizeof(struct login_info), 
			PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	if(info == MAP_FAILED) {
		perror("mmap()");
		return NULL;
	}
	
	memset(info, 0, sizeof(struct login_info));
	
	return info;
}

int main(int argc, char** argv) {
	int iterator;
	int pid;
	struct login_info *info;

	/*  익명 파일에 대한 메모리 할당 요청  */
	info = login_info_init();
	if(info == 0) {
		perror("login_info_init()");
		return -1;
	}

#define NUM_FORK	5
	/*  자식 프로세스 5회 생성  */
	for(iterator = 0; iterator < NUM_FORK; iterator++) {
		pid = fork();
		if(pid > 0) {
		/*  parent process  */

		} else if(pid == 0) {
		/*  child process  */

			/*  공유 메모리 데이터 변경  */
			do_login(iterator, info);

			/*  공유 메모리 할당 해제  */
			munmap(info, sizeof(struct login_info));

			return 0;
		} else {
		/*  error  */

			perror("fork()");
			return -1;
		}
	}

	do_monitoring(info);
	
	for(iterator = 0; iterator < NUM_FORK; iterator++) {
		/*  프로세스 대기(wait) 상태로 변경  */
		pid = wait(NULL);
		printf("pid %d is end\n", pid);
	}
	return 0;
}


