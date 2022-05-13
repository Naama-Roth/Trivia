#pragma once
#include "Responses.h"

#define FOUR_BYTES 4

#define MESSAGE "message"
#define STATUS "status"
#define ROOMS "Rooms"
#define PLAYERS_IN_ROOM "PlayersInRoom"
#define STATISTICS "statistics"
#define P1 "p1"
#define P2 "p2"
#define P3 "p3"
#define P4 "p4"
#define P5 "p5"
#define GAME_BEGAN "hasGameBegun"
#define PLAYERS "players"
#define NUMBER_OF_QUESTIONS "questionCount"

class JsonResponsePacketSerializer
{
private:
	static string getPaddedNumber(int num, int digits = FOUR_BYTES);

public:
	static string SerializeResponse(IResponse*);
};