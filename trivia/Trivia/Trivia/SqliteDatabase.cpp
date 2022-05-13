#include "SqliteDatabase.h"

bool SqliteDatabase::open()
{
	int doesFileExist = _access((const char*)DB_NAME, 0);
	int res = sqlite3_open((const char*)DB_NAME, &_db);
	if (res != SQLITE_OK) 
	{
		_db = nullptr;
		cout << "Failed to open DB" << endl;
		return false;
	}
	
	
	//create users
	if (doesFileExist == -1)
	{
		char* sqlStatement = (char*)"CREATE TABLE users(name TEXT PRIMARY KEY, password TEXT NOT NULL, email TEXT NOT NULL);";
		char* errMessage = nullptr;
		res = sqlite3_exec(_db, sqlStatement, nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
			return false;
		cout << "opened  users" << '\n';
	}

	addBasicUsers();

	//create questions
	if (doesFileExist == -1)
	{
		char* sqlStatement = (char*)"CREATE TABLE questions(id INT PRIMARY KEY, question TEXT NOT NULL, wrongAnswer1 TEXT NOT NULL, wrongAnswer2 TEXT NOT NULL, wrongAnswer3 TEXT NOT NULL, trueAnswer TEXT NOT NULL);";
		char* errMessage = nullptr;
		res = sqlite3_exec(_db, sqlStatement, nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
			return false;

		
		cout << "opened  ques" << '\n';
	}
	//adding questions:
	addQuesToVector();

	//create statistics
	if (doesFileExist == -1)
	{
		char* sqlStatement = (char*)"CREATE TABLE statistics(name TEXT FOREIGNKEY REFERENCES users(name) NOT NULL, numTotalAns INT, avgTime float, numRightAns INT, totalGamesPlayed INT);";
		char* errMessage = nullptr;
		res = sqlite3_exec(_db, sqlStatement, nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
			return false;
		
		cout << "opened  stats" << '\n';
	}
	//adding stats
	addStatsToVector();

	return true;
}

void SqliteDatabase::close()
{
	sqlite3_close(_db);
	_db = nullptr;
}

//User's Funcs
bool SqliteDatabase::doesUserExist(string name)
{
	bool exist = false;
	string sqlStatement = "SELECT * FROM users WHERE name = '" + name + "';";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), isTrue, &exist, &errMessage);
	if (res != SQLITE_OK)
		cout << "there was a problem getting the user " + res << errMessage << endl;
	return exist;
}

bool SqliteDatabase::doesPasswordMatch(string name, string password)
{
	bool exist = false;
	string sqlStatement = "SELECT * FROM users WHERE name = '" + name + "' AND password = '" + password + "';";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), isTrue, &exist, &errMessage);
	if (res != SQLITE_OK)
		cout << "there was a problem getting the user " + res << errMessage << endl;
	return exist;
}

void SqliteDatabase::addNewUser(string name, string password, string email)
{
	string sqlStatement = "INSERT INTO users (name, password, email) VALUES ('" + name + "', '" + password + "', '" + email + "');";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		cout << "there was a problem creating the album " + res << errMessage << endl;
}



//Question's Funcs
list<string> SqliteDatabase::getQuestions(int)
{
	vector<string> ques;
	string sqlStatement = "SELECT question FROM questions;";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), callback, &ques, &errMessage);

	if (res != SQLITE_OK)
		cout << "there was a problem getting the questions: " << errMessage << endl;

	//make vector list
	list<string> lQue(ques.begin(), ques.end());
	return lQue;
}


//Statistics's Func
vector<vector<string>> SqliteDatabase::getFiveBestTimes()
{
	vector<string> sqlAns;
	string sqlStatement = "SELECT name, avgTime FROM statistics ORDER BY avgTime DESC LIMIT 5;"; //get 5 best result
	char* errMessage = nullptr;

	int res = sqlite3_exec(_db, sqlStatement.c_str(), callback, &sqlAns, &errMessage);
	if (res != SQLITE_OK)
		cout << "there was a problem getting the times: " + res << errMessage << endl;

	//orginazing vector strings
	vector<vector<string>> bestResult;
	for (int i = 0; i < sqlAns.size(); i++)
	{
		string str = sqlAns[i];
		vector<string> user;
		
		//slicing the str
		user.push_back(str.substr(0, str.find(", "))); 
		str = str.substr(str.find(",") + VECTOR_SIZE, str.size() - VECTOR_SIZE);
		user.push_back(str);


		bestResult.push_back(user);
	}
	return bestResult;
}

