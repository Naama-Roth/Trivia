#include "StatisticManager.h"

StatisticManager::StatisticManager(IDatabase& database) : m_db(database) {}

vector<string> StatisticManager::getUserStatistics(string pName)
{
	vector<string> vStatistics;

	string avgTime = "Average answer time: " + to_string(m_db.getPlayerAverageAnswerTime(pName));
	vStatistics.push_back(avgTime);

	string allAns = "All answers: " + to_string(m_db.getNumOfTotalAnswers(pName));
	vStatistics.push_back(allAns);

	string currAns = "Corecct answers: " + to_string(m_db.getNumOfCorrectAnswers(pName));
	vStatistics.push_back(currAns);

	string totGames = "Games played: " + to_string(m_db.getNumOfPlayerGames(pName));
	vStatistics.push_back(totGames);

	return vStatistics;
}

vector<vector<string>> StatisticManager::getHighScore()
{
	return m_db.getFiveBestTimes();
}