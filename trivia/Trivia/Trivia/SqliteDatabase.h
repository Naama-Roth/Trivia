#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <io.h>
#include <vector>

#define DB_NAME "triviaDB.sqlite"

#define QUES_ARR_LEN 5
#define Q_Wa_1 1 // 1st wrong answer index
#define Q_Wa_2 2 //2nd wrong answer index
#define Q_Wa_3 3 //3rd wrong answer index
#define Q_Ra 4 //right answer index

#define VECTOR_SIZE 2

using std::cout;
using std::endl;
using std::vector;

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase() = default;
	~SqliteDatabase() = default;

	virtual bool doesUserExist(string) override;
	virtual bool doesPasswordMatch(string, string) override;
	virtual void addNewUser(string, string, string) override;

	virtual bool open() override;
	virtual void close() override;

	//questions
	virtual list<string> getQuestions(int) override; //list of questions

	//statistics
	virtual vector<vector<string>> getFiveBestTimes() override;
	virtual float getPlayerAverageAnswerTime(string) override;
	virtual int getNumOfCorrectAnswers(string) override;
	virtual int getNumOfTotalAnswers(string) override;
	virtual int getNumOfPlayerGames(string) override;

private:
	sqlite3* _db;

	//helpers
	static string convertToString(char* toStr);

	void addBasicUsers(); //building the basic users vector for other adding func \/
	void addBasicUsersToDatabase(vector<vector<string>>& u); //add the users to the database

	void addQuesToVector(); //building the questions vector for other adding func \/
	void addQuesToDatabase(vector<vector<string>>& q); //add the questions to the database

	void addStatsToVector(); //building the basic random stats vector for other adding func \/
	void addStatsToDatabase(vector<vector<string>>& s); //add random statistics to the database

	//callbacks
	static int isTrue(void* data, int argc, char** argv, char** azColName);
	static int callback(void* data, int argc, char** argv, char** azColName);
};

