현재 가장 간단한 TCP/IP 소켓 통신 구현 상태이다.(에코서버)

소켓이 데이터를 주고받을때 운영체제 레벨에있는 Recvbuffer 와 Sendbuffer의 딜레마는 해결되지 않은 상태이다.

만약 한쪽에서 다른 작업을처리하느라 Recv버퍼가 꽉 찰떄가지 recv를 하지않는다면?  혹은 sendbuffer가 가득찬상태에서 send를보낸다면?  상대방은 가득 찬 recvbuffer를 한번에 recv로 받게된다면? 등을 생각해볼수있는 코드라고생각한다.

