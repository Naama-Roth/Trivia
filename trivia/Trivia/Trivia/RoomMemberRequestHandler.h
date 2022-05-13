#pragma once
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(Room& r, LoggedUser& lu, RoomManager& rm, RequestHandlerFactory* rhf);
	virtual bool isRequestRelevant(RequestInfo&) override;
	virtual RequestResult handleRequest(RequestInfo) override;

private:
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handleFactory;
};

