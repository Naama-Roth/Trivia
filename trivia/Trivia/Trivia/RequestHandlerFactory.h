#pragma once
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h"
#include "SqliteDatabase.h"
#include "RoomManager.h"
#include "StatisticManager.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* iData);
	
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();

	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	StatisticManager& getStatisticManager();
	RoomManager& getRoomManager();

	RoomAdminRequestHandler* createRoomAdminReqestHandler(Room room, LoggedUser user);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(Room room, LoggedUser user);

private:
	LoginManager m_loginManager;
	IDatabase* m_iDatabase;
	RoomManager m_roomManager;
	StatisticManager m_statisticManager;
};

