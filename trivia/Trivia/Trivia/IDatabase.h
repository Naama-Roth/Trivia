#pragma once
#include <iostream>
#include <string>
#include <List>
#include <vector>

using std::list;
using std::string;
using std::vector;

class IDatabase
{
public:
	virtual ~IDatabase() = default;

	virtual bool doesUserExist(string) = 0;
	virtual bool doesPasswordMatch(string, string) = 0;
	virtual void addNewUser(string, string, string) = 0;	

	virtual bool open() = 0;
	virtual void close() = 0;

	virtual list<string> getQuestions(int) = 0; //list of questions
	virtual vector<vector<string>> getFiveBestTimes() = 0;
	virtual float getPlayerAverageAnswerTime(string) = 0;
	virtual int getNumOfCorrectAnswers(string) = 0;
	virtual int getNumOfTotalAnswers(string) = 0;
	virtual int getNumOfPlayerGames(string) = 0;
};

