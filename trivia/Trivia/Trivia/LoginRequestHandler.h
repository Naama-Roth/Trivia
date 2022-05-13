#pragma once
//#define _CRT_SECURE_NO_WARNINGS
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

//class IRequestHandler;
class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(LoginManager loginManager, RequestHandlerFactory* handleFactory);

	virtual bool isRequestRelevant(RequestInfo& r) override; //check if the request is login or sign up
	virtual RequestResult handleRequest(RequestInfo r) override; //return result of request


private:
	LoginManager m_loginManager;
	RequestHandlerFactory& m_handleFactory;

	int login(string username, string password); //checks if the user can login and logging them in if can
	int signup(string username, string password, string email); //checks if the user can signup and signing them in if can
};
