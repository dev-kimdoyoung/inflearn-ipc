# Linux IPC Programming
## Synchronization
### Semaphore 개요
#### 정의
1. 사전적 정의
    - 깃발을 이용한 신호 및 신호 체계

2. 소프트웨어적 정의
    - 임계 영역 (Critical Section)을 보호하기 위한 수단
  
#### Semaphore 종류
- **Counting Semaphore**  
    - 예시 : 의자 앉기 게임  
  
      - N개의 의자와 M명의 사람이 존재  
      - N명 까지는 앉을 수 있지만 N + 1번째 사람은 자리가 날 때까지 대기
  
    - 특징  
  
      - **Semaphore 초기 값을 N**으로 설정  
      - N개의 프로세스는 대기(Blocking) 없이 접근 가능  
      - N + 1번째 사람은 자원 사용이 가능할 때까지 대기(Blocking)  
      - Semaphore를 획득한 프로세스가 release하면 기다리던 프로세스가 깨어나 Semaphore를 획득

- **Binary Semaphore**
    - 특징
  
      - Counting Semaphore과 동일한 원리
      - **Semaphore 초기 값을 1**로 설정
      - 오직 하나의 프로세스가 대기(Blocking) 없이 접근 가능
      - 나머지 프로세스는 자원 사용이 가능할 때까지 대기
      - Semaphore를 획득한 프로세스가 release하면 기다리던 프로세스가 깨어나 Semaphore를 획득
