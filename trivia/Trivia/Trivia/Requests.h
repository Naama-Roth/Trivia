#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "json.hpp"
#include "RoomManager.h"
#include "StatisticManager.h"


using std::string;
using json = nlohmann::json;
using std::string;

//User's Connection Requests

struct LoginRequest
{
	string username;
	string password;
	LoginRequest(std::string newUsername, std::string newPassword) : username(newUsername), password(newPassword) {}
};

struct SignUpRequest
{
	string username;
	string password;
	string email;
	SignUpRequest(std::string newUsername, std::string newPassword, std::string newEmail) : username(newUsername), password(newPassword), email(newEmail) {}
};


//User's Room Requests

struct GetPlayersInRoomRequest
{
	unsigned int roomID;
	GetPlayersInRoomRequest(unsigned int id) : roomID(id) {};
};

struct JoinRoomRequest
{
	unsigned int roomID;
	JoinRoomRequest(unsigned int id) : roomID(id) {};
};

struct CreateRoomRequest
{
	CreateRoomRequest() = default;
	string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeOut;
	CreateRoomRequest(string name, int max, int quesCount, int timeOut) : roomName(name), maxUsers(max), questionCount(quesCount), answerTimeOut(timeOut) {};
};

struct GetRoomStateRequest
{
	RoomData rd;
	GetRoomStateRequest(RoomData roomD) : rd(roomD) {};
};