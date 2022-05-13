#pragma once
#pragma comment (lib, "ws2_32.lib")

#include "Communicator.h"
#include "WSAInitializer.h"
#include "IDatabase.h"


#define EXIT "EXIT"

class Server
{
public:
	Server();
	//~Server();

	void run();

private:
	Communicator* m_communicator;
	IDatabase* m_iDatabase;
	RequestHandlerFactory* m_handleFactory;
};

