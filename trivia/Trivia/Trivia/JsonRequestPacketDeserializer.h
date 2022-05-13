#pragma once
#include "Requests.h"

#define USER_NAME "username"
#define PASSWORD "password"
#define EMAIL "email"
#define ID "id"
#define ROOM_NAME "roomName"
#define MAX_USERS "maxUsers"
#define QUES_COUNT "questionCount"
#define ANSWER_TIME_OUT "answerTimeOut"
#define NAME "name"
#define MAX_PLAYERS "maxPlayers"
#define NUM_OF_QUES "numOfQuestionsInGame"
#define TIME_PER_QUE "timePerQuestion"
#define ACTIVITY "isActive"


#define LEN_USER_NAME 11 //' "username": '
#define LEN_PASSWORD 11 //' "password": '
#define LEN_EMAIL 8 //' "email": '

#define FOUR_BYTES 4
#define IND_DATA_IN_BUFF 5


class JsonRequestPacketDeserializer
{
public:
	//Connection Funcs
	static LoginRequest deserializeLoginRequests(const char* buffer); //destrialize login req
	static SignUpRequest deserializeSignUpRequests(const char* buffer); //destrialize sign up req

	//Room Funcs
	static GetPlayersInRoomRequest destrializeGetPlayersRequests(const char* buffer);
	static JoinRoomRequest destrializeJoinRoomRequests(const char* buffer);
	static CreateRoomRequest destrializeCreateRoomRequests(const char* buffer);
	static GetRoomStateRequest deserializeGetRoomStateRequest(const char* buffer);

private:
	static json converCharArrToJson(string);
};

