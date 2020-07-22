# Linux IPC Programming
## Data Transfer
### 사전 지식
#### Byte Stream
- 정의 : 입력과 출력 사이에서 이동하는 byte의 흐름
- 종류 : 읽기 (read) 스트림, 쓰기 (write) 스트림

#### File Descriptor (FD, 파일 기술자)
- 정의 : 프로세스에서 특정 파일에 접근할 때 사용하는 추상적인 값
- fd는 0을 포함한 양의 정수 값을 가짐
- Unix System에서 정규파일, 디렉토리, 소켓, 파이프 등 모든 객체들은 파일로써 관리되는데  
  해당 **System 내 프로세스가 위 파일들을 접근할 때** fd라는 개념을 이용
   
### Pipe
#### Pipe
- int pipe(pipe_fd[2])
  * pipe[0] : write 스트림
  * pipe[1] : read 스트림
- 단방향 Byte Stream
- Name 또는 ID가 없음 : **related process** 간에 사용 가능
- 각 프로세스는 **read 또는 write 중 하나의 스트림 만을 가져가는 것**이 좋음
  * parent 프로세스에서 child 프로세스로 데이터를 안전하게 전송하기 위함
  * 각 프로세스가 read/write 스트림 둘 다 open한 상태일 경우  
    parent에서 child로 데이터 전송 시 child가 아닌 parent가 데이터를 read할 경우가 존재
- write size가 PIPE_BUF 크기를 넘어서지 않도록 하자
  * **interleaved**될 위험성이 있으니 유념
 
#### Named Pipe
- int mkfifo(const char *pathname, mode_t mode);
  * pathname : 생성할 named pipe 파일 경로
  * mode : 해당 파일에 부여할 권한 ex) 0644
- 단방향 Byte stream
- 파일 경로가 ID : **unrelated process** 간에도 사용 가능
- FIFO 생성과 open이 분리되어 있음
  * open()과 mkfifo()를 별도로 해주어야 함
- open() 시 read()와 write()가 동기화 됨
  * **read()와 write() 모두 open되어 있어야 success**
  
