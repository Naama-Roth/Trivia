#pragma once
#include "IDatabase.h"
#include <vector>

using std::vector;
using std::to_string;

class StatisticManager
{
public:
	StatisticManager(IDatabase& database);
	vector<vector<string>> getHighScore();
	vector<string> getUserStatistics(string);
private:
	IDatabase& m_db;
};

