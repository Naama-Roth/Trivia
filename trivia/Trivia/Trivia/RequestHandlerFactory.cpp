#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* iData) : m_statisticManager(*iData), m_loginManager(*iData), m_iDatabase(iData) {};

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_loginManager;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(m_loginManager, this);
}

StatisticManager& RequestHandlerFactory::getStatisticManager()
{
	return m_statisticManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return m_roomManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
	return new MenuRequestHandler(m_loginManager, user, m_roomManager, m_statisticManager, this);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminReqestHandler(Room room, LoggedUser user)
{
	return new RoomAdminRequestHandler(room, user, m_roomManager, this);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(Room room, LoggedUser user)
{
	return new RoomMemberRequestHandler(room, user, m_roomManager, this);
}