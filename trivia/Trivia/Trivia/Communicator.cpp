#include "Communicator.h"

static const unsigned int IFACE = 0;

static std::chrono::high_resolution_clock packetClock; //checking the time each packet has been recieved

Communicator::Communicator(RequestHandlerFactory* hFac)
{
	m_handleFactory = hFac;
}

Communicator::~Communicator() 
{
	map<SOCKET, IRequestHandler*>::iterator it;
	for ( it = m_clients.begin(); it != m_clients.end(); it++)
	{
		delete it->second;
	}
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(m_serverSocket);
	}
	catch (...) {}
}

void Communicator::bindAndListen()
{
	m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");

	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;

	// again stepping out to the global namespace
	if (::bind(m_serverSocket, (struct sockaddr*) & sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	std::cout << "binded..." << std::endl;

	if (::listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << PORT << "..." << std::endl;
}

void Communicator::handleNewClient(SOCKET client)
{
	char buffer[BUFFER_SIZE];
	int res = 0;

	try
	{
		while (true)
		{
			//reciving login / sign up request

			//creating requestInfo
			RequestInfo r;
			//get id
			char buff[BUFFER_SIZE] = { 0 };

			r.requestId = getIdFromSocket(client, buff);
			getPartFromSocket(client, buff, LEN_SIZE);
			int dataLen = convertStringToInt(buff, LEN_SIZE); //getting length

			//getting data
			char buffer[] = { 0 };
			if (dataLen > 0)
			{
				getPartFromSocket(client, buffer, dataLen);
			}
			r.buffer = buffer;

			RequestResult reqResult = m_clients[client]->handleRequest(r);

			m_clients[client] = reqResult.newHandler; //putting new handler

			sendResponseToSocket(client, reqResult.response.c_str());
		}
	}
	catch (std::exception & e)
	{
		std::cout << "Exception was thrown in function: " << e.what() << std::endl;
	}
	cout << "out" << '\n';
}



void Communicator::startHandleRequests()
{
	SOCKET client;

	packetClock.now();

	try
	{
		bindAndListen();

		while (true)
		{
			// the main thread is only accepting clients 
			// and add them to the map of handlers
			cout << "accepting client..." << endl;
			client = ::accept(m_serverSocket, NULL, NULL);
			if (client == INVALID)
				throw std::exception(__FUNCTION__ " - accept");

			cout << "Client accepted !" << endl;

			// create new thread for client	and detach from it
			std::thread tr(&Communicator::handleNewClient, this, client);
			tr.detach();
			LoginRequestHandler* handler = m_handleFactory->createLoginRequestHandler();
			m_clients.insert(std::pair<SOCKET, IRequestHandler*>(client, handler));
		}
	}
	catch (std::exception & e)
	{
		std::cout << "Exception was thrown in function: " << e.what() << std::endl;
	}
}

codes Communicator::getIdFromSocket(SOCKET clientSocket, char buff[BUFFER_SIZE])
{
	char id = '0';

	getPartFromSocket(clientSocket, buff, 1);
	id = buff[0];
	
	return (codes)(id);
}

void Communicator::getPartFromSocket(SOCKET sock, char* buff, int length)
{
	do
	{
		recv(sock, buff, length, 0);
	} while (buff == INVALID);
	
	buff[length] = 0;
}

int Communicator::convertStringToInt(char* buff, int length)
{
	int result = 0;
	for (int i = 0; i < length; i++)
	{
		result += (int)(buff[i]);
	}
	return result;
}

void Communicator::sendResponseToSocket(SOCKET clientSocket, const char* response)
{
	std::string responseStr(response);

	if (send(clientSocket, response, responseStr.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending response to client");
	}
}
