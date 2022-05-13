#include "JsonRequestPacketDeserializer.h"

json JsonRequestPacketDeserializer::converCharArrToJson(string buffer)
{
	string strBuff = buffer;
	strBuff = strBuff.substr(0, strBuff.find('}') + 1);

	return json::parse(strBuff);
}

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequests(const char* buffer)
{
	json jsonMessage = converCharArrToJson(buffer);

	std::cout << "destrializer req from client: " << jsonMessage.dump(4) << std::endl;

	return LoginRequest(jsonMessage[USER_NAME], jsonMessage[PASSWORD]);
}

SignUpRequest JsonRequestPacketDeserializer::deserializeSignUpRequests(const char* buffer)
{
	json jsonMessage = converCharArrToJson(buffer);

	std::cout << "destrializer req from client: " << jsonMessage.dump(4) << std::endl;

	return SignUpRequest(jsonMessage[USER_NAME], jsonMessage[PASSWORD], jsonMessage[EMAIL]);
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::destrializeGetPlayersRequests(const char* buffer)
{
	json jsonMessage = converCharArrToJson(buffer);

	std::cout << "req from client: " << jsonMessage.dump(1) << std::endl;

	return GetPlayersInRoomRequest(jsonMessage[ID]);
}

JoinRoomRequest JsonRequestPacketDeserializer::destrializeJoinRoomRequests(const char* buffer)
{
	json jsonMessage = converCharArrToJson(buffer);

	std::cout << "req from client: " << jsonMessage.dump(1) << std::endl;

	return JoinRoomRequest(jsonMessage[ID]);
}

CreateRoomRequest JsonRequestPacketDeserializer::destrializeCreateRoomRequests(const char* buffer)
{
	json jsonMessage = converCharArrToJson(buffer);

	std::cout << "req from client: " << jsonMessage.dump(4) << std::endl;

	return CreateRoomRequest(jsonMessage[ROOM_NAME], jsonMessage[MAX_USERS], jsonMessage[QUES_COUNT], jsonMessage[ANSWER_TIME_OUT]);
}

GetRoomStateRequest JsonRequestPacketDeserializer::deserializeGetRoomStateRequest(const char* buffer)
{
	json jsonMessage = converCharArrToJson(buffer);

	std::cout << "req from client: " << jsonMessage.dump(4) << std::endl;

	RoomData rd;
	rd.id = jsonMessage[ID];
	rd.maxPlayers = jsonMessage[MAX_PLAYERS];
	rd.name = jsonMessage[NAME];
	rd.numOfQuestionsInGame = jsonMessage[NUM_OF_QUES];
	rd.timePerQuestion = jsonMessage[TIME_PER_QUE];
	rd.activity = 1;

	return GetRoomStateRequest(rd);
}