float SqliteDatabase::getPlayerAverageAnswerTime(string pName)
{
	vector<string> sum;
	string sqlStatement = "SELECT avgTime FROM statistics WHERE name = '" + pName + "';";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), callback, &sum, &errMessage);

	if (res != SQLITE_OK)
		cout << "there was a problem getting the time: " + res << errMessage << endl;

	return std::stof(sum[0]); //the answer from callback is in the first (and only) value in vector
}

int SqliteDatabase::getNumOfCorrectAnswers(string pName)
{
	vector<string> rAns;
	string sqlStatement = "SELECT numRightAns FROM statistics WHERE name = '" + pName + "';";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), callback, &rAns, &errMessage);

	if (res != SQLITE_OK)
		cout << "there was a problem getting the summary: " + res << errMessage << endl;

	return std::stoi(rAns[0]); //the answer from callback is in the first (and only) value in vector
}

int SqliteDatabase::getNumOfTotalAnswers(string pName)
{
	vector<string> tAns;
	string sqlStatement = "SELECT numTotalAns FROM statistics WHERE name = '" + pName + "';";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), callback, &tAns, &errMessage);
	
	if (res != SQLITE_OK)
		cout << "there was a problem getting the summary: " + res << errMessage << endl;

	return std::stoi(tAns[0]); //the answer from callback is in the first (and only) value in vector
}

int SqliteDatabase::getNumOfPlayerGames(string pName)
{
	vector<string> sum;
	string sqlStatement = "SELECT totalGamesPlayed FROM statistics WHERE name = '" + pName + "';";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), callback, &sum, &errMessage);

	if (res != SQLITE_OK)
		cout << "there was a problem getting the summary: " + res << errMessage << endl;

	return std::stoi(sum[0]); //the answer from callback is in the first (and only) value in vector
}



//callbacks
int SqliteDatabase::isTrue(void* data, int argc, char** argv, char** azColName)
{
	bool* exist = (bool*)data;
	*exist = true;
	return 0;
}

int SqliteDatabase::callback(void* data, int argc, char** argv, char** azColName)
{
	vector<string>* m = static_cast<vector<string>*>(data);

	try {
		string str;
		int i = 0;
		str = convertToString(argv[0]);

		for (i = 1; i < argc; i++)
		{
			str += ", ";
			str += convertToString(argv[i]);
		}
		m->emplace_back(str);
	}
	catch (...)
	{
		// abort select on failure, don't let exception propagate thru sqlite3 call-stack
		return 1;
	}
	return 0;
}


//helpers
string SqliteDatabase::convertToString(char* toStr)
{
	string str;
	int counter = 0;
	while (toStr[counter] != 0)
	{
		str = str + toStr[counter];
		counter++;
	}
	return str;
}


void SqliteDatabase::addBasicUsers()
{
	vector<vector<string>> u;
	u.push_back({ "Admin Roni", "Roni123", "Roni@Ron" });
	u.push_back({ "Admin Naama", "Naama123", "Naama@Nam" });
	u.push_back({ "Admin N1", "N123", "N1@N" });
	u.push_back({ "Admin R1", "R123", "R1@R" });
	u.push_back({ "Admin Noa", "Noa123", "Noa@No" });

	addBasicUsersToDatabase(u);
}

void SqliteDatabase::addBasicUsersToDatabase(vector<vector<string>>& u)
{
	bool exist = false;
	string sqlStatement = "";
	char* errMessage = nullptr;

	for (int i = 0; i < u.size(); i++)
	{
		sqlStatement = "Select name FROM users WHERE name = '" + u[i][0] + "';"; //get stats

		int res = sqlite3_exec(_db, sqlStatement.c_str(), isTrue, &exist, &errMessage); //check if stats exists
		if (res != SQLITE_OK)
			cout << "there was a problem getting the user " + res << errMessage << endl;

		if (!exist)
		{
			cout << "added user!" << '\n';
			sqlStatement = "INSERT INTO users (name, password, email) VALUES ('" + u[i][0] + "', '" + u[i][Q_Wa_1] + "', '" + u[i][Q_Wa_2] + "');";

			//adding question
			res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
			if (res != SQLITE_OK)
				cout << "there was a problem adding the statistics " + res << errMessage << endl;
		}
	}
}

