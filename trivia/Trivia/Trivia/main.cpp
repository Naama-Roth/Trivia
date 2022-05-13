#include "Server.h"

int main()
{
	Server server;
	try
	{
		server.run();
	}
	catch (std::exception & e)
	{
		cout << e.what() << endl;
	}
	return 0;
}