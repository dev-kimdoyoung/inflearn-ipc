# Linux IPC Programming
## Data Transfer
### Message
#### Message Queue 개념
- **Message**를 데이터 단위로 하여 통신
- FIFO(First-In, First-Out)를 이용한 메세지 큐
- **Unrelated Process** 간에도 사용 가능

#### SysV Message Queue
- 특징
  * message 별 **message Type** 지원  
    ☞ 메세지 별 타입 지정 및 수정 가능
  * **IPC Key** 기반 Identifier  
    ☞ **fd가 아닌 것을** 유념하자
  * 메세지 큐 관리 도구  
    ☞ ipcs : IPC Object 관련 정보 조회
    ☞ ipcrm : IPC Object 삭제
    
- int msgget(key_t key, int msgflg)  
  * 메세지 큐 ID를 구하는 함수로 옵션에 따라 생성 가능
  * key_t : 임의의 IPC Objecty Key | IPC_PRIVATE (시스템에서 자동으로 할당)
  * msgflg : IPC_CREAT (key에 매칭되는 queue ID 없을 시 생성) | IPC_EXCL (key에 매칭되는 queue ID 있을 시 에러)
 
#### POSIX Message Queue
- 특징
  * **File I/O** 기반의 동작  
    ☞ I/O multiplexing 가능  
    ☞ SysV와 달리 **fd 기반**으로 동작한다.
  * Message **우선순위** 적용 가능
  * **Message Notification** 기능  
    ☞ 해당 메세지 큐가 다른 작업을 하는 중간에 메세지가 들어올 경우 들어온 메세지 READ 가능
  * Multi Thread Safe
  * 메세지 큐 관리 도구  
    ☞ **가상 파일 시스템 기반 mount**  
    ☞ CLI에서 "mount | grep queue" 명령어를 통해 메세지 큐가 어떤 파일로 관리되는지 확인 가능  
     *예) /dev/mqueue*
     
- mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr)  
  * 메세지 큐 생성하여 열기
  * name : 메세지 큐 이름  
    ☞ 반드시 /로 시작해야 한다 *ex) "/tmp/testmq"*  
  * oflog : O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_EXCL, ..  
  * mode : permisson  
    *ex) 0644)*  
  * attr : 속성 지정  
    ☞ 메세지 큐 표준 속성 구조체  
  ```
   struct mq_attr {
     long mq_flags;   // 0 또는 O_NONBLOCK
     long mq_maxmsg;  // 큐에 들어갈 수 있는 메세지 최대 (max) 개수
     long mq_msgsize; // 메세지 최대 (max) 사이즈
     long mq_curmsgs; // 현재 큐에 입력된 메세지 개수
   };
  ```
