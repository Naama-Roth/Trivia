#pragma once
#include "LoginManager.h"
#include <map>
#include <algorithm>

using std::map;

enum roomState
{
	INACTIVE = 0,
	ACTIVE,
	STARTED_GAME
};

struct RoomData
{
	RoomData() = default;
	unsigned int id;
	string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int activity;
};

class Room
{
public:
	Room() = default;
	Room(RoomData);

	void addUser(LoggedUser); //adds user if the room isn't full
	void removeUser(LoggedUser); //removes a user from the room
	vector<string> getAllUsers(); //returns all the user names

	RoomData getRoomData() const; //returns the data of the room (needed in order to do some stuff in the roomManager

	void changeState(unsigned int state); //changing room's state

private:
	RoomData m_metadata;
	vector<LoggedUser> m_users;

	unsigned int m_countPlayers; //counts the players in the room
};

class RoomManager
{
public:
	RoomManager() = default;

	void createRoom(LoggedUser, RoomData); //creates a anew room
	void deleteRoom(int id); //deletes a room
	unsigned int getRoomState(int id); //return if the room is active or not
	vector<RoomData> getRooms() const;
	map<unsigned int, Room> getAllRooms() const; //returns all the rooms
	int getRoomCount(); //every time you ask for the room counter, the counter will add 1 to the value

private:
	map<unsigned int, Room> m_rooms; //key: room id, value: Room
	int m_currRoomCounter;
};