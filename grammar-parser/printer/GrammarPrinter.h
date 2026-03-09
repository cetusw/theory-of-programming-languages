#pragma once

#include "grammar/Grammar.h"
#include <iosfwd>
#include <map>

using StringRules = std::map<std::string, std::vector<std::string>>;
const std::string RULE_OPERATOR = "->";
const std::string OR_OPERATOR = "|";

class GrammarPrinter
{
public:
	static void Print(std::ostream& os, const Grammar& grammar);

private:
	static StringRules GetStringRules(const std::vector<Production>& rules);
	static void PrintRhsAlternatives(std::ostream& os, const std::vector<std::string>& alternatives);
	static void PrintStartRule(std::ostream& os, const StringRules& stringRules, const std::string& start);
	static void PrintRules(std::ostream& os, const StringRules& stringRules, const std::string& start);
};
