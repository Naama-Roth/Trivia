#pragma once
#include <iostream>
#include <vector>
#include "IDatabase.h"

using std::string;
using std::vector;

class LoggedUser
{
public:
	LoggedUser() = default;
	LoggedUser(string);
	string getUsername();

private:
	string m_username;
};


class LoginManager
{
public:
	LoginManager(IDatabase& database);

	LoggedUser getLoggedUser(string name);

	bool signup(string, string, string); //adds a new user to the DB if it doesn't already exist
	bool login(string, string); //adds a new user to the logged users vector if it already exist in the DB
	bool logout(string); //removes a user from the logged users vector

private:
	IDatabase& m_database;
	vector<LoggedUser> m_loggedUsers;
};

