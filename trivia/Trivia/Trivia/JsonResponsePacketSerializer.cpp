#include "JsonResponsePacketSerializer.h"

LoginResponse::LoginResponse(unsigned int stat) { responseType = LOGIN; status = stat; }

SignupResponse::SignupResponse(unsigned int stat) { responseType = SIGNUP; status = stat; }

LogoutResponse::LogoutResponse(unsigned int stat) { responseType = LOGOUT; status = stat; }


GetRoomResponse::GetRoomResponse(unsigned int stat, vector<RoomData> rooms) { responseType = GET_ALL_ROOMS; status = stat; m_rooms = rooms; }

CreateRoomResponse::CreateRoomResponse(unsigned int stat) { responseType = CREATE_ROOM; status = stat; }

JoinRoomResponse::JoinRoomResponse(unsigned int stat) { responseType = JOIN_ROOM; status = stat; }


GetPlayersInRoomResponse::GetPlayersInRoomResponse(unsigned int stat, vector<string> players) { responseType = LOGOUT; status = stat; m_players = players; }

GetHighScoreResponse::GetHighScoreResponse(unsigned int stat, vector<vector<string>> statistics) { responseType = GET_HIGH_SCORE; status = stat; m_p1 = statistics[0]; m_p2 = statistics[1]; m_p3 = statistics[2]; m_p4 = statistics[3]; m_p5 = statistics[4]; }

GetPersonalStatResponse::GetPersonalStatResponse(unsigned int stat, vector<string> statistics) { responseType = GET_PERSONAL_STAT; status = stat; m_statistics = statistics; }

CloseRoomResponse::CloseRoomResponse(unsigned int stat) { responseType = CLOSE_ROOM; status = stat; }

StartGameResponse::StartGameResponse(unsigned int stat) { responseType = START_GAME; status = stat; }

GetRoomStateResponse::GetRoomStateResponse(unsigned int stat, bool hasBegun, unsigned int qCount, vector<string> p) { responseType = GET_ROOM_STATE; status = stat; hasGameBegun = hasBegun; questionCount = qCount; players = p; }

string StatusResponse::toJson()
{
	json j;
	j[STATUS] = status;
	return j.dump();
}

string ErrorResponse::toJson()
{
	json j;
	j[MESSAGE] = message;
	return j.dump();
}

string GetRoomResponse::toJson()
{
	json j;
	j[STATUS] = status;

	vector<string> roomsData;
	string stringData;
	for (auto room : m_rooms)
	{
		stringData = "Id: " + to_string(room.id) + ", Name: " + room.name + ", Max players number: " + to_string(room.maxPlayers) +
			", Number of questions: " + to_string(room.numOfQuestionsInGame) + ", Time for each question: " + to_string(room.timePerQuestion);
		roomsData.push_back(stringData);
	}
	j[ROOMS] = roomsData;

	return j.dump();
}

string GetPlayersInRoomResponse::toJson()
{
	json j;
	j[STATUS] = status;
	j[PLAYERS_IN_ROOM] = m_players;

	return j.dump();
}

string GetHighScoreResponse::toJson()
{
	json j;
	j[STATUS] = status;
	j[P1] = m_p1;
	j[P2] = m_p2;
	j[P3] = m_p3;
	j[P4] = m_p4;
	j[P5] = m_p5;

	return j.dump();
}

string GetPersonalStatResponse::toJson()
{
	json j;
	j[STATUS] = status;
	j[STATISTICS] = m_statistics;

	return j.dump();
}

string GetRoomStateResponse::toJson()
{
	json j;
	j[STATUS] = status;
	j[GAME_BEGAN] = hasGameBegun;
	j[PLAYERS] = players;
	j[NUMBER_OF_QUESTIONS] = questionCount;

	return j.dump();
}

string JsonResponsePacketSerializer::getPaddedNumber(int num, int digits)
{
	const int digit = 10;
	int numberOfDigits = 0;
	int number = num;
	for (number; number != 0; number /= digit, numberOfDigits++) {} //gets the number of digits
	string padded;
	for (int i = 0; i < (digits - numberOfDigits); i++) //finds how many digits are missing and adds them
	{
		padded += "0";
	}
	padded += std::to_string(num);
	return padded;
}

string JsonResponsePacketSerializer::SerializeResponse(IResponse* response)
{
	string jsonString = "";
	try
	{
		jsonString = response->toJson();
	}
	catch (...)
	{
		throw("User Left");
	}
	std::cout << "cout ser -> res: " << jsonString << '\n';

	string responseString;
	responseString += codes(response->responseType);
	
	//COUT
	std::cout << "ser cout -> size: " + std::to_string(jsonString.size()) << '\n';


	responseString += getPaddedNumber(jsonString.size(), FOUR_BYTES);
	responseString += jsonString;

	return responseString;
}