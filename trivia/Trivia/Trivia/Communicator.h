#pragma once

#include <WinSock2.h>
#include <map>
#include <Windows.h>
#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestHandlerFactory.h"

#define INVALID 0
#define BUFFER_SIZE 8
#define LEN_SIZE 4

using std::map;
using std::string;
using std::cout;
using std::endl;

class Communicator
{
public:
	Communicator(RequestHandlerFactory* hFac);
	~Communicator();

	void startHandleRequests();

private:
	SOCKET m_serverSocket;
	map<SOCKET, IRequestHandler*> m_clients;
	const int PORT = 8826;
	RequestHandlerFactory* m_handleFactory;

	void bindAndListen();
	void handleNewClient(SOCKET);

	codes getIdFromSocket(SOCKET clientSocket, char buff[BUFFER_SIZE]);
	void getPartFromSocket(SOCKET sock, char* buff, int length);
	int convertStringToInt(char* buff, int length);

	void sendResponseToSocket(SOCKET clientSocket, const char* response);
};

