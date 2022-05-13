#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "LoginManager.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(LoginManager&, LoggedUser&, RoomManager&, StatisticManager&, RequestHandlerFactory*);

	virtual bool isRequestRelevant(RequestInfo& r) override; //check if the request is login or sign up
	virtual RequestResult handleRequest(RequestInfo r) override; //return result of request

private:
	LoginManager m_loginManager;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	StatisticManager& m_statisticManager;
	RequestHandlerFactory& m_handleFactory;
};

