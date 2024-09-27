#pragma once

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>

using namespace std;


#pragma comment(lib,"ws2_32.lib")

int main()
{
	//원속 초기화 (ws2_32라이브러리 초기화)
	//관련 정보가 wsaData에 채워짐
	//MAKEWORD는 2.2버전을 요청한다는 뜻
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "start up 에러" << endl;
		return 0;
	}

	//ad : Address Family(AF_INET = IPv4, AF_INET6 = IPv6))
		//type : TCP(SOCK_STREAM) vs  UDP(SOCK_DGRAM)
		//PROTOCAL : 0 -> 다양한 프로토콜을 선택할수있지만 tcp환경이라 실수를안하게 0으로 셋팅
		//return : descriptor
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		int32_t errCode = ::WSAGetLastError();
		cout << "socket create fail listen소켓 생성 실패" << endl;
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}


	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(5252);
	//htons -host to network short
	//Little-Endian vs Big-Endian
	//ex) 0x12345678 4바이트 정수
	//low[0x78][0x56][0x34][0x12]high < Little-Endian
	//low[0x12][0x34][0x56][0x78]high < Big-Endian 
	// 디버그 찍어서 sin_port를 볼려고하면 엉뚱한값이 나올 것이다. 리틀 또는 빅 엔디안으로 저장이되는데 cpu에따라 다름
	//현대 intel amd등 메모리에 저장을할때 리틀엔디안 방식으로 저장이되지만 네트워크 TCP/IP프로토콜은 빅 엔디안을 사용한다.


	//소켓 바인딩
	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32_t errCode = ::WSAGetLastError();
		cout << "socket bind fail 소켓 바인딩 실패" << endl;
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	//두번쨰 인자의 경우 백로그를 받는데 서버 소켓이 동시에 수용할 수 있는 연결 요청의 최대 개수를 의미한다.
	if (::listen(listenSocket, 10) == SOCKET_ERROR)
	{
		int32_t errCode = ::WSAGetLastError();
		cout << "listen 실패" << endl;
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	while (true)
	{
		//클라이언트에 대한 정보를 담는 것
		SOCKADDR_IN clientAddr;
		memset(&clientAddr, 0, sizeof(clientAddr));
		int32_t addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			int32_t errCode = ::WSAGetLastError();
			cout << "socket accpet 실패" << endl;
			cout << "Socket ErrorCode : " << errCode << endl;
			return 0;
		}
		//클라이언트 정보
		char ipAddress[16];
		inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "Client Connect.... Client ip : [" << ipAddress << "]" << endl;
	}


	//윈속 종료
	::WSACleanup();
}