// Load_read.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <string>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PortNum 5024;
int iRet;
SOCKET clientSocket;

void Welcome_info();
int Send_Recv();
int main()
{
#pragma region Socketlib_init

	WSADATA wsaData;														//init the WSADATA for socket lib

	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);							//socket lib init
	if (iRet != 0)															//error catch
	{
		cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!";		//fatal error, end the programe
		return -1;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))		//WSADATA version check
	{
		WSACleanup();														//version not right, please check the lib install
		cout << "WSADATA version is not correct!";
		return -1;
	}

#pragma endregion

#pragma region Socket_init

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);									//socket init
	if (clientSocket == INVALID_SOCKET)												//fatal error, end the progame
	{
		cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!";
		return -1;
	}

	/*int iMode = 1;
	iRet = ioctlsocket(clientSocket, FIONBIO, (u_long FAR*) & iMode);
	if (iRet == SOCKET_ERROR)
	{
		cout << "ioctlsocket failed!\n";
		WSACleanup();
		return -1;
	}*/

#pragma endregion

#pragma region Welcome_info

	Welcome_info();				//version information

#pragma endregion

#pragma region Server_setting

	char ip_addr[20];										//for ip address
	int port = PortNum;										//for prot number
	cout << "Server IP: ";
	cin >> ip_addr;
	cout << "Connecting...";
	SOCKADDR_IN srv_Addr;									//sturcture for ip address
	srv_Addr.sin_addr.S_un.S_addr = inet_addr(ip_addr);		//use old version function "inet_addr",change it if you want
	srv_Addr.sin_family = AF_INET;							//stipulate the family format
	srv_Addr.sin_port = htons(port);						//define port number

#pragma endregion

#pragma region Socket_connent

	while (true)
	{
		iRet = connect(clientSocket, (SOCKADDR*)& srv_Addr, sizeof(SOCKADDR));								//try to connect
		if (0 != iRet)
		{
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK || err == WSAEINVAL)													//connection bolcked, wait and try again
			{
				Sleep(500);
				continue;
			}
			else if (err == WSAEISCONN)
			{
				break;																						//connect successful
			}
			else
			{
				cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) execute failed!";		//fatal error, end programe
				closesocket(clientSocket);
				WSACleanup();
				return -1;
			}

		}
	}
	cout << "OK\n";																							//connection information and time
	cout << "Connected!\n";
	for (int i = 0; i < 100; i++)
	{
		cout << ">";
	}
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	cout << "\n***************************************************************";
	cout << "\n******************Time and Information*************************\n*******************";
	cout << st.wMonth << "/" << st.wDay << "/" << st.wYear << " " << st.wHour << ":" << st.wMinute << ":" << st.wSecond;
	cout << "**************************\n**********";
	cout << "Server IP: " << "192.168.11.106" << " Port: " << "501" << "******************\n";
	cout << "***************************************************************\n\n";

#pragma endregion

#pragma region Main_UI



	char Wel_re[1000];
	recv(clientSocket, Wel_re, sizeof(Wel_re), 0);
	cout << Wel_re;
	cout << "\nPlease begin: \n";
	Send_Recv();
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;

#pragma endregion
}

void Welcome_info()
{
	std::cout << "Engine statr\n";
	std::cout << "Version: 0.0.1\n";
	for (int i = 0; i < 100; i++)
	{
		cout << ">";
	}
	cout << "\n";
}

int Send_Recv()
{
	char Sd[1000];
	char Re[1000];
	while (true)
	{
		cin >> Sd;
		if (Sd == "eee")
		{
			break;
		}
		while(true)
		{
			iRet = send(clientSocket, (const char*)Sd, sizeof(Sd), 0);								//try send the data
			if (SOCKET_ERROR == iRet)
			{
				int err = WSAGetLastError();
				if (err == WSAEWOULDBLOCK)
				{
					Sleep(500);																						//send blocked, wait and try again
					continue;
				}
				else
				{
					printf("send failed!\n");																		//fatal error end programe
					closesocket(clientSocket);
					WSACleanup();
					return -1;
				}
			}
			break;
		}
		while (true)
		{
			ZeroMemory(Re, 1000);																		//receive data buffer
			iRet = recv(clientSocket, Re, sizeof(Re), 0);												//non-blocking receive
			if (SOCKET_ERROR == iRet)
			{
				int err = WSAGetLastError();
				if (err == WSAEWOULDBLOCK)
				{
					Sleep(100);																						//receive blocked, wait and try again
					continue;
				}
				else if (err == WSAETIMEDOUT || err == WSAENETDOWN)
				{
					printf("recv failed!\n");																		//fatal error, end programe
					closesocket(clientSocket);
					WSACleanup();
					return -1;
				}
				break;
			}
			break;
		}
		cout << Re;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
