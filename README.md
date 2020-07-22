# Linux IPC Programming
## 개요
Linux에서 동작하는 IPC(Inter-Process Communication, 프로세스 간 통신)을 이해하고 이에 대한 예제 코드를 작성

## 강의 목표
### 1. Linux IPC 도구의 종류 및 특징을 파악한다.
### 2. IPC 도구를 이용해 프로그래밍하는 방법을 익힌다.
### 3. 소프트웨어 요구사항에 맞는 IPC 도구를 선택해 설계/구현할 수 있는 능력을 배양한다.

## 사전 지식
### Byte Stream
- 정의 : 입력과 출력 사이에서 이동하는 byte의 흐름
- 종류 : 읽기 (read) 스트림, 쓰기 (write) 스트림

### File Descriptor (FD, 파일 기술자)
- 정의 : 프로세스에서 특정 파일에 접근할 때 사용하는 추상적인 값
- fd는 0을 포함한 양의 정수 값을 가짐
- Unix System에서 정규파일, 디렉토리, 소켓, 파이프 등 모든 객체들은 파일로써 관리되는데  
  해당 **System 내 프로세스가 위 파일들을 접근할 때** fd라는 개념을 이용

## 강의 링크
### [Inlfearm Linux IPC Programming](https://www.inflearn.com/course/%EB%A6%AC%EB%88%85%EC%8A%A4IPC%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D/dashboard "해당 Inflearn 강의로 이동")

## 목차
### 1. Data Transfer
#### 1. Pipe/Named Pipe(FIFO) : [Pipe Source Code Link](https://github.com/dev-kimdoyoung/inflearn-ipc/tree/data-transfer/byte-stream "Pipe Branch로 이동")
#### 2. Message (POSIX, SysV) : [Message Source Code Link](https://github.com/dev-kimdoyoung/inflearn-ipc/tree/data-transfer/message "Message Branch로 이동")
#### 3. Socket (Stream / Datagram) : [Socket Source Code Link](https://github.com/dev-kimdoyoung/inflearn-ipc/tree/data-transfer/socket "Socket Branch로 이동")
---
### 2. Shared Memory

---
### 3. Synchronization

---
