#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(Room& r, LoggedUser& lu, RoomManager& rm, RequestHandlerFactory* rhf) : m_room(r), m_user(lu), m_roomManager(rm), m_handleFactory(*rhf) {};

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo& r)
{
	return r.requestId == LEAVE_ROOM_ || r.requestId == GET_ROOM_STATE_;
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo r)
{
	IResponse* response = nullptr;

	switch (r.requestId)
	{
	case LEAVE_ROOM_:
	{
		response = new LeaveRoomResponse(1);
		m_room.removeUser(m_user);
		break;
	}

	case GET_ROOM_STATE_:
	{
		RoomData rd = m_room.getRoomData();
		response = new GetRoomStateResponse(m_roomManager.getRoomState(rd.id), rd.activity, rd.numOfQuestionsInGame, m_room.getAllUsers());
		break;
	}

	default:
		break;
	}
	RequestResult res;
	res.response = JsonResponsePacketSerializer::SerializeResponse(response);
	res.newHandler = m_handleFactory.createMenuRequestHandler(m_user);

	return res;
}