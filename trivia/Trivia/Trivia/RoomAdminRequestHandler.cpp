#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(Room& r, LoggedUser& lu, RoomManager& rm, RequestHandlerFactory* rhf) : m_room(r), m_user(lu), m_roomManager(rm), m_handleFactory(*rhf) {};

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo& r)
{
	return r.requestId == CLOSE_ROOM_ || r.requestId == START_GAME_ || r.requestId == GET_ROOM_STATE_;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo r)
{
	IResponse* response = nullptr;
	RoomData rd;

	switch (r.requestId)
	{
	case CLOSE_ROOM_:
	{
		response = new CloseRoomResponse(1);
		m_room.changeState(INACTIVE);
		break;
	}

	case START_GAME_:
	{
		response = new StartGameResponse(1);
		m_room.changeState(STARTED_GAME);
		break;
	}

	case GET_ROOM_STATE_:
		rd = m_room.getRoomData();
		response = new GetRoomStateResponse(m_roomManager.getRoomState(rd.id), rd.activity, rd.numOfQuestionsInGame, m_room.getAllUsers());
		break;

	default:
		break;
	}

	RequestResult res;
	res.response = JsonResponsePacketSerializer::SerializeResponse(response);
	res.newHandler = m_handleFactory.createMenuRequestHandler(m_user);

	return res;
}