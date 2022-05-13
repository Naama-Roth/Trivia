#pragma once
#include <iostream>
#include <time.h>
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "LoginManager.h"


struct RequestResult;
class IRequestHandler;


using std::string;
using json = nlohmann::json;
using std::vector;

enum code : char
{
	ERROR_ = 'A',
	LOGIN_,
	SIGNUP_,
	LOGOUT_,
	CREATE_ROOM_,
	JOIN_ROOM_,
	GET_ROOM_,
	GET_ALL_ROOMS_,
	GET_ROOM_PLAYERS_,
	GET_HIGH_SCORE_,
	GET_PERSONAL_STAT_,
	CLOSE_ROOM_,
	START_GAME_,
	GET_ROOM_STATE_,
	LEAVE_ROOM_
};

enum Status
{
	SUCCESS = 1,
	FAIL
};

struct RequestInfo
{
	RequestInfo();
	int requestId;
	time_t recivalTime;
	string buffer;
}typedef RequestInfo;


struct RequestResult
{
	string response;
	IRequestHandler* newHandler;
}typedef RequestResult;


class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo& r) = 0;
	virtual RequestResult handleRequest(RequestInfo r) = 0;
};
