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
