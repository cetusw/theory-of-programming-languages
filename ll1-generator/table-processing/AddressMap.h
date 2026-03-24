#pragma once
#include "../grammar-parser/grammar/Grammar.h"
#include <map>
#include <string>
#include <vector>

class AddressMap
{
public:
	std::map<std::string, int> ntRows;
	std::map<int, int> rhsRows;
	int acceptRowId = 0;

	void Calculate(const std::vector<Production>& rules);
};
