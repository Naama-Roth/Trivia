#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "json.hpp"
#include "RoomManager.h"
#include "StatisticManager.h"

using std::string;
using json = nlohmann::json;
using std::vector;

enum codes : char
{
	ERROR_C = 'A',
	LOGIN,
	SIGNUP,
	LOGOUT,
	CREATE_ROOM,
	JOIN_ROOM,
	GET_ROOM,
	GET_ALL_ROOMS,
	GET_ROOM_PLAYERS,
	GET_HIGH_SCORE,
	GET_PERSONAL_STAT,
	CLOSE_ROOM,
	START_GAME,
	GET_ROOM_STATE,
	LEAVE_ROOM,
	PROMPT
};

/*
IResponse is an interface for all Responses
that'll be serialized into Json format.
*/
struct IResponse
{
	codes responseType;
	virtual string toJson() = 0;
};

struct StatusResponse : public IResponse
{
	unsigned int status;
	virtual string toJson() override;
};

struct ErrorResponse : public IResponse
{
	std::string message;
	ErrorResponse(std::string msg) : message(msg) { responseType = ERROR_C; }
	string toJson() override;
};


//User's Connsction Responses
struct LoginResponse : public StatusResponse
{
	LoginResponse(unsigned int stat);
};

struct SignupResponse : public StatusResponse
{
	SignupResponse(unsigned int);
};

struct LogoutResponse : public StatusResponse
{
	LogoutResponse(unsigned int stat);
};


//User's Room Responses

struct GetRoomResponse : public StatusResponse
{
	GetRoomResponse(unsigned int stat, vector<RoomData> rooms);
	vector<RoomData> m_rooms;
	virtual string toJson() override;
};

struct CreateRoomResponse : public StatusResponse
{
	CreateRoomResponse(unsigned int);
};

struct JoinRoomResponse : public StatusResponse
{
	JoinRoomResponse(unsigned int);
};

struct GetPlayersInRoomResponse : public StatusResponse
{
	GetPlayersInRoomResponse(unsigned int, vector<string> players);
	vector<string> m_players;
	virtual string toJson() override;
};

struct CloseRoomResponse : public StatusResponse
{
	CloseRoomResponse(unsigned int stat);
};

struct StartGameResponse : public StatusResponse
{
	StartGameResponse(unsigned int stat);
};

struct GetRoomStateResponse : public StatusResponse
{
	bool hasGameBegun;
	vector<string> players;
	unsigned int questionCount;
	virtual string toJson() override;
	GetRoomStateResponse(unsigned int stat, bool hasBegun, unsigned int qCount, vector<string> p);
};

struct LeaveRoomResponse : public StatusResponse
{
	LeaveRoomResponse(unsigned int stat) { status = stat; }
};


//User's Statistic Responses

struct GetHighScoreResponse : public StatusResponse
{
	GetHighScoreResponse(unsigned int, vector<vector<string>> statistics);
	vector<string> m_p1;
	vector<string> m_p2;
	vector<string> m_p3;
	vector<string> m_p4;
	vector<string> m_p5;
	virtual string toJson() override;
};

struct GetPersonalStatResponse : public StatusResponse
{
	GetPersonalStatResponse(unsigned int, vector<string> statistics);
	vector<string> m_statistics;
	virtual string toJson() override;
};