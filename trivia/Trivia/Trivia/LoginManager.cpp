#include "LoginManager.h"

LoggedUser::LoggedUser(string name)
{
	m_username = name;
}

string LoggedUser::getUsername()
{
	return m_username;
}


LoginManager::LoginManager(IDatabase& database) : m_database(database) {}

bool LoginManager::login(string name, string password)
{
	if (m_database.doesUserExist(name) && m_database.doesPasswordMatch(name, password))
	{
		m_loggedUsers.push_back(LoggedUser(name));
		return true;
	}
	return false;
}

bool LoginManager::signup(string name, string password, string email)
{
	if (!(m_database.doesUserExist(name)))
	{
		m_database.addNewUser(name, password, email);
		m_loggedUsers.push_back(LoggedUser(name));
		return true;
	}
	return false;
}

bool LoginManager::logout(string name)
{
	vector<LoggedUser>::iterator toRemove;
	bool isExist = false;
	for (vector<LoggedUser>::iterator it = m_loggedUsers.begin(); it != m_loggedUsers.end(); it++)
	{
		if ((*it).getUsername() == name)
		{
			toRemove = it;
			isExist = true;
		}
	}
	if (isExist)
	{
		m_loggedUsers.erase(toRemove);
	}
	return isExist;
}

LoggedUser LoginManager::getLoggedUser(string name)
{
	for (auto it = m_loggedUsers.begin(); it != m_loggedUsers.end(); it++)
	{
		if (it->getUsername() == name)
		{
			return *it;
		}
	}
	throw std::exception("Cannot find user.");
}