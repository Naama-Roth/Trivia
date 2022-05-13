#include "RoomManager.h"

Room::Room(RoomData data)
{
	m_metadata = data;
	m_countPlayers = 0;
}

void Room::addUser(LoggedUser user)
{
	if (m_countPlayers < m_metadata.maxPlayers) //checks if the room is not full
	{
		m_users.push_back(user);
		m_countPlayers++;
	}
	else
	{
		throw std::exception("User cannot be added, room is full.");
	}
}

void Room::removeUser(LoggedUser user)
{
	vector<LoggedUser>::iterator toRemove = m_users.end();
	for (vector<LoggedUser>::iterator it = m_users.begin(); it != m_users.end(); it++) //finds the user's itreator
	{
		if ((*it).getUsername() == user.getUsername())
		{
			toRemove = it;
			break;
		}
	}

	if (m_users.end() == toRemove) //if couldnt find the user
	{
		throw std::exception("User does not exist and cannot be removed.");
	}
	m_users.erase(toRemove);
}

vector<string> Room::getAllUsers()
{
	if (m_users.empty())
	{
		throw std::exception("No users were added.");
	}

	vector<string> users;
	for (int i = 0; i < m_users.size(); i++)
	{
		users.push_back(m_users[i].getUsername());
	}
	return users;
}

RoomData Room::getRoomData() const
{
	return m_metadata;
}

void Room::changeState(unsigned int state)
{
	m_metadata.activity = state;
}




void RoomManager::createRoom(LoggedUser user, RoomData data)
{
	if (m_rooms.find(data.id) != m_rooms.end()) //if room exist
	{
		throw std::exception("The room is already created.");
	}
	m_rooms[data.id] = Room(data);
	m_rooms[data.id].addUser(user);
}

void RoomManager::deleteRoom(int id)
{
	if (m_rooms.find(id) == m_rooms.end())
	{
		throw std::exception("Room does not exist.");
	}
	m_rooms.erase(id);
}

unsigned int RoomManager::getRoomState(int id)
{
	return m_rooms[id].getRoomData().activity;
}

vector<RoomData> RoomManager::getRooms() const
{
	vector<RoomData> rooms;
	for (auto& room : m_rooms)
	{
		rooms.push_back(room.second.getRoomData());
	}
	return rooms;
}

map<unsigned int, Room> RoomManager::getAllRooms() const
{
	return m_rooms;
}

int RoomManager::getRoomCount()
{
	m_currRoomCounter++;
	return m_currRoomCounter - 1;
}