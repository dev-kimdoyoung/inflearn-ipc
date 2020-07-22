# Linux IPC Programming
## Data Transfer
### Socket
#### Socket이란?
- 컴퓨터 네트워크 상에서 데이터를 교환하는 종점(EndPoint)을 정의하기 위한 추상적 개념
- Server - Client 소켓이 서로 연결되어야 데이터 통신이 가능
- **각 Socket은 Address를 가지므로** 서로 Identifiying 가능
- 모든 Socket은 **domain과 type**을 지정해야 함

#### Socket Interface
- 데이터 통신을 위한 Application 프로그래밍 인터페이스
- 커널이 제공
- 커널이 서비스하는 TCP/IP network stack을 Application에서 쉽게 이용할 수 있음

#### Domain
- Socket에 어떤 방법으로 address를 부여할지 정의  

| Domain | address 정의 방법 | 기타 |
|:---:|:---:|:---:|
| Unix Domain Socket | `AF_UNIX` : filepath | file descriptor 전송 가능 |
| IPv4 Internet Domain Socket | `AF_INET` : IPv4 주소 + PORT 번호 | |
| IPv6 Internet Domain Socket | `AF_INET6` : IPv6 주소 + PORT 번호 | |

#### 데이터 타입
- Client - Server 소켓 간 통신에 사용할 데이터 타입 정의
- 데이터 타입 종류  

| Socket Type | 특징 | 기타 |
|:---:|:---:|:---:|
| Stream | `SOCK_STREAM` : Connect-orient, Byte Stream, Reliable, 양방향 | TCP 통신에 활용 |
| Datagram | `SOCK_DGRAM` : Connectionless, Unreliable, 양방향 | UDP 통신에 활용 |
