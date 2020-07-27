#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHARED_FILENAME "shared"

struct login_info {
	int pid;	// 프로세스 아이디
	int counter;	// 로그인 횟수
};

static void print_usage(const char *progname) {
	printf("%s (monitor|)\n", progname);
}

static int do_monitoring(void) {
	int fd;
	int n;
	int isSuccess;
	struct login_info local;
	struct login_info *info;
	
	/*  SHARED_FILENAME 파일로 fd 생성  */
	fd = open(SHARED_FILENAME, O_RDWR | O_CREAT, 0644);

	if(fd == -1) {
		perror("open()");
		return -1;
	}
	
	/*  
	 * 파일 크기를 login_info 자료형 크기로 변경  
	 *  login_info 크기 : 8 Byte
	 */
	isSuccess = ftruncate(fd, sizeof(struct login_info));
	if(isSuccess == -1) {
		perror("ftruncate()");
		close(fd);
		return -1;
	}

	/*  커널에게 해당 파일을 Memory 할당 요청 */
	info = mmap(NULL, sizeof(struct login_info),
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(info == MAP_FAILED) {
		perror("mmap()");
		close(fd);
		return -1;
	}

	memset(info, 0, sizeof(struct login_info));

	close(fd);

	memset(&local, 0, sizeof(local));
	n = 0;

	while(n != 5) {
		/*  로컬 데이터와 파일 상의 데이터가 다른지 비교  */
		isSuccess = memcmp(info, &local, sizeof(struct login_info));

		/*  Shared Memory 상의 데이터와 다를 경우 
		 *  1. 새로운 데이터 출력
		 *  2. 메모리에 할당된 파일의 데이터 변경
		 */
		if(isSuccess != 0) {
			printf("pid = %d, counter = %d\n", info->pid, info->counter);
			memcpy(&local, info, sizeof(struct login_info));
			n++;
		}

		sleep(1);
	}
	/*  Shared Memory 할당 해제  */
	munmap(info, sizeof(struct login_info));
	
	return 0;
}

static int do_login(void) {
        int fd;
        struct login_info *info;

        fd = open(SHARED_FILENAME, O_RDWR);
        if (fd == -1) {
                perror("open()");
                return -1;
        }

	/*  커널에게 해당 파일을 Memory 할당 요청  */
        info = mmap(NULL, sizeof(struct login_info),
                         PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (info == MAP_FAILED) {
                perror("mmap()");
                close(fd);
                return -1;
        }

        close(fd);

	/*  메모리에 할당된 공유 데이터를 변경  */
        info->pid = getpid();
        info->counter++;

	/*  메모리 할당 해제  */
        munmap(info, sizeof(struct login_info));

        return 0;
}

int main(int argc, char** argv) {
	int monitor = 0;

	if (argc == 2) {
		if (!strcmp(argv[1], "monitor")) {
 	               monitor = 1;
                } else {
                       print_usage(argv[0]);
                       return -1;
                }
        } else if (argc > 2) {
                print_usage(argv[0]);
                return -1;
        }

        if (monitor) {
                do_monitoring();
        } else {
                do_login();
        }

	return 0;

}