void SqliteDatabase::addQuesToVector()
{
	vector<vector<string>> v;

	v.push_back({ "How old is the oldest woman on earth (in guinness records)?", "100", "104", "132", "122" });

	v.push_back({ "What is the tallest building on earth?", "Taipei 101 (Taipei)", "Eiffel Tower (France)", "Shanghai Tower (China)", "Burj Khalifa (Dubai)" });

	v.push_back({ "What is Queen Elizabeth II's surname?", "Buckingham","Middleton ", "Lannister", "Windsor" });

	v.push_back({ "What's a baby rabbit called?", "Kindling ", "Doe", "Bunny", "Kittens" });

	v.push_back({ "How many religions are there", "23", "12", "109", "10,000" });

	v.push_back({ "\"What does kakorrhaphiophobia\" means?", "Fear of heights", "Fear of blood", "Fear of insects", "Fear of failure" });

	v.push_back({ "How long is the longest chess game ever?", "10h", "7h", "14h and 34m", "20h" });

	v.push_back({ "In what year did The Beatles split up?", "1984", "1963", "1872", "1970" });

	v.push_back({ "Which US state was Donald Trump born in?", "New Jersey", "Tennessee", "Washington", "New York" });

	v.push_back({ "How long is the longest word in English?", "183", "1,909", "92", "189,819" }); //its The chemical composition of titin, the largest known protein
	
	addQuesToDatabase(v);
}

void SqliteDatabase::addQuesToDatabase(vector<vector<string>>& q)
{
	bool exist = false;
	string sqlStatement = "";
	char* errMessage = nullptr;

	for (int i = 0; i < q.size(); i++)
	{
		sqlStatement = "Select question FROM questions WHERE question = '" + q[i][0] + "';"; //get question
		
		int res = sqlite3_exec(_db, sqlStatement.c_str(), isTrue, &exist, &errMessage); //check if question exists
		if (res != SQLITE_OK)
			cout << "there was a problem getting the user " + res << errMessage << endl;

		if (!exist)
		{
			sqlStatement = "INSERT INTO questions (id, question, wrongAnswer1, wrongAnswer2, wrongAnswer3, trueAnswer ) VALUES ('" + std::to_string(i + 1) + "', '" + q[i][0] + "', '" + q[i][Q_Wa_1] + "', '" + q[i][Q_Wa_2] + "', '" + q[i][Q_Wa_3] + "', '" + q[i][Q_Ra] + "');";

			//adding question
			res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
			if (res != SQLITE_OK)
				cout << "there was a problem adding the question " + res << errMessage << endl;
		}
	}
}

void SqliteDatabase::addStatsToVector()
{
	vector<vector<string>> s;

	s.push_back({ "Admin Roni", "12", "4.06", "10", "4" });
	s.push_back({ "Admin Naama", "23", "3.53", "17", "10" });
	s.push_back({ "Admin N1", "17", "6.47", "15", "8" });
	s.push_back({ "Admin R1", "35", "2.41", "21", "11" });
	s.push_back({ "Admin Noa", "41", "3.32", "37", "23" });

	addStatsToDatabase(s);
}

void SqliteDatabase::addStatsToDatabase(vector<vector<string>>& s)
{
	bool exist = false;
	string sqlStatement = "";
	char* errMessage = nullptr;

	for (int i = 0; i < s.size(); i++)
	{
		sqlStatement = "Select name FROM statistics WHERE name = '" + s[i][0] + "';"; //get stats

		int res = sqlite3_exec(_db, sqlStatement.c_str(), isTrue, &exist, &errMessage); //check if stats exists
		if (res != SQLITE_OK)
			cout << "there was a problem getting the stats " + res << errMessage << endl;

		if (!exist)
		{
			sqlStatement = "INSERT INTO statistics (name, numTotalAns, avgTime, numRightAns, totalGamesPlayed) VALUES ('" + s[i][0] + "', '" + s[i][Q_Wa_1] + "', '" + s[i][Q_Wa_2] + "', '" + s[i][Q_Wa_3] + "', '" + s[i][Q_Ra] + "');";

			//adding question
			res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
			if (res != SQLITE_OK)
				cout << "there was a problem adding the statistics " + res << errMessage << endl;
		}
	}
}