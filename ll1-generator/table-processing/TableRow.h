#pragma once
#include <string>
#include <vector>
#include <set>

struct TableRow {
	int id;
	std::string currentSymbol;
	std::set<std::string> guidingSymbols;
	bool shift;
	bool error;
	int pointer;
	int pushStackAddress;
	bool endOfParsing;
};

using Table = std::vector<TableRow>;