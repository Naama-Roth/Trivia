#include "Server.h"

Server::Server() 
{
	m_iDatabase = new SqliteDatabase();
	m_handleFactory = new RequestHandlerFactory(m_iDatabase);
	m_communicator = new Communicator(m_handleFactory);
}

//Server::~Server() 
//{
//	delete m_iDatabase;
//	delete m_handleFactory;
//	delete m_communicator;
//}

void Server::run()
{
	WSAInitializer wsaInit;
	m_iDatabase->open();

	std::thread connector(&Communicator::startHandleRequests, this->m_communicator);
	connector.detach();

	string input;
	while (input != EXIT)
	{
		std::cout << "type 'EXIT' to close server" << std::endl;
		std::cin >> input;
	}
	m_iDatabase->close();
}