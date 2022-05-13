#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(LoginManager& loginManager, LoggedUser& loggedUser, RoomManager& roomManager, StatisticManager& statisticManager, RequestHandlerFactory* requestHandlerFactory) : m_handleFactory(*requestHandlerFactory), m_roomManager(roomManager), m_statisticManager(statisticManager), m_user(loggedUser) , m_loginManager(loginManager){};

bool MenuRequestHandler::isRequestRelevant(RequestInfo& r)
{
	return r.requestId == CREATE_ROOM_ || r.requestId == JOIN_ROOM_ || r.requestId == GET_ALL_ROOMS_ || r.requestId == GET_ROOM_PLAYERS_ || r.requestId == GET_PERSONAL_STAT_ || r.requestId == GET_HIGH_SCORE_ || r.requestId == LOGOUT;
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo r)
{
	IResponse* response = nullptr;
	RequestResult result;

	switch (r.requestId)
	{
	case CREATE_ROOM_:
	{
		CreateRoomRequest c = JsonRequestPacketDeserializer::destrializeCreateRoomRequests(r.buffer.c_str());
		RoomData rd;

		//building room data
		rd.id = m_roomManager.getRoomCount();
		rd.name = c.roomName;
		rd.activity = true;
		rd.maxPlayers = c.maxUsers;
		rd.numOfQuestionsInGame = c.questionCount;
		rd.timePerQuestion = c.answerTimeOut;

		//need to check success or fail
		m_roomManager.createRoom(m_user, rd);

		response = new CreateRoomResponse(1);

		map<unsigned int, Room> rooms = m_roomManager.getAllRooms(); //getting all rooms for a specific room
		result.newHandler = m_handleFactory.createRoomAdminReqestHandler(rooms[rd.id], m_user); //creating handler
		break;
	}

	case JOIN_ROOM_:
	{
		JoinRoomRequest j = JsonRequestPacketDeserializer::destrializeJoinRoomRequests(r.buffer.c_str());

		map<unsigned int, Room> rooms = m_roomManager.getAllRooms();
		rooms[j.roomID].addUser(m_user);

		response = new JoinRoomResponse(1);

		result.newHandler = m_handleFactory.createRoomMemberRequestHandler(rooms[j.roomID], m_user); //creating handler
		break;
	}

	case GET_ALL_ROOMS_:
	{
		vector<RoomData> rooms = m_roomManager.getRooms();
		response = new GetRoomResponse(1, rooms);

		result.newHandler = m_handleFactory.createMenuRequestHandler(m_user); //creating handler
		break;
	}

	case GET_ROOM_PLAYERS_:
	{
		GetPlayersInRoomRequest p = JsonRequestPacketDeserializer::destrializeGetPlayersRequests(r.buffer.c_str());

		map<unsigned int, Room> rooms = m_roomManager.getAllRooms();
		response = new GetPlayersInRoomResponse(1, rooms[p.roomID].getAllUsers());

		result.newHandler = m_handleFactory.createMenuRequestHandler(m_user); //creating handler
		break;
	}

	case GET_PERSONAL_STAT_:
	{
		response = new GetPersonalStatResponse(1, m_statisticManager.getUserStatistics(m_user.getUsername()));

		result.newHandler = m_handleFactory.createMenuRequestHandler(m_user); //creating handler
		break;
	}

	case GET_HIGH_SCORE_:
	{
		response = new GetHighScoreResponse(1, m_statisticManager.getHighScore());

		result.newHandler = m_handleFactory.createMenuRequestHandler(m_user); //creating handler
		break;
	}

	case LOGOUT:
	{
		m_loginManager.logout(m_user.getUsername());
		response = new LogoutResponse(1);
		break;
	}
	case GET_ROOM_STATE:
	{
		GetRoomStateRequest rsReq = JsonRequestPacketDeserializer::deserializeGetRoomStateRequest(r.buffer.c_str());
		RoomData rd = rsReq.rd;
		map<unsigned int, Room> rooms = m_roomManager.getAllRooms();

		response = new GetRoomStateResponse(m_roomManager.getRoomState(rd.id), rd.activity, rd.numOfQuestionsInGame, rooms[rd.id].getAllUsers());
		result.newHandler = m_handleFactory.createMenuRequestHandler(m_user); //creating handler
		break;
	}
	case PROMPT:
	{
		cout << "This is my message, You clicked the button" << endl;
		break;
	}
	default:
		result.newHandler = m_handleFactory.createMenuRequestHandler(m_user); //creating default handler
		break;
	}

	//create result
	result.response = JsonResponsePacketSerializer::SerializeResponse(response);
	
	return result;

}