#pragma once
#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <thread>

using namespace std;


#pragma comment(lib,"ws2_32.lib")

int main()
{

	//원속 초기화 (ws2_32라이브러리 초기화)
	//관련 정보가 wsaData에 채워짐
	//MAKEWORD는 2.2버전을 요청한다는 뜻
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		cout << "start up 에러" << endl;

	//ad : Address Family(AF_INET = IPv4, AF_INET6 = IPv6))
	//type : TCP(SOCK_STREAM) vs  UDP(SOCK_DGRAM)
	//PROTOCAL : 0 -> 다양한 프로토콜을 선택할수있지만 tcp환경이라 실수를안하게 0으로 셋팅
	//return : descriptor
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		int32_t errCode = ::WSAGetLastError();
		cout << "socket create fail 클라이언트 소켓 생성 실패" << endl;
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}


	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");<=구식
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(5252);
	//htons -host to network short
	//Little-Endian vs Big-Endian
	//ex) 0x12345678 4바이트 정수
	//low[0x78][0x56][0x34][0x12]high < Little-Endian
	//low[0x12][0x34][0x56][0x78]high < Big-Endian 
	// 디버그 찍어서 sin_port를 볼려고하면 엉뚱한값이 나올 것이다. 리틀 또는 빅 엔디안으로 저장이되는데 cpu에따라 다름
	//현대 intel amd등 메모리에 저장을할때 리틀엔디안 방식으로 저장이되지만 네트워크 TCP/IP프로토콜은 빅 엔디안을 사용한다.

	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32_t errCode = ::WSAGetLastError();
		cout << "connet failed 통신 연결 실패 " << endl;
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	cout << "Connected To Server!" << endl;

	while (true)
	{
		//TODO 
		char sendBuffer[100] = "Hello World!";

		int32_t resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR)
		{
			int32_t errCode = ::WSAGetLastError();
			cout << "message send failed 메세지 전송 실패" << endl;
			cout << "Socket ErrorCode : " << errCode << endl;
			return 0;
		}
		cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;

		char recvBuffer[1000];
		int32_t  recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen <= 0)
		{
			int32_t errCode = ::WSAGetLastError();
			cout << "message recv failed 메세지 전송 실패" << endl;
			cout << "Socket ErrorCode : " << errCode << endl;
			return 0;
		}
		cout << "Recv Dadta! Len = " << recvBuffer << endl;
		cout << "Recv Dadta! Len = " << recvLen << endl;

		this_thread::sleep_for(1s);
	}


	//소켓 자원 반환
	::closesocket(clientSocket);
	//윈속 종료
	::WSACleanup();
}

